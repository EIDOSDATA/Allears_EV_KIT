/*
 * stim_lib_st_msp.h
 *
 *  Created on: 2023. 4. 25.
 *      Author: eidos
 */

#ifndef SRC_STIM_LIB_STIM_LIB_ST_MSP_H_
#define SRC_STIM_LIB_STIM_LIB_ST_MSP_H_

#include "stim_lib_st_init.h"

void stimLib_adc_mspInit(ADC_HandleTypeDef *hadc);
void stimLib_tim_pwmmspInit(TIM_HandleTypeDef *htim_pwm);
void stimLib_tim_basemspInit(TIM_HandleTypeDef *htim_base);
void stimLib_tim_msppostInit(TIM_HandleTypeDef *htim);

#endif /* SRC_STIM_LIB_STIM_LIB_ST_MSP_H_ */
