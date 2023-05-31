#include "stim_lib_st_init.h"
#include "stim_lib_st_msp.h"
#include "stim_lib_stim_cfg.h"
#include "stim_lib_state.h"

/* INIT FUNCTION */
bool stimLib_gpioInit(void)
{
	/* GPIO INIT */
	GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* Configure GPIO pin Output Level */
	/* DAC and PEAK DISCHARGE and PWR SW */
#ifdef STIM_LIB_EVKIT_CC
	HAL_GPIO_WritePin(GPIOB,
			STIM_LIB_DAC_N0_PIN | STIM_LIB_DAC_N1_PIN | STIM_LIB_DAC_N2_PIN | STIM_LIB_DAC_N3_PIN | STIM_LIB_DAC_N4_PIN | STIM_LIB_DAC_N5_PIN
					| STIM_LIB_DAC_N6_PIN | STIM_LIB_DAC_N7_PIN | STIM_LIB_PEAK_DISCHG_SW_PIN | STIM_LIB_PEAK_DETECTION_PWR_SW_PIN, GPIO_PIN_RESET);

	/* Configure GPIO pin : DAC and PEAK DISCHARGE, PWR SW */
	GPIO_InitStruct.Pin = STIM_LIB_DAC_N0_PIN | STIM_LIB_DAC_N1_PIN | STIM_LIB_DAC_N2_PIN | STIM_LIB_DAC_N3_PIN | STIM_LIB_DAC_N4_PIN
			| STIM_LIB_DAC_N5_PIN | STIM_LIB_DAC_N6_PIN | STIM_LIB_DAC_N7_PIN | STIM_LIB_PEAK_DISCHG_SW_PIN | STIM_LIB_PEAK_DETECTION_PWR_SW_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif

#ifdef STIM_LIB_EVKIT_CV
	/*Configure GPIO pins : PEAK DETECTION ADC PIN(in CC mode) : PA6 PA7 : ANALOG */
	GPIO_InitStruct.Pin = STIM_LIB_PEAK_DETECTION_R_PIN | STIM_LIB_PEAK_DETECTION_L_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Configure GPIO pin : DAC and PEAK DISCHARGE, PWR SW : ANALOG */
	GPIO_InitStruct.Pin = STIM_LIB_DAC_N0_PIN | STIM_LIB_DAC_N1_PIN | STIM_LIB_DAC_N2_PIN | STIM_LIB_DAC_N3_PIN | STIM_LIB_DAC_N4_PIN
			| STIM_LIB_DAC_N5_PIN | STIM_LIB_DAC_N6_PIN | STIM_LIB_DAC_N7_PIN | STIM_LIB_PEAK_DISCHG_SW_PIN | STIM_LIB_PEAK_DETECTION_PWR_SW_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif

	return true;
}

bool stimLib_dmaInit(void)
{
	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();

	/* PWM PULSE DMA Interrupt INIT */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	/* DMA1_Channel7_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

	/* DMA1_Channel2_IRQn interrupt configuration */
#ifdef STIM_LIB_EVKIT_CC
	HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
#endif

	return STIM_LIB_RESOURCE_INIT_OK;
}

bool stimLib_tim1_Init(void)
{
	TIM_MasterConfigTypeDef sMasterConfig =
	{ 0 };
	TIM_OC_InitTypeDef sConfigOC =
	{ 0 };
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig =
	{ 0 };

#ifdef STIM_LIB_EVKIT_CV
	/*
	 * STEP UP INPUT VOLTAGE : 5.3v
	 * TIMER HZ : 10.0 kHz
	 * Test End point Resistance : 50 kΩ
	 * */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	htim1.Init.Period = 7999;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
#endif
#ifdef STIM_LIB_EVKIT_CC
	/*
	 * STEP UP INPUT VOLTAGE : 5.3v
	 * TIMER HZ : 4.0 kHz
	 * Test End point Resistance : 50 kΩ
	 * */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	htim1.Init.Period = 19999;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
#endif
	if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
	{
		return false;
	}
	/* MSP :: Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
	stimLib_tim_pwmmspInit(&htim1);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
	{
		return false;
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		return false;
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
	{
		return false;
	}

	/* MSP :: TIMER GPIO */
	stimLib_tim_msppostInit(&htim1);

	return STIM_LIB_RESOURCE_INIT_OK;
}

