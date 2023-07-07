/*
 * stim_lib_current.c
 *
 *  Created on: 2023. 4. 28.
 *      Author: eidos
 */

#include "stim_lib_st_init.h"
#include "stim_lib_volt.h"

#include "td_debug.h"

void stimLib_dacctrl_setRaw(void)
{
	/* GPIO DAC CONTROL CLEAR */
	HAL_GPIO_WritePin(STIM_LIB_DAC_N0_GPIO_PORT,
			STIM_LIB_DAC_N0_PIN | STIM_LIB_DAC_N1_PIN | STIM_LIB_DAC_N2_PIN | STIM_LIB_DAC_N3_PIN | STIM_LIB_DAC_N4_PIN | STIM_LIB_DAC_N5_PIN
					| STIM_LIB_DAC_N6_PIN | STIM_LIB_DAC_N7_PIN, GPIO_PIN_SET);

	/* GPIO DAC SETTING */

	TD_DEBUG_PRINT(("GPIOB ODR : %ld\n", GPIOB->ODR));

	HAL_GPIO_TogglePin(STIM_LIB_DAC_N0_GPIO_PORT, ((uint16_t) DAC_CONTROL_VALUE) << 1);

	TD_DEBUG_PRINT(("GPIOB ODR : %ld\n", GPIOB->ODR));

	/*
	 0x01fe;

	 0000 0001 1111 1110

	 GPIOB->ODR = 0xffff;

	 HAL_GPIO_WritePin(STIM_LIB_DAC_N0_GPIO_PORT, ((uint16_t) DAC_CONTROL_VALUE) << 1, GPIO_PIN_RESET);
	 */
}

void stimLib_dacctrl_offRaw(void)
{
	/* GPIO DAC CONTROL CLEAR */
	HAL_GPIO_WritePin(STIM_LIB_DAC_N0_GPIO_PORT,
			STIM_LIB_DAC_N0_PIN | STIM_LIB_DAC_N1_PIN | STIM_LIB_DAC_N2_PIN | STIM_LIB_DAC_N3_PIN | STIM_LIB_DAC_N4_PIN | STIM_LIB_DAC_N5_PIN
					| STIM_LIB_DAC_N6_PIN | STIM_LIB_DAC_N7_PIN, GPIO_PIN_RESET);
}
