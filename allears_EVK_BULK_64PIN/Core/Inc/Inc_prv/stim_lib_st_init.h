#ifndef	STIM_LIB_ST_H

#define	STIM_LIB_ST_H
#include <stdbool.h>

#include "stm32l412xx.h"
#include "stm32l4xx_hal.h"


typedef enum
{
	stim_lib_resrc_dma = 0x00,
	stim_lib_resrc_tim1 = 1,
	stim_lib_resrc_tim2 = 2,
	stim_lib_resrc_tim6 = 3,
	stim_lib_resrc_adc1 = 4,
	stim_lib_resrc_adc2 = 5,
	stim_lib_resrc_gpio = 6,

	stim_lib_resrc_max
} stim_lib_resrc_t;


#define STIM_LIB_RESRC_START							stim_lib_resrc_dma
#define STIM_LIB_RESOURCE_INIT_ERROR					false
#define STIM_LIB_RESOURCE_INIT_OK						true

/* GPIOA */
#define STIM_LIB_STIM_ANODE_PIN							GPIO_PIN_1
#define STIM_LIB_STIM_ANODE_GPIO_PORT					GPIOA
#define STIM_LIB_STIM_CATHODE_PIN						GPIO_PIN_2
#define STIM_LIB_STIM_CATHODE_GPIO_PORT					GPIOA
#define STIM_LIB_DAC_ON_N_PIN							GPIO_PIN_3
#define STIM_LIB_DAC_ON_N_GPIO_PORT						GPIOA
#define STIM_LIB_STEP_UP_FEEDBACK_PIN					GPIO_PIN_4
#define STIM_LIB_STEP_UP_FEEDBACK_GPIO_PORT				GPIOA
#define STIM_LIB_STIM_TRIGGER_OUTPUT_PIN				GPIO_PIN_5
#define STIM_LIB_STIM_TRIGGER_OUTPUT_GPIO_PORT			GPIOA
#define STIM_LIB_PEAK_DETECTION_R_PIN					GPIO_PIN_6
#define STIM_LIB_PEAK_DETECTION_R_GPIO_PORT				GPIOA
#define STIM_LIB_PEAK_DETECTION_L_PIN					GPIO_PIN_7
#define STIM_LIB_PEAK_DETECTION_L_GPIO_PORT				GPIOA

#define STIM_LIB_STEP_UP_SW_PIN							GPIO_PIN_8
#define STIM_LIB_STEP_UP_SW_GPIO_PORT					GPIOA
#define STIM_LIB_ST_UART_TX_PIN							GPIO_PIN_9
#define STIM_LIB_ST_UART_TX_GPIO_PORT					GPIOA
#define STIM_LIB_ST_UART_RX_PIN							GPIO_PIN_10
#define STIM_LIB_ST_UART_RX_GPIO_PORT					GPIOA
#define STIM_LIB_QCC_CTRL0_PIN							GPIO_PIN_11
#define STIM_LIB_QCC_CTRL0_GPIO_PORT					GPIOA
#define STIM_LIB_QCC_CTRL0_EXTI_IRQN					EXTI15_10_IRQn
#define STIM_LIB_QCC_CTRL1_PIN							GPIO_PIN_12
#define STIM_LIB_QCC_CTRL1_GPIO_PORT					GPIOA
#define STIM_LIB_QCC_CTRL1_EXTI_IRQN					EXTI15_10_IRQn
#define STIM_LIB_TMS_PIN								GPIO_PIN_13
#define STIM_LIB_TMS_GPIO_PORT							GPIOA
#define STIM_LIB_TCK_PIN								GPIO_PIN_14
#define STIM_LIB_TCK_GPIO_PORT							GPIOA
#define STIM_LIB_TP0_PIN								GPIO_PIN_15
#define STIM_LIB_TP0_GPIO_PORT							GPIOA

