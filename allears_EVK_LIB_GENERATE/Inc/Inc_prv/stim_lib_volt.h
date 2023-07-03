/*
 * stim_lib_volt.h
 *
 *  Created on: Apr 27, 2023
 *      Author: eidos
 */

#ifndef SRC_STIM_LIB_STIM_LIB_VOLT_H_
#define SRC_STIM_LIB_STIM_LIB_VOLT_H_

#include "stm32l4xx_hal.h"
#include "stim_lib_type.h"
#include "stim_lib_state.h"

typedef struct
{
	int output_vol;
	int adc_val;
	int fdbk_vol;
} stim_lib_stepup_ref_value_t;

#ifdef STIM_LIB_EVKIT_CC
#define STEPUP_TARGET_VOLTAGE						40
#define DAC_CONTROL_VALUE							STIM_LIB_STATE_SIG_DEGREE
#endif
#ifdef STIM_LIB_EVKIT_CV
#define STEPUP_TARGET_VOLTAGE						STIM_LIB_STATE_SIG_DEGREE
#define DAC_CONTROL_VALUE							0
#endif

#ifdef STIM_LIB_EVKIT_CC
#define STIM_LIB_STEPUP_MAXIMUM_VOLTAGE				40
#endif
#ifdef STIM_LIB_EVKIT_CV
#define STIM_LIB_STEPUP_MAXIMUM_VOLTAGE				45
#endif

#define STIM_LIB_STEPUP_TABLE_SIZE					30
#define STIM_LIB_STEPUP_CTRL_TABLE_LIMIT			STIM_LIB_STEPUP_TABLE_SIZE - 0

/* ADC1 CONVERSION SIZE */
#define STIM_LIB_ADC1_CHECK_CHANNEL					1
#define STIM_LIB_ADC1_CONV_SIZE						10
#define STIM_LIB_ADC1_TOTAL_SIZE					STIM_LIB_ADC1_CONV_SIZE * STIM_LIB_ADC1_CHECK_CHANNEL

/* STEPUP CONTROL VALUE :: PULSE and Counter */
#define STIM_LIB_VOLTAGE_CTRL_PULSE					voltage_ctrl_pulse
#define STIM_LIB_STEPUP_FEEDBACK_CNT				stepup_feedback_cnt

/* STEPUP STATUS CHECK :: FLAG */
#define STIM_LIB_STEPUP_IS_STARTED()				(stepup_status == true)
#define STIM_LIB_STEPUP_SET_START()					stepup_status = true
#define STIM_LIB_STEPUP_SET_STOP()					stepup_status = false
extern bool stepup_status;

/* STEPUP FEEDBACK */
void stimLib_stepup_ctrlScheduler(void);
void stimLib_stepup_voltFeedback(void);

/* STEPUP VOLTAGE CONTROL AND CONTROL :: RAW */
void stimLib_voltCfg(uint32_t stepup_voltage);

/* DATA READ AND CLAC */
void stimLib_adc1_readBuffer(uint16_t *stepup_buff, uint16_t *adc1_conv_buff, uint8_t conv_length);
uint32_t stimLib_stepup_adcAVG(uint16_t *stepup_buff, uint8_t conv_length);
uint64_t stimLib_stepup_voltCalc(uint32_t stepup_adc_avg, uint32_t r1, uint32_t r2);

/* STEPUP DATA PRINT */
void stimLib_stepup_dataPrint(void);

/* DAC GPIO CONTROL */
void stimLib_dacctrl_Set(void);
void stimLib_dacctrl_Off(void);

/* STEPUP START AND STOP */
void stimLib_stepup_timerStart(void);
void stimLib_stepup_timerStop(void);
void stimLib_stepup_adcStart(void);
void stimLib_stepup_adcStop(void);

void stimLib_stepupStart(void);
void stimLib_stepupStop(void);

/*
 * stim_lib_volt_st.c
 * */
void stimLib_stepup_startRaw(void);
void stimLib_stepup_stopRaw(void);

void stimLib_stepup_adcStartRaw(void);
void stimLib_stepup_adcStopRaw(void);

/*
 * stim_lib_current_st.c
 * */
void stimLib_dacctrl_setRaw(void);
void stimLib_dacctrl_offRaw(void);

#endif /* SRC_STIM_LIB_STIM_LIB_VOLT_H_ */
