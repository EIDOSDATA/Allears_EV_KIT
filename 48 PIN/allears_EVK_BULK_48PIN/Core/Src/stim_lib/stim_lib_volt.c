/*
 * stim_lib_volt.c
 *
 *  Created on: Apr 27, 2023
 *      Author: eidos
 */

#include <stdlib.h>

#include "stim_lib_volt.h"
#include "stim_lib_current.h"
#include "stim_lib_st_init.h"
#include "stim_lib_private.h"
#include "stim_lib_stim_cfg.h"

#include "td_debug.h"

/* STEPUP VOLTAGE PARAMETER */
#define R1_Vstup									3600 /* 3.6M Ohm */
#define R2_Vstup									110 /* 110K Ohm */
#define STEPUP_VOLTAGE_SCALE_SIZE					10000
#define STEPUP_DEC_PLACES							4
#ifdef STIM_LIB_EVKIT_CV
#define STEPUP_FDBK_VOLT_RANGE						(int)(1.5 * STEPUP_VOLTAGE_SCALE_SIZE)
#endif
#ifdef STIM_LIB_EVKIT_CC
#define STEPUP_FDBK_VOLT_RANGE						(int)(1.8 * STEPUP_VOLTAGE_SCALE_SIZE)
#endif

/* STEPUP REFERENCE VALUE :: EXTERN */
#define STEPUP_REF_VALUE							exStimLib_stepup_table
extern stim_lib_stepup_ref_value_t exStimLib_stepup_table[STIM_LIB_STEPUP_TABLE_SIZE];

/* STEPUP CONTROL VALUE :: PULSE and Counter */
int exStimLib_voltageCtrlPulse = 0;
int gStimLIb_stepupFeedbackCnt = 0;

/* STEPUP :: CONTROL FLAG */
bool gStimLib_slopeCtrl_F = false;
bool gStimLib_stepupDataPrint_F = false;
bool exStimLib_voltageStabilized_F = false;

/* STEPUP STATUS CHECK :: CONTROL FLAG */
bool exStimLib_stepupStatus = false;

/* STEPUP FEEDBACK :: ADC1 BUFFER and CLAC Value */
uint16_t exStimLib_adc1_conv_buff[STIM_LIB_ADC1_TOTAL_SIZE]; /* ADC1 */
uint16_t gStimLib_setpupBuff[STIM_LIB_ADC1_TOTAL_SIZE]; /* ADC1 */
uint32_t gStimLib_stepupFeedback_adcAvg;
uint32_t gStimLib_stepupFeedback_volt;

/*
 * STEPUP CONTROL SCHEDULER
 * Calling from "HAL_TIM_PeriodElapsedCallback()" and "stimLib_stepup_ctrlCallback()" Function
 * Check out the "stim_lib_st_inc.c" file.
 * */
void stimLib_stepup_ctrlScheduler(void)
{
	STIM_LIB_STEPUP_FEEDBACK_CNT++;
	stimLib_adc1_readBuffer(gStimLib_setpupBuff, exStimLib_adc1_conv_buff, STIM_LIB_ADC1_TOTAL_SIZE);
	/* SLOPE VOLTAGE RISE CONTROL */
	if (STIM_LIB_STEPUP_FEEDBACK_CNT == 10 && STIM_LIB_SLOPE_CTRL_END_FLAG == false)
	{
		stimLib_stepup_voltFeedback();
		STIM_LIB_STEPUP_FEEDBACK_CNT = 0;
		STIM_LIB_STEPUP_DATA_PRINT_FLAG = true;
	}
	/* VOLTAGE RANGE KEEPING CONTROL */
	if (STIM_LIB_STEPUP_FEEDBACK_CNT == 1 && STIM_LIB_SLOPE_CTRL_END_FLAG == true)
	{
		stimLib_stepup_voltFeedback();
		STIM_LIB_STEPUP_FEEDBACK_CNT = 0;
		STIM_LIB_STEPUP_DATA_PRINT_FLAG = true;
	}
	/* STEP UP DATA PRINT */
#ifdef STIM_LIB_STEPUP_DATA_PRINT
	stimLib_stepup_dataPrint();
#endif

}

