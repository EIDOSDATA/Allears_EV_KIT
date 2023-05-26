/*
 * stim_lib_volt.c
 *
 *  Created on: Apr 27, 2023
 *      Author: eidos
 */

#include <stdlib.h>

#include "stim_lib_volt.h"
#include "stim_lib_st_init.h"
#include "stim_lib_private.h"
#include "stim_lib_stim_cfg.h"

#include "td_debug.h"

/* STEPUP VOLTAGE PARAMETER */
#define R1_Vstup									3600 /* 3.6M Ohm */
#define R2_Vstup									110 /* 110K Ohm */
#define STEPUP_VOLTAGE_SCALE_SIZE					100000
#define STEPUP_DEC_PLACES							5
#define STEPUP_FDBK_VOLT_RANGE						1.8 * STEPUP_VOLTAGE_SCALE_SIZE

/* STEPUP REFERENCE VALUE :: EXTERN */
#define STEPUP_REF_VALUE							exStimLib_stepup_table
extern stim_lib_stepup_ref_value_t exStimLib_stepup_table[STIM_LIB_STEPUP_TABLE_SIZE];

/* STEPUP CONTROL VALUE :: PULSE and Counter */
uint8_t voltage_ctrl_pulse = 0;
uint8_t stepup_feedback_cnt = 0;

/* CONTROL FLAG */
static bool SLOPE_CTRL_END_FLAG = false;
static bool STEPUP_DATA_PRINT_FLAG = false;
bool stepup_status = false;

/* STEPUP FEEDBACK :: ADC1 BUFFER and CLAC Value */
uint16_t adc1_conv_buff[STIM_LIB_ADC1_TOTAL_SIZE]; /* ADC1 */
uint16_t setpup_buff[STIM_LIB_ADC1_TOTAL_SIZE]; /* ADC1 */
uint32_t stepup_fdbk_adc_avg;
uint32_t stepup_fdbk_volt;

/*
 * STEPUP CONTROL SCHEDULER
 * Calling from "HAL_TIM_PeriodElapsedCallback()" and "stimLib_stepup_ctrlCallback()" Function
 * Check out the "stim_lib_st_inc.c" file.
 * */
void stimLib_stepup_ctrlScheduler(void)
{
	STIM_LIB_STEPUP_FEEDBACK_CNT++;
	stimLib_adc1_readBuffer(setpup_buff, adc1_conv_buff,
	STIM_LIB_ADC1_TOTAL_SIZE);
	/* SLOPE VOLTAGE RISE CONTROL */
	if (STIM_LIB_STEPUP_FEEDBACK_CNT == 10 && SLOPE_CTRL_END_FLAG == false)
	{
		stimLib_stepup_voltFeedback();
		STIM_LIB_STEPUP_FEEDBACK_CNT = 0;
		STEPUP_DATA_PRINT_FLAG = true;
	}
	/* VOLTAGE RANGE KEEPING CONTROL */
	else if (STIM_LIB_STEPUP_FEEDBACK_CNT == 1 && SLOPE_CTRL_END_FLAG == true)
	{
		stimLib_stepup_voltFeedback();
		STIM_LIB_STEPUP_FEEDBACK_CNT = 0;
		STEPUP_DATA_PRINT_FLAG = true;
	}
	stimLib_stepup_dataPrint();
}

/*
 * STEP UP VOLTAGE FEEDBACK
 * */
void stimLib_stepup_voltFeedback(void)
{
	/* STEPUP DATA CLAC */
	stepup_fdbk_adc_avg = stimLib_stepup_adcAVG(setpup_buff,
	STIM_LIB_ADC1_TOTAL_SIZE);
	stepup_fdbk_volt = stimLib_stepup_voltCalc(stepup_fdbk_adc_avg, R1_Vstup,
	R2_Vstup);

	/* STEPUP VOLTAGE CONFIG */
	stimLib_voltCfg(stepup_fdbk_volt);
}

/*
 * STEP UP VOLTAGE CONFIG
 * */
