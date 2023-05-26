/*
 * stim_lib_current.c
 *
 *  Created on: 2023. 4. 28.
 *      Author: eidos
 */

#include "stim_lib_st_init.h"
#include "stim_lib_volt.h"

/*
 * TODO:
 * Add in Future
 * Need to create a DAC Control GPIO Function
 * */

void stimLib_dacctrl_setRaw(void)
{
	/* GPIO DAC CONTROL CLEAR */
	HAL_GPIO_WritePin(STIM_LIB_DAC_N0_GPIO_PORT,
			STIM_LIB_DAC_N0_PIN | STIM_LIB_DAC_N1_PIN | STIM_LIB_DAC_N2_PIN
					| STIM_LIB_DAC_N3_PIN | STIM_LIB_DAC_N4_PIN
					| STIM_LIB_DAC_N5_PIN | STIM_LIB_DAC_N6_PIN
					| STIM_LIB_DAC_N7_PIN, GPIO_PIN_RESET);

	/* GPIO DAC SETTING */
	HAL_GPIO_WritePin(STIM_LIB_DAC_N0_GPIO_PORT, STIM_LIB_STATE_SIG_DEGREE << 1,
			GPIO_PIN_SET);
}

void stimLib_dacctrl_offRaw(void)
{
	/* GPIO DAC CONTROL CLEAR */
	HAL_GPIO_WritePin(STIM_LIB_DAC_N0_GPIO_PORT,
			STIM_LIB_DAC_N0_PIN | STIM_LIB_DAC_N1_PIN | STIM_LIB_DAC_N2_PIN
					| STIM_LIB_DAC_N3_PIN | STIM_LIB_DAC_N4_PIN
					| STIM_LIB_DAC_N5_PIN | STIM_LIB_DAC_N6_PIN
					| STIM_LIB_DAC_N7_PIN, GPIO_PIN_RESET);
}
