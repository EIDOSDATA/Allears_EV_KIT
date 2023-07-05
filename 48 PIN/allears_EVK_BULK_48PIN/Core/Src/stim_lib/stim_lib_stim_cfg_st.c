/*
 * stim_lib_stim_cfg_st.c
 *
 *  Created on: 2023. 5. 2.
 *      Author: eidos
 */
#include "stim_lib_st_msp.h"
#include "stim_lib_st_inc.h"
#include "stim_lib_stim_cfg.h"
#include "stim_lib_volt.h"

#include "td_debug.h"

#define STIM_LIB_DMA_DAC_ON_BUF										gStimLib_dacOnDma
#define STIM_LIB_DMA_DISCHG_BUF										gStimLib_dischgDma
#define STIM_LIB_DMA_CATHODE_BUF									gStimLIb_cathodeDma

/* TIM2 DMA buffer: WORD size */
volatile uint32_t gStimLib_dacOnDma[4];
volatile uint32_t gStimLib_dischgDma[2];
volatile uint32_t gStimLIb_cathodeDma[2];

bool stimLib_paramPulseSettingRaw(void)
{
	TIM_MasterConfigTypeDef sMasterConfig =
	{ 0 };
	TIM_OC_InitTypeDef sConfigOC =
	{ 0 };

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = STIM_LIB_SIGNAL_PSC_INPUT;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = STIM_LIB_SIGNAL_ARR_INPUT;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
	{
		return false;
	}
	/* MSP :: Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
	stimLib_tim_pwmmspInit(&htim2);

	if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
	{
		return false;
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC2REF;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		return false;
	}

	/* CHANNEL 1 :: TRIGGER OUTPUT */
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = STIM_LIB_TRG_OUTPUT_PULSE_TIME;

	if (STIM_LIB_STATE_TRG_OUT_ACT_POL == stim_lib_trg_output_active_low)
	{
		sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
	}
	else if (STIM_LIB_STATE_TRG_OUT_ACT_POL == stim_lib_trg_output_active_high)
	{
		sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	}

	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, STIM_LIB_PULSE_TRG_OUT_TIM_CH) != HAL_OK)
	{
		return stim_lib_stim_rsp_failed;
	}

	/* CHANNEL 2 :: ANODE PULSE */
	sConfigOC.Pulse = STIM_LIB_ANODE_PULSE_TIME;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, STIM_LIB_PULSE_ANODE_TIM_CH) != HAL_OK)
	{
		TD_DEBUG_PRINT(("STIM_LIB: TIM2 CH2 Set failed\n"));
		return false;
	}

	/* CHANNEL 3 :: CATHODE PULSE */
	sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
	sConfigOC.Pulse = STIM_LIB_CATHODE_PULSE_TIME1;
	if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, STIM_LIB_PULSE_CATHODE_TIM_CH) != HAL_OK)
	{
		TD_DEBUG_PRINT(("STIM_LIB: TIM2 CH3 Set failed\n"));
		return false;
	}

	/* CHANNEL 4 :: DAC_ON_N PULSE or STIM DISCHARGE PULSE */
#ifdef STIM_LIB_EVKIT_CC
	sConfigOC.Pulse = STIM_LIB_SIGNAL_GLICH_TIME; // STIM_LIB_DAC_CTRL_TIME3
#endif
#ifdef STIM_LIB_EVKIT_CV
	sConfigOC.Pulse = STIM_LIB_DISCHARGE_PULSE_TIME1;
#endif
	if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, STIM_LIB_PULSE_DAC_ON_TIM_CH) != HAL_OK)
	{
		return false;
	}

	/* MSP :: TIMER GPIO */
	//stimLib_tim_msppostInit(&htim2);
	HAL_TIM_GenerateEvent(&htim2, TIM_EVENTSOURCE_UPDATE);

	return true;
}

void stimLib_paramTrgSettingRaw(void)
{
	/* GPIO INIT */
	GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };

	if (STIM_LIB_TRG_INPUT_IS_ENABLED() == false)
		return;

	/* Configure GPIO pin : STIM_TRIGGER_INPUT_Pin */
	if (STIM_LIB_TRG_INPUT_IS_FALLING_EDGE())
	{
		GPIO_InitStruct.Pin = STIM_LIB_STIM_TRIGGER_INPUT_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(STIM_LIB_STIM_TRIGGER_INPUT_GPIO_PORT, &GPIO_InitStruct);
	}
	else if (STIM_LIB_TRG_INPUT_IS_RISING_EDGE())
	{
		GPIO_InitStruct.Pin = STIM_LIB_STIM_TRIGGER_INPUT_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(STIM_LIB_STIM_TRIGGER_INPUT_GPIO_PORT, &GPIO_InitStruct);
	}
	else if (STIM_LIB_TRG_INPUT_TOGGLE_IS_ACT_LOW())
	{
		GPIO_InitStruct.Pin = STIM_LIB_STIM_TRIGGER_INPUT_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(STIM_LIB_STIM_TRIGGER_INPUT_GPIO_PORT, &GPIO_InitStruct);
	}
	else if (STIM_LIB_TRG_INPUT_TOGGLE_IS_ACT_HIGH())
	{
		GPIO_InitStruct.Pin = STIM_LIB_STIM_TRIGGER_INPUT_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(STIM_LIB_STIM_TRIGGER_INPUT_GPIO_PORT, &GPIO_InitStruct);
	}

	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

void stimLib_paramTrgResettingRaw(void)
{
	HAL_NVIC_DisableIRQ(EXTI0_IRQn);
}

