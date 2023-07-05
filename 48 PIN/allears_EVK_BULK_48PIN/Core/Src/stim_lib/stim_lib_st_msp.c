/*
 * stim_lib_st_msp.c
 *
 *  Created on: 2023. 4. 25.
 *      Author: eidos
 */
#include "stim_lib_st_msp.h"

static uint32_t HAL_RCC_ADC_CLK_ENABLED = 0;

/* BASE TIMER MSP INIT :: NON APPLICATION JUST STIMLIB */
void stimLib_adc_mspInit(ADC_HandleTypeDef *hadc)
{
	GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };
	if (hadc->Instance == ADC1)
	{
		/* USER CODE BEGIN ADC1_MspInit 0 */

		/* USER CODE END ADC1_MspInit 0 */
		/* Peripheral clock enable */
		HAL_RCC_ADC_CLK_ENABLED++;
		if (HAL_RCC_ADC_CLK_ENABLED == 1)
		{
			__HAL_RCC_ADC_CLK_ENABLE();
		}

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**ADC1 GPIO Configuration
		 PA4     ------> ADC1_IN9
		 */
		GPIO_InitStruct.Pin = STIM_LIB_STEP_UP_FEEDBACK_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(STIM_LIB_STEP_UP_FEEDBACK_GPIO_PORT, &GPIO_InitStruct);

		/* ADC1 DMA Init */
		/* ADC1 Init */
		hdma_adc1.Instance = DMA2_Channel3;
		hdma_adc1.Init.Request = DMA_REQUEST_0;
		hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
		hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_adc1.Init.Mode = DMA_CIRCULAR;
		hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
		if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
		{
			//Error_Handler();
		}

		__HAL_LINKDMA(hadc, DMA_Handle, hdma_adc1);

		/* ADC1 interrupt Init */
		HAL_NVIC_SetPriority(ADC1_2_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
		/* USER CODE BEGIN ADC1_MspInit 1 */

		/* USER CODE END ADC1_MspInit 1 */
	}

#ifdef STIM_LIB_EVKIT_CC
	else if (hadc->Instance == ADC2)
	{
		/* USER CODE BEGIN ADC2_MspInit 0 */

		/* USER CODE END ADC2_MspInit 0 */
		/* Peripheral clock enable */
		HAL_RCC_ADC_CLK_ENABLED++;
		if (HAL_RCC_ADC_CLK_ENABLED == 1)
		{
			__HAL_RCC_ADC_CLK_ENABLE();
		}

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**ADC2 GPIO Configuration
		 PA6     ------> ADC2_IN11
		 PA7     ------> ADC2_IN12
		 */
		GPIO_InitStruct.Pin = STIM_LIB_PEAK_DETECTION_R_PIN
				| STIM_LIB_PEAK_DETECTION_L_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* ADC2 DMA Init */
		/* ADC2 Init */
		hdma_adc2.Instance = DMA1_Channel2;
		hdma_adc2.Init.Request = DMA_REQUEST_0;
		hdma_adc2.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc2.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc2.Init.MemInc = DMA_MINC_ENABLE;
		hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_adc2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_adc2.Init.Mode = DMA_CIRCULAR;
		hdma_adc2.Init.Priority = DMA_PRIORITY_LOW;
		if (HAL_DMA_Init(&hdma_adc2) != HAL_OK)
		{
			//Error_Handler();
		}

		__HAL_LINKDMA(hadc, DMA_Handle, hdma_adc2);

	}
#endif

}

void stimLib_tim_pwmmspInit(TIM_HandleTypeDef *htim_pwm)
{
	if (htim_pwm->Instance == TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 2, 0);
		HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
	}
	else if (htim_pwm->Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();

		/* TIM2 DMA Init */
		/* TIM2_CH2_CH4 Init */
		hdma_tim2_ch2_ch4.Instance = DMA1_Channel7;
		hdma_tim2_ch2_ch4.Init.Request = DMA_REQUEST_4;
		hdma_tim2_ch2_ch4.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_tim2_ch2_ch4.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_tim2_ch2_ch4.Init.MemInc = DMA_MINC_ENABLE;
		hdma_tim2_ch2_ch4.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		hdma_tim2_ch2_ch4.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
		hdma_tim2_ch2_ch4.Init.Mode = DMA_CIRCULAR;
		hdma_tim2_ch2_ch4.Init.Priority = DMA_PRIORITY_VERY_HIGH;
		if (HAL_DMA_Init(&hdma_tim2_ch2_ch4) != HAL_OK)
		{
			//Error_Handler();
		}

		/* Several peripheral DMA handle pointers point to the same DMA handle.
		 Be aware that there is only one channel to perform all the requested DMAs. */
		__HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_CC2], hdma_tim2_ch2_ch4);
		__HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_CC4], hdma_tim2_ch2_ch4);

		/* TIM2_CH3 Init */
		hdma_tim2_ch3.Instance = DMA1_Channel1;
		hdma_tim2_ch3.Init.Request = DMA_REQUEST_4;
		hdma_tim2_ch3.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_tim2_ch3.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_tim2_ch3.Init.MemInc = DMA_MINC_ENABLE;
		hdma_tim2_ch3.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		hdma_tim2_ch3.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
		hdma_tim2_ch3.Init.Mode = DMA_CIRCULAR;
		hdma_tim2_ch3.Init.Priority = DMA_PRIORITY_VERY_HIGH;
		if (HAL_DMA_Init(&hdma_tim2_ch3) != HAL_OK)
		{
			//Error_Handler();
		}

		__HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_CC3], hdma_tim2_ch3);

		/* TIM2 interrupt Init */
		HAL_NVIC_SetPriority(TIM2_IRQn, 2, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
		/* USER CODE BEGIN TIM2_MspInit 1 */

		/* USER CODE END TIM2_MspInit 1 */
	}

}

void stimLib_tim_basemspInit(TIM_HandleTypeDef *htim_base)
{
	if (htim_base->Instance == TIM6)
	{
		__HAL_RCC_TIM6_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM6_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(TIM6_IRQn);
	}
}

/* TIMER PWM MSP INIT :: NVIC and DMA */
/* TIMER CONTROL PIN MSP INIT */
void stimLib_tim_msppostInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct =
	{ 0 };
	if (htim->Instance == TIM1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**TIM1 GPIO Configuration
		 PA8     ------> TIM1_CH1
		 */
		GPIO_InitStruct.Pin = STIM_LIB_STEP_UP_SW_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
		HAL_GPIO_Init(STIM_LIB_STEP_UP_SW_GPIO_PORT, &GPIO_InitStruct);
	}
	else if (htim->Instance == TIM2)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**TIM2 GPIO Configuration
		 PA1     ------> TIM2_CH2
		 PA2     ------> TIM2_CH3
		 PA3     ------> TIM2_CH4
		 PA5     ------> TIM2_CH1
		 */
		GPIO_InitStruct.Pin = STIM_LIB_STIM_ANODE_PIN | STIM_LIB_STIM_CATHODE_PIN | STIM_LIB_DAC_ON_N_PIN | STIM_LIB_STIM_TRIGGER_OUTPUT_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

