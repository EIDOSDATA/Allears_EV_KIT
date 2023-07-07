/*
 * stim_lib_current.c
 *
 *  Created on: 2023. 4. 28.
 *      Author: eidos
 */

#include "stim_lib_st_init.h"
#include "stim_lib_volt.h"

#include "td_debug.h"

#define STIM_LIB_DAC_ODR			((uint16_t)DAC_CONTROL_VALUE << 1)
#define STIM_LIB_DAC_MASK			0x01FE
#define STIM_LIB_DAC_BSRR			GPIOB->BSRR

void stimLib_dacctrl_setRaw(void)
{
#if 0
	/* 0x01fe; >>  0000 0001 1111 1110 */
	HAL_GPIO_WritePin(STIM_LIB_DAC_N0_GPIO_PORT,
			STIM_LIB_DAC_N0_PIN | STIM_LIB_DAC_N1_PIN | STIM_LIB_DAC_N2_PIN | STIM_LIB_DAC_N3_PIN | STIM_LIB_DAC_N4_PIN | STIM_LIB_DAC_N5_PIN
					| STIM_LIB_DAC_N6_PIN | STIM_LIB_DAC_N7_PIN, GPIO_PIN_SET);

	HAL_GPIO_WritePin(STIM_LIB_DAC_N0_GPIO_PORT, ((uint16_t) DAC_CONTROL_VALUE) << 1, GPIO_PIN_RESET);
#endif

	/* GPIO DAC SETTING */
	TD_DEBUG_PRINT(("\r\n"));
	TD_DEBUG_PRINT(("BEFORE : %lx\n", GPIOB->ODR));

	STIM_LIB_DAC_BSRR = ((STIM_LIB_DAC_ODR & STIM_LIB_DAC_MASK) << 16) | (~STIM_LIB_DAC_ODR & STIM_LIB_DAC_MASK);

	TD_DEBUG_PRINT(("AFTER : %lx\n", GPIOB->ODR));
	TD_DEBUG_PRINT(("\r\n"));
}

void stimLib_dacctrl_offRaw(void)
{
	/* GPIO DAC CONTROL CLEAR */
	HAL_GPIO_WritePin(STIM_LIB_DAC_N0_GPIO_PORT,
			STIM_LIB_DAC_N0_PIN | STIM_LIB_DAC_N1_PIN | STIM_LIB_DAC_N2_PIN | STIM_LIB_DAC_N3_PIN | STIM_LIB_DAC_N4_PIN | STIM_LIB_DAC_N5_PIN
					| STIM_LIB_DAC_N6_PIN | STIM_LIB_DAC_N7_PIN, GPIO_PIN_SET);
}