void stimLib_voltCfg(uint64_t stepup_voltage)
{
	uint32_t voltage_scaleup_val = STEPUP_TARGET_VOLTAGE
			* STEPUP_VOLTAGE_SCALE_SIZE;

#define VOLTAGE_DIFFERENCE_ABS		abs(voltage_scaleup_val - stepup_voltage)
#define FAST_STEPUP_VOLTAGE			6 * STEPUP_VOLTAGE_SCALE_SIZE /* 6 V(voltage) */
#define FAST_STEPUP_ENABLE			VOLTAGE_DIFFERENCE_ABS > FAST_STEPUP_VOLTAGE

#define OUTPUT_VOLTAGE_IS_LOW		voltage_scaleup_val > stepup_voltage
#define OUTPUT_VOLTAGE_IS_HIGH		voltage_scaleup_val < stepup_voltage
#define OUTPUT_VOLTAGE_IS_SAME		voltage_scaleup_val == stepup_voltage

	if (VOLTAGE_DIFFERENCE_ABS < STEPUP_FDBK_VOLT_RANGE)
	{
		if (OUTPUT_VOLTAGE_IS_LOW)
		{
			STIM_LIB_VOLTAGE_CTRL_PULSE++;
			if (STIM_LIB_VOLTAGE_CTRL_PULSE
					> STIM_LIB_STEPUP_CTRL_TABLE_LIMIT - 1)
			{
				STIM_LIB_VOLTAGE_CTRL_PULSE = STIM_LIB_STEPUP_CTRL_TABLE_LIMIT
						- 1;
			}
		}

		else if (OUTPUT_VOLTAGE_IS_HIGH)
		{
			STIM_LIB_VOLTAGE_CTRL_PULSE--;
		}

		SLOPE_CTRL_END_FLAG = true;
		TIM1->CCR1 = STIM_LIB_VOLTAGE_CTRL_PULSE;
	}

	else if (VOLTAGE_DIFFERENCE_ABS > STEPUP_FDBK_VOLT_RANGE)
	{
		if (OUTPUT_VOLTAGE_IS_LOW)
		{
			/*
			 * FAST STEPUP
			 * When you set the voltage, raise the voltage here.
			 */
			if (FAST_STEPUP_ENABLE)
			{
				STIM_LIB_VOLTAGE_CTRL_PULSE += 6;
			}
			else
			{
				STIM_LIB_VOLTAGE_CTRL_PULSE += 1;
			}

			SLOPE_CTRL_END_FLAG = false;
			if (STIM_LIB_VOLTAGE_CTRL_PULSE
					> STIM_LIB_STEPUP_CTRL_TABLE_LIMIT - 1)
			{
				STIM_LIB_VOLTAGE_CTRL_PULSE = STIM_LIB_STEPUP_CTRL_TABLE_LIMIT
						- 1;
			}
		}
		else if (OUTPUT_VOLTAGE_IS_HIGH)
		{
			STIM_LIB_VOLTAGE_CTRL_PULSE--;
			SLOPE_CTRL_END_FLAG = true;
			if (STIM_LIB_VOLTAGE_CTRL_PULSE <= 0)
			{
				STIM_LIB_VOLTAGE_CTRL_PULSE = 0;
			}
		}
		TIM1->CCR1 = STIM_LIB_VOLTAGE_CTRL_PULSE;

	}

	else if (OUTPUT_VOLTAGE_IS_SAME)
	{
		SLOPE_CTRL_END_FLAG = true;
	}

	else
	{
		SLOPE_CTRL_END_FLAG = true;
	}

}

/*
 * ADC CONVERSION DATA READ :: ADC1 DATA >> STEP UP BUFFER
 * */
void stimLib_adc1_readBuffer(uint16_t *stepup_buff, uint16_t *adc1_conv_buff,
		uint8_t conv_length)
{
	uint8_t i;
	for (i = 0; i < conv_length; i++)
	{
		/*
		 * STEP UP ADC FILTER
		 * When the ADC buffer is read once,
		 * if the deviation of the value is an unexpected value, it is filtered.
		 *  */
		if (abs(adc1_conv_buff[i] - adc1_conv_buff[(i + 1) % conv_length])
				< 200)
		{
			stepup_buff[i] = adc1_conv_buff[i];
		}
		else
		{
			stepup_buff[i] = adc1_conv_buff[i + 1];
		}
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
uint64_t stimLib_stepup_voltCalc(uint32_t stepup_adc_avg, uint32_t r1,
		uint32_t r2)
{
	/*
	 * TODO:
	 * ADD IN FUATURE
	 * ADC TABLE
	 * */
	/* REFERENCE TABLE */
	uint64_t adc_val = (STEPUP_REF_VALUE[STIM_LIB_VOLTAGE_CTRL_PULSE].fdbk_vol
			* stepup_adc_avg)
			/ STEPUP_REF_VALUE[STIM_LIB_VOLTAGE_CTRL_PULSE].adc_val;

	uint64_t v_out = (STEPUP_REF_VALUE[STIM_LIB_VOLTAGE_CTRL_PULSE].output_vol
			* adc_val) / STEPUP_REF_VALUE[STIM_LIB_VOLTAGE_CTRL_PULSE].fdbk_vol;

	return v_out;
}

/*
 * STEP UP ADC DATA PRINT
 * */
void stimLib_stepup_dataPrint(void)
{
	if (STEPUP_DATA_PRINT_FLAG == true)
	{
		uint8_t i;
		uint8_t dec_point[STEPUP_DEC_PLACES];
		int mode_val = STEPUP_VOLTAGE_SCALE_SIZE;
		uint32_t n_number = (int) (stepup_fdbk_volt / STEPUP_VOLTAGE_SCALE_SIZE);

		for (i = 0; i < STEPUP_DEC_PLACES; i++)
		{
			dec_point[i] = (stepup_fdbk_volt % mode_val) / (mode_val / 10);
			mode_val /= 10;
		}

		/* dec_point[0] = fdbk_adc_voltage % STEPUP_VOLTAGE_SCALE; */
		TD_DEBUG_PRINT(("----- STEP-UP -----\n"));
		TD_DEBUG_PRINT(("TARGET VOLTAGE : %d\n", STEPUP_TARGET_VOLTAGE));

		TD_DEBUG_PRINT(
				("MEAS Voltage : %ld.%d%d%d%d%d\n", n_number, dec_point[0], dec_point[1], dec_point[2], dec_point[3], dec_point[4]));
		TD_DEBUG_PRINT(("STEP-UP ADC AVG : %ld\n", stepup_fdbk_adc_avg));
		TD_DEBUG_PRINT(("STEP-UP PW : %d\n", STIM_LIB_VOLTAGE_CTRL_PULSE));

#ifdef STIM_LIB_EVKIT_CC
		TD_DEBUG_PRINT(("DAC CTRL VALUE : %d\n\n", DAC_CONTROL_VALUE));
#endif

	}
	STEPUP_DATA_PRINT_FLAG = false;

}

/*
 * DAC GPIO CONTROL
 * */
void stimLib_dacctrl_Set(void)
{
	stimLib_dacctrl_setRaw();
}

void stimLib_dacctrl_Off(void)
{
	stimLib_dacctrl_offRaw();
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