bool stimLib_pulseConfigRaw(void)
{
	TIM2->CNT = 0;
	TIM2->CCR1 = STIM_LIB_TRG_OUTPUT_PULSE_TIME;
	TIM2->CCR2 = STIM_LIB_ANODE_PULSE_TIME;

	STIM_LIB_DMA_CATHODE_BUF[0] = STIM_LIB_CATHODE_PULSE_TIME0;
	STIM_LIB_DMA_CATHODE_BUF[1] = STIM_LIB_CATHODE_PULSE_TIME1;

	/* DAC_ON_N Control Time */
#ifdef STIM_LIB_EVKIT_CC
	if (STIM_LIB_STATE_TRG_IN_ENABLE == false)
	{
		for (int i = 0; i < 0xffff; i++)
			;
	}
	STIM_LIB_DMA_DAC_ON_BUF[0] = STIM_LIB_DAC_CTRL_TIME0;
	STIM_LIB_DMA_DAC_ON_BUF[1] = STIM_LIB_DAC_CTRL_TIME1;
	STIM_LIB_DMA_DAC_ON_BUF[2] = STIM_LIB_DAC_CTRL_TIME2;
	STIM_LIB_DMA_DAC_ON_BUF[3] = STIM_LIB_DAC_CTRL_TIME3;
#endif

	/* DISCHARGE PULSE Control Time */
#ifdef STIM_LIB_EVKIT_CV
	if (STIM_LIB_STATE_TRG_IN_ENABLE == false)
	{
		for (int i = 0; i < 0xffff; i++);
	}
	STIM_LIB_DMA_DISCHG_BUF[0] = STIM_LIB_DISCHARGE_PULSE_TIME0;
	STIM_LIB_DMA_DISCHG_BUF[1] = STIM_LIB_DISCHARGE_PULSE_TIME1;
#endif

	return stim_lib_stim_rsp_ok;
}

bool stimLib_stimStartRaw(void)
{
	/* START PULSE TIMER CH4 :: DAC_ON_N or STIM_DISCHARGE :: GPIOA PIN 3 */
#ifdef STIM_LIB_EVKIT_CC
	HAL_TIM_OC_Start_DMA(&htim2, STIM_LIB_PULSE_DAC_ON_TIM_CH, (const uint32_t*) STIM_LIB_DMA_DAC_ON_BUF, 4);
	__HAL_DMA_DISABLE_IT(&hdma_tim2_ch2_ch4, (DMA_IT_TC | DMA_IT_HT));
#endif
#ifdef STIM_LIB_EVKIT_CV
	HAL_TIM_OC_Start_DMA(&htim2, STIM_LIB_PULSE_DAC_ON_TIM_CH, (const uint32_t*) STIM_LIB_DMA_DISCHG_BUF, 2);
	__HAL_DMA_DISABLE_IT(&hdma_tim2_ch2_ch4, (DMA_IT_TC | DMA_IT_HT));
#endif

	/* START PULSE TIMER CH2 :: ANODE :: GPIOA PIN 1 */
	HAL_TIM_PWM_Start(&htim2, STIM_LIB_PULSE_ANODE_TIM_CH);

	/* START PULSE TIMER CH3 :: CATHODE :: GPIOA PIN 2 */
	HAL_TIM_OC_Start_DMA(&htim2, STIM_LIB_PULSE_CATHODE_TIM_CH, (const uint32_t*) STIM_LIB_DMA_CATHODE_BUF, 2);
	__HAL_DMA_DISABLE_IT(&hdma_tim2_ch3, (DMA_IT_TC | DMA_IT_HT));

	/* START PULSE TIMER CH1 :: TRIGGER OUTPUT :: GPIOA PIN 5 */
	if (STIM_LIB_TRG_OUTPUT_IS_ENABLED() == true)
	{
		HAL_TIM_PWM_Start(&htim2, STIM_LIB_PULSE_TRG_OUT_TIM_CH);
		__HAL_TIM_DISABLE_IT(&htim2, TIM_IT_CC1);
	}

	return stim_lib_stim_rsp_ok;
}

bool stimLib_stimStopRaw(void)
{
	/*
	 * Normalize OCREFs in PWM mode
	 * OCREF cannot be changed in stop state
	 * Check stimLib_trgPauseRaw() Function
	 * */

	/*
	 * TODO:
	 * STIM ENDPOINT TEST
	 * */
#if 1
	if (STIM_LIB_STATE_TRG_IN_ENABLE == true)
	{
		TIM2->CCMR1 &= ~(TIM_CCMR1_OC1M | TIM_CCMR1_OC2M);
		TIM2->CCMR1 |= (TIM_OCMODE_PWM1) | (TIM_OCMODE_PWM1 << 8U);
	}
#endif
#ifdef STIM_LIB_EVKIT_CV
	HAL_TIM_PWM_Stop(&htim2, STIM_LIB_PULSE_TRG_OUT_TIM_CH);
	HAL_TIM_PWM_Stop(&htim2, STIM_LIB_PULSE_ANODE_TIM_CH);

	HAL_TIM_OC_Stop_DMA(&htim2, STIM_LIB_PULSE_CATHODE_TIM_CH);
	HAL_TIM_OC_Stop_DMA(&htim2, STIM_LIB_PULSE_DAC_ON_TIM_CH);
#endif

#ifdef STIM_LIB_EVKIT_CC
	HAL_TIM_PWM_Stop(&htim2, STIM_LIB_PULSE_TRG_OUT_TIM_CH);
	HAL_TIM_PWM_Stop(&htim2, STIM_LIB_PULSE_ANODE_TIM_CH);

	HAL_TIM_OC_Stop_DMA(&htim2, STIM_LIB_PULSE_DAC_ON_TIM_CH);
	HAL_TIM_OC_Stop_DMA(&htim2, STIM_LIB_PULSE_CATHODE_TIM_CH);
#endif

	TIM2->CNT = 0;

	return stim_lib_stim_rsp_ok;
}
