/*
 * stim_lib_st_inc.h
 *
 *  Created on: 2023. 4. 25.
 *      Author: eidos
 */

#ifndef SRC_STIM_LIB_STIM_LIB_ST_INC_H_
#define SRC_STIM_LIB_STIM_LIB_ST_INC_H_

#include "stm32l4xx_hal.h"
#include "stim_lib_type.h"

/*
 * STIM LIB :: Library required definition elements
 *
 * ADC_HandleTypeDef hadc1;
 * ADC_HandleTypeDef hadc2;
 * DMA_HandleTypeDef hdma_adc1;
 * DMA_HandleTypeDef hdma_adc2;
 *
 * TIM_HandleTypeDef htim1;
 * TIM_HandleTypeDef htim2;
 * TIM_HandleTypeDef htim6;
 * DMA_HandleTypeDef hdma_tim2_ch2_ch4;
 * DMA_HandleTypeDef hdma_tim2_ch3;
 *
 * @ DEBUG PRINT
 * UART_HandleTypeDef huart3;
 *
 * */

/*
 * ADC Callback
 * */
void stimLib_adcWatchdoc_callback(void);
void stimLib_adcError_callback(void);
void stimLib_stepup_ctrlCallback(TIM_HandleTypeDef *htim);

/*
 * Trigger Input Callback
 * */
void stimLib_trgInput_callback(uint16_t gpio_pin);

/*
 * Handle stimulation
 */
void stimLib_timError_callback(TIM_HandleTypeDef *htim);
void stimLib_timPwmPluseFinished_callback(TIM_HandleTypeDef *htim);

/*
 * stim_lib_st_inc_st.c
 * */

#endif /* SRC_STIM_LIB_STIM_LIB_ST_INC_H_ */
