/*
 * stim_lib_init.c
 *
 *  Created on: Apr 19, 2023
 *      Author: eidos
 */

#include "stm32l4xx_hal.h"
#include "stm32l412xx.h"

#include "stim_lib_private.h"
#include "stim_lib_common.h"
#include "stim_lib_init.h"
#include "stim_lib_st_init.h"
#include "stim_lib_stim_cfg.h"
#include "stim_lib_state.h"

#define STIM_LIB_RESOURCE_INIT_ERROR				false
#define STIM_LIB_RESOURCE_INIT_OK					true

static void stimLib_initResource(bool *resource_check)
{
	uint8_t i;

	if (resource_check == NULL)
	{
		return;
	}

	for (i = STIM_LIB_RESRC_START; i < stim_lib_resrc_max; i++)
	{
		resource_check[i] = false;
	}

	for (i = STIM_LIB_RESRC_START; i < stim_lib_resrc_max; i++)
	{
		switch (i)
		{
		case stim_lib_resrc_dma:
			resource_check[i] = stimLib_dmaInit();
			break;

		case stim_lib_resrc_tim1:
			resource_check[i] = stimLib_tim1_Init();
			break;

		case stim_lib_resrc_tim2:
			resource_check[i] = stimLib_tim2_Init();
			break;

		case stim_lib_resrc_tim6:
			resource_check[i] = stimLib_tim6_Init();
			break;

		case stim_lib_resrc_adc1:
			resource_check[i] = stimLib_adc1_Init();
			break;

		case stim_lib_resrc_adc2:
			resource_check[i] = stimLib_adc2_Init();
			break;

		case stim_lib_resrc_gpio:
			resource_check[i] = stimLib_gpioInit();
			break;

		default:
			break;
		}

		if (resource_check[i] == false)
		{
			break;
		}
	}
}

uint8_t stimLib_initResrc(void)
{
	uint8_t i;
	bool resrc_chk[stim_lib_resrc_max] =
	{ false, };

	/* Check system environment */
	if (stimLib_chkSystem() == false)
	{
		return stim_lib_stim_rsp_unsupported;
	}

	/* Check system clock */
	if (SystemCoreClock != STIM_LIB_SYSTEM_CLOCK_HZ)
	{
		return stim_lib_stim_rsp_unsupported;
	}

	if (stimLib_stateGet() != stim_lib_state_uninitialized)
	{
		return stim_lib_stim_rsp_invalid_status;
	}
	else
	{
		/* Resource Init */
		stimLib_initResource(resrc_chk);
	}

	for (i = STIM_LIB_RESRC_START; i < stim_lib_resrc_max; i++)
	{
		if (resrc_chk[i] == false)
		{
			break;
		}
	}

	if (i == stim_lib_resrc_max)
	{
		return stim_lib_stim_rsp_ok;
	}
	else
	{
		/* Keep the order of stim_lib_resrc_t elements */
		switch (i)
		{
		case stim_lib_resrc_gpio:
			stimLib_gpioDeinit();

		case stim_lib_resrc_adc2:
			stimLib_adc2_Deinit();

		case stim_lib_resrc_adc1:
			stimLib_adc1_Deinit();

		case stim_lib_resrc_tim6:
			stimLib_tim6_Deinit();

		case stim_lib_resrc_tim2:
			stimLib_tim2_Deinit();

		case stim_lib_resrc_tim1:
			stimLib_tim1_Deinit();

		case stim_lib_resrc_dma:
			stimLib_dmaDeinit();

		default:
			break;
		}

		return stim_lib_stim_rsp_failed;
	}

}

uint8_t stimLib_deinitResrc(void)
{
	stimLib_gpioDeinit();
	stimLib_dmaDeinit();
	stimLib_tim1_Deinit();
	stimLib_tim2_Deinit();
	stimLib_tim6_Deinit();
	stimLib_adc1_Deinit();
	stimLib_adc2_Deinit();

	return stim_lib_stim_rsp_uninitailized;
}