/*
 * STEP UP VOLTAGE FEEDBACK
 * */
void stimLib_stepup_voltFeedback(void)
{
	/* STEPUP DATA CLAC */
	gStimLib_stepupFeedback_adcAvg = stimLib_stepup_adcAVG(gStimLib_setpupBuff, STIM_LIB_ADC1_TOTAL_SIZE);
	gStimLib_stepupFeedback_volt = stimLib_stepup_voltCalc(gStimLib_stepupFeedback_adcAvg, R1_Vstup, R2_Vstup);

	/* STEPUP VOLTAGE CONFIG and FEEDBACK */
#ifndef STIM_LIB_STEPUP_TUNING
	stimLib_voltCfg(gStimLib_stepupFeedback_volt);
#endif
}

/*
 * STEP UP VOLTAGE CONFIG
 * */
void stimLib_voltCfg(uint32_t stepup_voltage)
{
	uint32_t voltage_scaleup_val = STIM_LIB_STEPUP_TARGET_VOLTAGE * STEPUP_VOLTAGE_SCALE_SIZE;

#define VOLTAGE_DIFFERENCE_ABS		abs(voltage_scaleup_val - stepup_voltage)
#define FAST_STEPUP_VOLTAGE			6 * STEPUP_VOLTAGE_SCALE_SIZE /* 6 V(voltage) */
#define FAST_STEPUP_INC_ENABLE		VOLTAGE_DIFFERENCE_ABS > FAST_STEPUP_VOLTAGE
#define FAST_STEPUP_DEC_ENABLE		VOLTAGE_DIFFERENCE_ABS < FAST_STEPUP_VOLTAGE

#define OUTPUT_VOLTAGE_IS_LOW		abs(voltage_scaleup_val - STEPUP_FDBK_VOLT_RANGE) > stepup_voltage
#define OUTPUT_VOLTAGE_IS_HIGH		abs(voltage_scaleup_val + STEPUP_FDBK_VOLT_RANGE) < stepup_voltage
#define OUTPUT_VOLTAGE_IS_SAME		(voltage_scaleup_val - STEPUP_FDBK_VOLT_RANGE < stepup_voltage) && (voltage_scaleup_val + STEPUP_FDBK_VOLT_RANGE > stepup_voltage)

#if 0
#define OUTPUT_VOLTAGE_IS_LOW		voltage_scaleup_val > stepup_voltage
#define OUTPUT_VOLTAGE_IS_HIGH		voltage_scaleup_val < stepup_voltage
#define OUTPUT_VOLTAGE_IS_SAME		voltage_scaleup_val == stepup_voltage
#endif

#ifdef STIM_LIB_STEPUP_DATA_PRINT
	TD_DEBUG_PRINT(("----- FEEDBACK DATA -----\n"));
	TD_DEBUG_PRINT(("VOLTAGE_DIFFERENCE_ABS : %d\n", VOLTAGE_DIFFERENCE_ABS));
	TD_DEBUG_PRINT(("STEPUP_FDBK_VOLT_RANGE : %d\n", STEPUP_FDBK_VOLT_RANGE));
	TD_DEBUG_PRINT(("\r\n"));

	TD_DEBUG_PRINT(("SLOPE_CTRL_END_FLAG : %d\n", STIM_LIB_SLOPE_CTRL_END_FLAG));
	TD_DEBUG_PRINT(("OUTPUT_VOLTAGE_IS_LOW : %d\n", OUTPUT_VOLTAGE_IS_LOW));
	TD_DEBUG_PRINT(("OUTPUT_VOLTAGE_IS_HIGH : %d\n", OUTPUT_VOLTAGE_IS_HIGH));
	TD_DEBUG_PRINT(("OUTPUT_VOLTAGE_IS_SAME : %d\n", OUTPUT_VOLTAGE_IS_SAME));
	TD_DEBUG_PRINT(("\r\n"));
#endif

	if (OUTPUT_VOLTAGE_IS_SAME)
	{
		STIM_LIB_STEPUP_VOLTAGTE_STABILIZED_FLAG = true;
	}
	else
	{
		STIM_LIB_STEPUP_VOLTAGTE_STABILIZED_FLAG = false;
	}

	if (VOLTAGE_DIFFERENCE_ABS < STEPUP_FDBK_VOLT_RANGE)
	{
		if (OUTPUT_VOLTAGE_IS_LOW)
		{
			STIM_LIB_VOLTAGE_CTRL_PULSE++;

			if (STIM_LIB_VOLTAGE_CTRL_PULSE > STIM_LIB_STEPUP_CTRL_TABLE_LIMIT - 1)
			{
				STIM_LIB_VOLTAGE_CTRL_PULSE = STIM_LIB_STEPUP_CTRL_TABLE_LIMIT - 1;
			}
		}

		if (OUTPUT_VOLTAGE_IS_HIGH)
		{
			STIM_LIB_VOLTAGE_CTRL_PULSE--;

			if (STIM_LIB_VOLTAGE_CTRL_PULSE < 0)
			{
				STIM_LIB_VOLTAGE_CTRL_PULSE = 0;
			}
		}

		STIM_LIB_SLOPE_CTRL_END_FLAG = true;
		TIM1->CCR1 = STIM_LIB_VOLTAGE_CTRL_PULSE;
		return;
	}

	if (VOLTAGE_DIFFERENCE_ABS > STEPUP_FDBK_VOLT_RANGE)
	{
		if (OUTPUT_VOLTAGE_IS_LOW)
		{
			if (FAST_STEPUP_INC_ENABLE)
			{
#ifdef STIM_LIB_EVKIT_CC
				STIM_LIB_VOLTAGE_CTRL_PULSE += 1;
#endif
#ifdef STIM_LIB_EVKIT_CV
				STIM_LIB_VOLTAGE_CTRL_PULSE += 1;
#endif
			}
			else
			{
				STIM_LIB_VOLTAGE_CTRL_PULSE += 1;
			}

			if (STIM_LIB_VOLTAGE_CTRL_PULSE > STIM_LIB_STEPUP_CTRL_TABLE_LIMIT - 1)
			{
				STIM_LIB_VOLTAGE_CTRL_PULSE = STIM_LIB_STEPUP_CTRL_TABLE_LIMIT - 1;
			}
		}

		if (OUTPUT_VOLTAGE_IS_HIGH)
		{
			if (FAST_STEPUP_DEC_ENABLE)
			{
				STIM_LIB_VOLTAGE_CTRL_PULSE -= 1;
			}
			else
			{
				STIM_LIB_VOLTAGE_CTRL_PULSE -= 1;
			}

			if (STIM_LIB_VOLTAGE_CTRL_PULSE < 0)
			{
				STIM_LIB_VOLTAGE_CTRL_PULSE = 0;
			}
		}

		STIM_LIB_SLOPE_CTRL_END_FLAG = false;
		TIM1->CCR1 = STIM_LIB_VOLTAGE_CTRL_PULSE;
		return;
	}

	else if (OUTPUT_VOLTAGE_IS_SAME)
	{
		STIM_LIB_SLOPE_CTRL_END_FLAG = true;
	}
	else
	{
		STIM_LIB_SLOPE_CTRL_END_FLAG = true;
	}
}