/* GPIOB */
#define STIM_LIB_STIM_TRIGGER_INPUT_PIN					GPIO_PIN_0
#define STIM_LIB_STIM_TRIGGER_INPUT_GPIO_PORT			GPIOB
#define STIM_LIB_STIM_TRIGGER_INPUT_EXTI_IRQN			EXTI0_IRQn
#define STIM_LIB_DAC_N0_PIN								GPIO_PIN_1
#define STIM_LIB_DAC_N0_GPIO_PORT						GPIOB
#define STIM_LIB_DAC_N1_PIN								GPIO_PIN_2
#define STIM_LIB_DAC_N1_GPIO_PORT						GPIOB
#define STIM_LIB_DAC_N2_PIN								GPIO_PIN_3
#define STIM_LIB_DAC_N2_GPIO_PORT						GPIOB
#define STIM_LIB_DAC_N3_PIN								GPIO_PIN_4
#define STIM_LIB_DAC_N3_GPIO_PORT						GPIOB
#define STIM_LIB_DAC_N4_PIN								GPIO_PIN_5
#define STIM_LIB_DAC_N4_GPIO_PORT						GPIOB
#define STIM_LIB_DAC_N5_PIN								GPIO_PIN_6
#define STIM_LIB_DAC_N5_GPIO_PORT						GPIOB
#define STIM_LIB_DAC_N6_PIN								GPIO_PIN_7
#define STIM_LIB_DAC_N6_GPIO_PORT						GPIOB
#define STIM_LIB_DAC_N7_PIN								GPIO_PIN_8
#define STIM_LIB_DAC_N7_GPIO_PORT						GPIOB
#define STIM_LIB_TP1_PIN								GPIO_PIN_9
#define STIM_LIB_TP1_GPIO_PORT							GPIOB

#define STIM_LIB_PEAK_DISCHG_SW_PIN						GPIO_PIN_12
#define STIM_LIB_PEAK_DISCHG_SW_GPIO_PORT				GPIOB
#define STIM_LIB_LED_PIN								GPIO_PIN_13
#define STIM_LIB_LED_GPIO_PORT							GPIOB
#define STIM_LIB_BTN_STIM_UP_PIN						GPIO_PIN_14
#define STIM_LIB_BTN_STIM_UP_GPIO_PORT					GPIOB
#define STIM_LIB_BTN_STIM_DOWN_PIN						GPIO_PIN_15
#define STIM_LIB_BTN_STIM_DOWN_GPIO_PORT				GPIOB

/* GPIOC */
#define STIM_LIB_BTN_START_PIN							GPIO_PIN_13
#define STIM_LIB_BTN_START_GPIO_PORT					GPIOC

/* GPIOH */
#define STIM_LIB_MCO_PIN 								GPIO_PIN_0
#define STIM_LIB_MCO_GPIO_PORT 							GPIOH
#define STIM_LIB_PEAK_DETECTION_PWR_SW_PIN				GPIO_PIN_3
#define STIM_LIB_PEAK_DETECTION_PWR_SW_GPIO_PORT		GPIOH

/* Timer channel */
#define STIM_LIB_PULSE_ANODE_TIM_CH									TIM_CHANNEL_2
#define STIM_LIB_PULSE_CATHODE_TIM_CH								TIM_CHANNEL_3
#define STIM_LIB_PULSE_DAC_ON_TIM_CH								TIM_CHANNEL_4
#define STIM_LIB_PULSE_TRG_OUT_TIM_CH								TIM_CHANNEL_1


/* ADC Handler */
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;

/* PWM TIMER Handler */
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;
extern DMA_HandleTypeDef hdma_tim2_ch2_ch4;
extern DMA_HandleTypeDef hdma_tim2_ch3;


/* INIT FUNCTION */
bool stimLib_gpioInit(void);
bool stimLib_dmaInit(void);

bool stimLib_tim1_Init(void); /* STEP UP PULSE */
bool stimLib_tim2_Init(void); /* STIM PULSE */
bool stimLib_tim6_Init(void); /* STEP UP FEEDBACK CONTROL TIMER */

bool stimLib_adc1_Init(void); /* STEP UP FEEDBACK ADC */
bool stimLib_adc2_Init(void); /* PEAK DETECTION ADC */

/* DEINIT FUNCTION */
void stimLib_gpioDeinit(void);
void stimLib_dmaDeinit(void);

void stimLib_tim1_Deinit(void); /* STEP UP PULSE */
void stimLib_tim2_Deinit(void); /* STIM PULSE */
void stimLib_tim6_Deinit(void); /* STEP UP FEEDBACK CONTROL TIMER */

void stimLib_adc1_Deinit(void); /* STEP UP FEEDBACK ADC */
void stimLib_adc2_Deinit(void); /* PEAK DETECTION ADC */

#endif /* STIM_LIB_ST_H */

