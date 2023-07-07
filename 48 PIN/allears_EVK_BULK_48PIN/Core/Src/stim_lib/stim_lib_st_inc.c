/*
 * stim_lib_st_inc.c
 *
 *  Created on: 2023. 4. 25.
 *      Author: eidos
 */
#include <stdlib.h>

#include "stim_lib.h"
#include "stim_lib_st_inc.h"
#include "stim_lib_st_init.h"
#include "stim_lib_private.h"
#include "stim_lib_stim_cfg.h"
#include "stim_lib_volt.h"
#include "stim_lib_current.h"
#include "stim_lib_error.h"

#include "td_debug.h"

/*
 * Trigger Check Definition
 * */
#define STIM_LIB_TRG_INPUT_PIN_IS_ENABLED(param)						(param == STIM_LIB_STIM_TRIGGER_INPUT_PIN && STIM_LIB_STATE_TRG_IN_ENABLE == stim_lib_trg_input_enable)
#define STIM_LIB_TRG_INPUT_PIN_IS_HIGH()								(HAL_GPIO_ReadPin(STIM_LIB_STIM_TRIGGER_INPUT_GPIO_PORT, STIM_LIB_STIM_TRIGGER_INPUT_PIN) == GPIO_PIN_SET)
#define STIM_LIB_TRG_INPUT_PIN_IS_LOW()									(HAL_GPIO_ReadPin(STIM_LIB_STIM_TRIGGER_INPUT_GPIO_PORT, STIM_LIB_STIM_TRIGGER_INPUT_PIN) == GPIO_PIN_RESET)

static void stim_lib_trg_input_active_low_hanlde(void);
static void stim_lib_trg_input_active_high_handle(void);

/*
 * ADC WATCH DOC INTERRUPT
 * Calling from "HAL_ADC_LevelOutOfWindowCallback()" Function
 * */
void stimLib_adcWatchdoc_callback(void)
{
	TD_DEBUG_PRINT(("WATCH DOC INTERRUPT :: stimLib_adcWatchdoc_callback()\r\n"));
	TD_DEBUG_PRINT(("SYSTEM ALL STOP AND RESET\r\n"));

	stimLib_errorHandle();

	NVIC_SystemReset();
}

/*
 * ADC WATCH DOC INTERRUPT
 * Calling from "HAL_ADC_ErrorCallback()" Function
 * */
void stimLib_adcError_callback(void)
{
	TD_DEBUG_PRINT(("ADC ERROR :: stimLib_adcError_callback()\r\n"));

	stimLib_errorHandle();

	NVIC_SystemReset();
}

/*
 * STEPUP CONTROL CALLBACK
 * Calling from "HAL_TIM_PeriodElapsedCallback()" Function
 * */
void stimLib_stepup_ctrlCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM6)
	{
		stimLib_stepup_ctrlScheduler();
#ifdef STIM_LIB_EVKIT_CC
		stimLib_dac_ctrlScheduler();
#endif
	}
}

/*
 * Trigger Input Callback
 * Calling from "HAL_GPIO_EXTI_Callback()" Function
 * */
void stimLib_trgInput_callback(uint16_t gpio_pin)
{
	if (STIM_LIB_TRG_INPUT_PIN_IS_ENABLED(gpio_pin))
	{
		if (STIM_LIB_TRG_INPUT_TOGGLE_IS_ACT_LOW())
		{
			stim_lib_trg_input_active_low_hanlde();
		}

		else if (STIM_LIB_TRG_INPUT_TOGGLE_IS_ACT_HIGH())
		{
			stim_lib_trg_input_active_high_handle();
		}

		else if (STIM_LIB_TRG_INPUT_IS_FALLING_EDGE(
		) || STIM_LIB_TRG_INPUT_IS_RISING_EDGE())
		{
			/*
			 * STIM START
			 * Initiate stimulation by trigger input.
			 * RISING EDGE and FALLING EDGE
			 * */
			stimLib_stimStart();
		}
	}
}

void stimLib_timError_callback(TIM_HandleTypeDef *htim)
{
	stimLib_errorHandle();
}

void stimLib_timPwmPluseFinished_callback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2)
	{
		stimLib_stimPulseStop();
	}
}

/* Define static functions */
static void stim_lib_trg_input_active_low_hanlde(void)
{
	if (STIM_LIB_TRG_INPUT_PIN_IS_HIGH())
	{
		/*
		 * Force low of TIM PIO outputs
		 * CAUTION: OCREF must be normalized in PWM mode. Refer to TIM stop Function, stimLib_stimStopRaw()
		 * Stop when the toggle signal is HIGH.
		 * */
		stimLib_stimStopDelayRaw();
	}
	else
	{
		/*
		 * STIM START
		 * Initiate stimulation by trigger input.
		 * Start when the toggle signal is LOW.
		 * */
		stimLib_stimStart();
	}
}

static void stim_lib_trg_input_active_high_handle(void)
{
	if (STIM_LIB_TRG_INPUT_PIN_IS_LOW())
	{
		/*
		 * Force low of TIM PIO outputs
		 * CAUTION: OCREF must be normalized in PWM mode. Refer to TIM stop Function, stimLib_stimStopRaw()
		 * Stop when the toggle signal is LOW.
		 * */
		stimLib_stimStopDelayRaw();
	}
	else
	{
		/*
		 * STIM START
		 * Initiate stimulation by trigger input.
		 * Start when the toggle signal is HIGH.
		 * */
		stimLib_stimStart();
	}
}

