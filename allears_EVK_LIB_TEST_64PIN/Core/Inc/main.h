/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_USART3_UART_Init(void);
void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM6_Init(void);
void MX_ADC1_Init(void);
void MX_ADC2_Init(void);
void MX_USART1_UART_Init(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BTN_START_Pin GPIO_PIN_13
#define BTN_START_GPIO_Port GPIOC
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define STIM_ANODE_Pin GPIO_PIN_1
#define STIM_ANODE_GPIO_Port GPIOA
#define STIM_CATHODE_Pin GPIO_PIN_2
#define STIM_CATHODE_GPIO_Port GPIOA
#define DAC_ON_N_Pin GPIO_PIN_3
#define DAC_ON_N_GPIO_Port GPIOA
#define STEP_UP_FEEDBACK_Pin GPIO_PIN_4
#define STEP_UP_FEEDBACK_GPIO_Port GPIOA
#define STIM_TRIGGER_OUTPUT_Pin GPIO_PIN_5
#define STIM_TRIGGER_OUTPUT_GPIO_Port GPIOA
#define PEAK_DETECTION_R_Pin GPIO_PIN_6
#define PEAK_DETECTION_R_GPIO_Port GPIOA
#define PEAK_DETECTION_L_Pin GPIO_PIN_7
#define PEAK_DETECTION_L_GPIO_Port GPIOA
#define STIM_TRIGGER_INPUT_Pin GPIO_PIN_0
#define STIM_TRIGGER_INPUT_GPIO_Port GPIOB
#define STIM_TRIGGER_INPUT_EXTI_IRQn EXTI0_IRQn
#define DAC_N0_Pin GPIO_PIN_1
#define DAC_N0_GPIO_Port GPIOB
#define DAC_N1_Pin GPIO_PIN_2
#define DAC_N1_GPIO_Port GPIOB
#define PEAK_DISCHG_SW_Pin GPIO_PIN_12
#define PEAK_DISCHG_SW_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOB
#define BTN_STIM_UP_Pin GPIO_PIN_14
#define BTN_STIM_UP_GPIO_Port GPIOB
#define BTN_STIM_DOWN_Pin GPIO_PIN_15
#define BTN_STIM_DOWN_GPIO_Port GPIOB
#define STEP_UP_SW_Pin GPIO_PIN_8
#define STEP_UP_SW_GPIO_Port GPIOA
#define ST_UART_TX_Pin GPIO_PIN_9
#define ST_UART_TX_GPIO_Port GPIOA
#define ST_UART_RX_Pin GPIO_PIN_10
#define ST_UART_RX_GPIO_Port GPIOA
#define QCC_CTRL0_Pin GPIO_PIN_11
#define QCC_CTRL0_GPIO_Port GPIOA
#define QCC_CTRL0_EXTI_IRQn EXTI15_10_IRQn
#define QCC_CTRL1_Pin GPIO_PIN_12
#define QCC_CTRL1_GPIO_Port GPIOA
#define QCC_CTRL1_EXTI_IRQn EXTI15_10_IRQn
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define TP0_Pin GPIO_PIN_15
#define TP0_GPIO_Port GPIOA
#define DAC_N2_Pin GPIO_PIN_3
#define DAC_N2_GPIO_Port GPIOB
#define DAC_N3_Pin GPIO_PIN_4
#define DAC_N3_GPIO_Port GPIOB
#define DAC_N4_Pin GPIO_PIN_5
#define DAC_N4_GPIO_Port GPIOB
#define DAC_N5_Pin GPIO_PIN_6
#define DAC_N5_GPIO_Port GPIOB
#define DAC_N6_Pin GPIO_PIN_7
#define DAC_N6_GPIO_Port GPIOB
#define PEAK_DETECTION_PWR_SW_Pin GPIO_PIN_3
#define PEAK_DETECTION_PWR_SW_GPIO_Port GPIOH
#define DAC_N7_Pin GPIO_PIN_8
#define DAC_N7_GPIO_Port GPIOB
#define TP1_Pin GPIO_PIN_9
#define TP1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
UART_HandleTypeDef* tdUsart1_handlerGet(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
