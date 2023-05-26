/*
 * stim_lib_volt_st.c
 *
 *  Created on: May 3, 2023
 *      Author: eidos
 */

#include "stim_lib_st_init.h"
#include "stim_lib_volt.h"
#include "stim_lib_state.h"

/* STEPUP FEEDBACK :: ADC1 BUFFER and CLAC Value */
extern uint16_t adc1_conv_buff[STIM_LIB_ADC1_TOTAL_SIZE]; // ADC1

/* STEPUP CONTROL VALUE :: PULSE and Counter */
extern uint8_t voltage_ctrl_pulse;
extern uint8_t stepup_feedback_cnt;

void stimLib_stepup_startRaw(void)
{
	TIM6->CNT = 0;
	TIM6->PSC = STIM_LIB_STEPUP_PSC_INPUT;
	TIM6->ARR = STIM_LIB_STEPUP_ARR_INPUT;

	/* STEP UP PULSE */
	TIM1->CCR1 = STIM_LIB_VOLTAGE_CTRL_PULSE;
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	/* STEP UP FEEDBACK */
	HAL_TIM_Base_Start_IT(&htim6);
}

void stimLib_stepup_stopRaw(void)
{
	STIM_LIB_VOLTAGE_CTRL_PULSE = 0;
	HAL_TIM_Base_Stop(&htim1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIM_Base_Stop_IT(&htim6);
}

void stimLib_stepup_adcStartRaw(void)
{
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc1_conv_buff,
			STIM_LIB_ADC1_TOTAL_SIZE);
	__HAL_DMA_DISABLE_IT(&hdma_adc1, DMA_IT_HT | DMA_IT_TC);
	__HAL_ADC_DISABLE_IT(&hadc1, ADC_IT_OVR);
}

void stimLib_stepup_adcStopRaw(void)
{
	HAL_ADC_Stop_DMA(&hadc1);
}