bool stimLib_tim2_Init(void)
{
	/* TIMER2 SETTING */
	TIM_MasterConfigTypeDef sMasterConfig =
	{ 0 };
	TIM_OC_InitTypeDef sConfigOC =
	{ 0 };
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = STIM_LIB_SIGNAL_PSC_INPUT;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	if (STIM_LIB_STATE_SIG_FREQ == 0)
	{
		STIM_LIB_STATE_SIG_FREQ = 1;
	}
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
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
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

	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC,
	STIM_LIB_PULSE_TRG_OUT_TIM_CH) != HAL_OK)
	{
		return false;
	}

	/* CHANNEL 2 :: ANODE PULSE */
	sConfigOC.Pulse = STIM_LIB_ANODE_PULSE_TIME;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC,
	STIM_LIB_PULSE_ANODE_TIM_CH) != HAL_OK)
	{
		return false;
	}

	/* CHANNEL 3 :: CATHODE PULSE */
	sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
	sConfigOC.Pulse = STIM_LIB_CATHODE_PULSE_TIME1;
	if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC,
	STIM_LIB_PULSE_CATHODE_TIM_CH) != HAL_OK)
	{
		return false;
	}

	/* CHANNEL 4 :: DAC_ON_N PULSE or STIM DISCHARGE PULSE */
#if 1
	sConfigOC.Pulse = STIM_LIB_SIGNAL_GLICH_TIME;
#else
	sConfigOC.Pulse = STIM_LIB_DISCHARGE_PULSE_TIME1;
#endif
	if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC,
	STIM_LIB_PULSE_DAC_ON_TIM_CH) != HAL_OK)
	{
		return false;
	}

	/* MSP :: TIMER GPIO */
	stimLib_tim_msppostInit(&htim2);

	return STIM_LIB_RESOURCE_INIT_OK;
}

bool stimLib_tim6_Init(void)
{
	TIM_MasterConfigTypeDef sMasterConfig =
	{ 0 };

	htim6.Instance = TIM6;
	htim6.Init.Prescaler = STIM_LIB_STEPUP_PSC_INPUT;
	htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim6.Init.Period = STIM_LIB_STEPUP_ARR_INPUT;
	htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
	{
		return false;
	}

	/* MSP :: TIMER 6 :: HAL_TIM_BASE_INIT >> HAL_TIM_Base_MspInit */
	stimLib_tim_basemspInit(&htim6);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
	{
		return false;
	}

	return STIM_LIB_RESOURCE_INIT_OK;
}

bool stimLib_adc1_Init(void)
{
	ADC_MultiModeTypeDef multimode =
	{ 0 };
	ADC_AnalogWDGConfTypeDef AnalogWDGConfig =
	{ 0 };
	ADC_ChannelConfTypeDef sConfig =
	{ 0 };

	/** Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DMAContinuousRequests = ENABLE;
	hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	hadc1.Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		return false;
	}

	/* MSP SETTING */
	stimLib_adc_mspInit(&hadc1);
	/* Set ADC error code to none */
	ADC_CLEAR_ERRORCODE(&hadc1);
	/* Initialize Lock */
	hadc1.Lock = HAL_UNLOCKED;

	/** Configure the ADC multi-mode
	 */
	multimode.Mode = ADC_MODE_INDEPENDENT;
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
	{
		return false;
	}

	/** Configure Analog WatchDog 1
	 */
	AnalogWDGConfig.WatchdogNumber = ADC_ANALOGWATCHDOG_1;
	AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
	AnalogWDGConfig.Channel = ADC_CHANNEL_9;
	AnalogWDGConfig.ITMode = ENABLE;
#ifdef STIM_LIB_EVKIT_CV
	AnalogWDGConfig.HighThreshold = 3280; /* STEP UP LIMIT : 50V */
#endif

#ifdef STIM_LIB_EVKIT_CC
	AnalogWDGConfig.HighThreshold = 2800; /* STEP UP LIMIT : 43V */
#endif

	AnalogWDGConfig.LowThreshold = 0;
	if (HAL_ADC_AnalogWDGConfig(&hadc1, &AnalogWDGConfig) != HAL_OK)
	{
		return false;
	}

	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_9;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	/*
	 * TODO:
	 * TEST : ADC_SAMPLETIME_47CYCLES_5 or ADC_SAMPLETIME_640CYCLES_5
	 * */
	sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		return false;
	}
	return STIM_LIB_RESOURCE_INIT_OK;
}

