/*
 * stim_lib_current.c
 *
 *  Created on: 2023. 4. 28.
 *      Author: eidos
 */

#include "stim_lib_st_init.h"
#include "stim_lib_current.h"

#include "td_debug.h"

#define STIM_LIB_DAC_ODR			((uint16_t)STIM_LIB_DAC_CTRL_VALUE << 1)
#define STIM_LIB_DAC_MASK			0x01FE
#define STIM_LIB_DAC_BSRR			GPIOB->BSRR

void stimLib_dacctrl_setRaw(void)
{
#if 0
	/* 0x01fe; >>  0000 0001 1111 1110 */
	HAL_GPIO_WritePin(STIM_LIB_DAC_N0_GPIO_PORT,
			STIM_LIB_DAC_N0_PIN | STIM_LIB_DAC_N1_PIN | STIM_LIB_DAC_N2_PIN | STIM_LIB_DAC_N3_PIN | STIM_LIB_DAC_N4_PIN | STIM_LIB_DAC_N5_PIN
					| STIM_LIB_DAC_N6_PIN | STIM_LIB_DAC_N7_PIN, GPIO_PIN_SET);

	HAL_GPIO_WritePin(STIM_LIB_DAC_N0_GPIO_PORT, ((uint16_t) STIM_LIB_DAC_TARGET_VALUE) << 1, GPIO_PIN_RESET);
#endif

	/* GPIO DAC SETTING */
	TD_DEBUG_PRINT(("\r\n"));
	TD_DEBUG_PRINT(("BEFORE : %lx\n", GPIOB->ODR));

	/*
	 * Responsible for GPIO Set and Reset.
	 * Because the DAC control operates negatively, the GPIO bit must be set to 0 to draw high current.
	 * BSRR register is composed of 32 bits, [0:15] is responsible for Set Bit and [16:31] is responsible for Reset Bit.
	 *
	 * The GPIO pins that control the DAC are GPIOB Pins 1 to 8, and operation is performed by covering the bit mask to control only the corresponding pins.
	 * Control of the DAC is entered through an operation with a bit mask, and since the DAC operates negatively, a NOT operation must be performed during bit operation.
	 *
	 * GPIO DAC Control PIN :: GPIOB Pin1 ~ GPIOB Pin8
	 * >> GPIOB : 0000 0001 1111 1110 >> 0x01FE
	 * >> DAC BIT AREA : 0000 000[1 1111 111]0 >> 0X01FE
	 * ------------------------------
	 * Example
	 * DAC_TARGET_VALUE (0~255) : 192 : 0000 0000 1100 0000 >> ~ 1111 1111 0011 1111
	 * BIT MASK : 0x01FE
	 * ------------------------------
	 * STIM_LIB_DAC_ODR : DAC_TARGET_VALUE << 1 = 0000 0001 1000 0000
	 * BIT MASK : 0000 0001 1111 1110
	 *
	 * BIT RESET[16:31] = (((0000 0001 1000 0000) & (0000 0001 1111 1110)) << 16) = 0000 0001 1000 0000
	 * BIT SET[0:15] = ((~(0000 0001 1000 0000)) & (0000 0001 1111 1110))
	 * 	>> ((1111 1110 0111 1111)) & (0000 0001 1111 1110)) = 0000 0000 0111 1110
	 * ------------------------------
	 *			--- RESET BIT ---	--- SET BIT ---
	 * BSSR = [0000 0001 1000 0000][0000 0000 0111 1110]
	 * */
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