/*
 * ADC CONVERSION DATA READ :: ADC1 DATA >> STEP UP BUFFER
 * */
void stimLib_adc1_readBuffer(uint16_t *stepup_buff, uint16_t *adc1_conv_buff, uint8_t conv_length)
{
	uint8_t i;
	for (i = 0; i < conv_length; i++)
	{
#if 1
		/*
		 * STEP UP ADC FILTER
		 * When the ADC buffer is read once,
		 * if the deviation of the value is an unexpected value, it is filtered.
		 *  */
		if (abs(adc1_conv_buff[i] - adc1_conv_buff[(i + 1) % conv_length]) < 200)
		{
			stepup_buff[i] = adc1_conv_buff[i];
		}
		else
		{
			stepup_buff[i] = adc1_conv_buff[i + 1];
		}
#else
	stepup_buff[i] = exStimLib_adc1_conv_buff[i];
#endif
	}
}

/*
 * STEPUP ADC DATA AVG
 * */
uint32_t stimLib_stepup_adcAVG(uint16_t *stepup_buff, uint8_t conv_length)
{
	uint8_t i;
	uint32_t stepup_total = 0;
	for (i = 0; i < conv_length; i++)
	{
		stepup_total += stepup_buff[i];
	}
	return stepup_total / conv_length;
}