bool stimLib_adc2_Init(void)
{
#ifdef STIM_LIB_EVKIT_CC
	ADC_ChannelConfTypeDef sConfig =
	{ 0 };

	/** Common config
	 */
	hadc2.Instance = ADC2;
	hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc2.Init.Resolution = ADC_RESOLUTION_12B;
	hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc2.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc2.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	hadc2.Init.LowPowerAutoWait = DISABLE;
	hadc2.Init.ContinuousConvMode = DISABLE;
	hadc2.Init.NbrOfConversion = 2;
	hadc2.Init.DiscontinuousConvMode = DISABLE;
	hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T2_TRGO;
	hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_FALLING;
	hadc2.Init.DMAContinuousRequests = ENABLE;
	hadc2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	hadc2.Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&hadc2) != HAL_OK)
	{
		return false;
	}
	/* MSP SETTING */
	stimLib_adc_mspInit(&hadc2);
	/* Set ADC error code to none */
	ADC_CLEAR_ERRORCODE(&hadc2);
	/* Initialize Lock */
	hadc2.Lock = HAL_UNLOCKED;

	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_11;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
	{
		return false;
	}

	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_12;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
	{
		return false;
	}
#endif
	return STIM_LIB_RESOURCE_INIT_OK;
}

/* DEINIT FUNCTION */
void stimLib_gpioDeinit(void)
{
#ifdef STIM_LIB_EVKIT_CC
	HAL_GPIO_DeInit(GPIOA,
			STIM_LIB_STEP_UP_SW_PIN | STIM_LIB_STEP_UP_FEEDBACK_PIN | STIM_LIB_STIM_ANODE_PIN | STIM_LIB_STIM_CATHODE_PIN | STIM_LIB_DAC_ON_N_PIN
					| STIM_LIB_STIM_TRIGGER_OUTPUT_PIN | STIM_LIB_PEAK_DETECTION_R_PIN | STIM_LIB_PEAK_DETECTION_L_PIN);
	HAL_GPIO_DeInit(GPIOB,
			STIM_LIB_STIM_TRIGGER_INPUT_PIN | STIM_LIB_DAC_N0_PIN | STIM_LIB_DAC_N1_PIN | STIM_LIB_DAC_N2_PIN | STIM_LIB_DAC_N3_PIN
					| STIM_LIB_DAC_N4_PIN | STIM_LIB_DAC_N5_PIN | STIM_LIB_DAC_N6_PIN | STIM_LIB_DAC_N7_PIN | STIM_LIB_PEAK_DISCHG_SW_PIN
					| STIM_LIB_PEAK_DETECTION_PWR_SW_PIN);
#endif
#ifdef STIM_LIB_EVKIT_CV
	HAL_GPIO_DeInit(GPIOA,
			STIM_LIB_STEP_UP_SW_PIN | STIM_LIB_STEP_UP_FEEDBACK_PIN | STIM_LIB_STIM_ANODE_PIN | STIM_LIB_STIM_CATHODE_PIN | STIM_LIB_DAC_ON_N_PIN
					| STIM_LIB_STIM_TRIGGER_OUTPUT_PIN);
	HAL_GPIO_DeInit(GPIOB, STIM_LIB_STIM_TRIGGER_INPUT_PIN);
#endif

}

void stimLib_dmaDeinit(void)
{
	HAL_DMA_DeInit(&hdma_adc1);
#ifdef STIM_LIB_EVKIT_CC
	HAL_DMA_DeInit(&hdma_adc2);
#endif
	HAL_DMA_DeInit(&hdma_tim2_ch2_ch4);
	HAL_DMA_DeInit(&hdma_tim2_ch3);
}

void stimLib_tim1_Deinit(void)
{
	HAL_TIM_PWM_DeInit(&htim1);
}

void stimLib_tim2_Deinit(void)
{
	HAL_TIM_PWM_DeInit(&htim2);
}

void stimLib_tim6_Deinit(void)
{
	HAL_TIM_PWM_DeInit(&htim6);
}

void stimLib_adc1_Deinit(void)
{
	HAL_ADC_DeInit(&hadc1);
}

void stimLib_adc2_Deinit(void)
{
#ifdef STIM_LIB_EVKIT_CC
	HAL_ADC_DeInit(&hadc2);
#endif
}