/*
 * STEP UP VOLTAGE CALC
 * */
uint64_t stimLib_stepup_voltCalc(uint32_t stepup_adc_avg, uint32_t r1, uint32_t r2)
{
	/* REFERENCE TABLE */
	uint64_t adc_val = (STEPUP_REF_VALUE[STIM_LIB_VOLTAGE_CTRL_PULSE].fdbk_vol * stepup_adc_avg)
			/ STEPUP_REF_VALUE[STIM_LIB_VOLTAGE_CTRL_PULSE].adc_val;

	uint64_t v_out = (STEPUP_REF_VALUE[STIM_LIB_VOLTAGE_CTRL_PULSE].output_vol * adc_val) / STEPUP_REF_VALUE[STIM_LIB_VOLTAGE_CTRL_PULSE].fdbk_vol;

	return v_out;
}

/*
 * STEP UP ADC DATA PRINT
 * */
void stimLib_stepup_dataPrint(void)
{
	if (STIM_LIB_STEPUP_DATA_PRINT_FLAG == true)
	{
		uint8_t i;
		uint8_t dec_point[STEPUP_DEC_PLACES];
		uint32_t mode_val = STEPUP_VOLTAGE_SCALE_SIZE;
		uint32_t n_number = gStimLib_stepupFeedback_volt / STEPUP_VOLTAGE_SCALE_SIZE;

		STIM_LIB_VOLTAGE_CTRL_PULSE = TIM1->CCR1;

		for (i = 0; i < STEPUP_DEC_PLACES; i++)
		{
			dec_point[i] = (gStimLib_stepupFeedback_volt % mode_val) / (mode_val / 10);
			mode_val /= 10;
		}

		/* dec_point[0] = fdbk_adc_voltage % STEPUP_VOLTAGE_SCALE; */
		TD_DEBUG_PRINT(("----- STEP-UP -----\n"));
		TD_DEBUG_PRINT(("TARGET VOLTAGE : %d\n", STIM_LIB_STEPUP_TARGET_VOLTAGE));
		TD_DEBUG_PRINT(("MEAS Voltage : %ld.%d%d%d%d%d\n", n_number, dec_point[0], dec_point[1], dec_point[2], dec_point[3], dec_point[4]));
		TD_DEBUG_PRINT(("STEP-UP ADC AVG : %ld\n", gStimLib_stepupFeedback_adcAvg));
		TD_DEBUG_PRINT(("STEP-UP PW : %d\n", STIM_LIB_VOLTAGE_CTRL_PULSE));
		TD_DEBUG_PRINT(("\r\n"));
	}
	STIM_LIB_STEPUP_DATA_PRINT_FLAG = false;

}

/*
 * STEP UP START :: TIMER
 * */
void stimLib_stepup_timerStart(void)
{
	stimLib_stepup_startRaw();
}

/*
 * STEP UP STOP :: TIMER
 * */
void stimLib_stepup_timerStop(void)
{
	stimLib_stepup_stopRaw();
}

/*
 * STEP UP START :: ADC CONVERSION START
 * */
void stimLib_stepup_adcStart(void)
{
	stimLib_stepup_adcStartRaw();
}

/*
 * STEP UP STOP :: ADC CONVERSION STOP
 * */
void stimLib_stepup_adcStop(void)
{
	stimLib_stepup_adcStopRaw();
}

/* STEP UP FUNCTION Collection */
void stimLib_stepupStart(void)
{
	STIM_LIB_STEPUP_SET_START();
#ifdef STIM_LIB_EVKIT_CC
	stimLib_dacctrl_Set();
#endif
	stimLib_stepup_adcStart();
	stimLib_stepup_startRaw();
}

void stimLib_stepupStop(void)
{
	STIM_LIB_STEPUP_SET_STOP();
#ifdef STIM_LIB_EVKIT_CC
	stimLib_dacctrl_Off();
#endif
	stimLib_stepup_stopRaw();
	stimLib_stepup_adcStop();
}
