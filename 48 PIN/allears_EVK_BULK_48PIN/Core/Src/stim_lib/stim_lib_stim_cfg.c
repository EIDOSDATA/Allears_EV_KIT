/*
 * stim_lib_stim_cfg.c
 *
 *  Created on: Apr 26, 2023
 *      Author: eidos
 */
#include "stim_lib_st_msp.h"
#include "stim_lib_st_inc.h"
#include "stim_lib_stim_cfg.h"
#include "stim_lib_volt.h"

#define STIM_LIB_SIGNAL_FREQ_MIN						1
#define STIM_LIB_SIGNAL_FREQ_MAX						60
#define STIM_LIB_SIGNAL_FREQ_IS_VALID(param)			(STIM_LIB_SIGNAL_FREQ_MIN <= param && param <= STIM_LIB_SIGNAL_FREQ_MAX)

#define STIM_LIB_SIGNAL_PW_MIN							100
#define STIM_LIB_SIGNAL_PW_MAX							1000
#define STIM_LIB_SIGNAL_PW_STEP							100
#define STIM_LIB_SIGNAL_PW_IS_VALID(param)				(STIM_LIB_SIGNAL_PW_MIN <= param && param <= STIM_LIB_SIGNAL_PW_MAX)

#define STIM_LIB_SIGNAL_DEGREE_MIN						0
#define STIM_LIB_SIGNAL_DEGREE_MAX						255
#define STIM_LIB_SIGNAL_DGREE_IS_VALID(param)			(param <= STIM_LIB_SIGNAL_DEGREE_MAX)				/* unsigned is always bigger than 0 */

bool stimLib_signalParamCheck(stim_signal_cfg_t *cfg)
{
	if (cfg == NULL)
	{
		return false;
	}

	/* Base check */
	if (STIM_LIB_SIGNAL_FREQ_IS_VALID(
			cfg->freq) && STIM_LIB_SIGNAL_PW_IS_VALID(cfg->pulse_width) && STIM_LIB_SIGNAL_DGREE_IS_VALID(cfg->degree))
	{
		/* Step check of pulse width */
		cfg->pulse_width = (cfg->pulse_width / STIM_LIB_SIGNAL_PW_STEP) * STIM_LIB_SIGNAL_PW_STEP;

		return true;
	}

	return false;
}

bool stimLib_triggerParamCheck(stim_trg_cfg_t *cfg)
{
	if (cfg == NULL)
	{
		return false;
	}

	return true;
}

bool stimLib_paramSetting(void)
{
	bool rslt;

	rslt = stimLib_stimPulseSetiing();

	if (rslt == true)
	{
		stimLib_paramTrgSettingRaw();
	}

	return rslt;
}

void stimLib_paramClear(void)
{
	stimLib_paramTrgResettingRaw();

	/* State clear */
	stimLib_stateParamClear();

	/* ST parameter clear :: NOT YET */
}

void stimLib_stimPulseStart(void)
{
	stimLib_pulseConfigRaw();
	stimLib_stimStartRaw();
	if (STIM_LIB_STATE_TRG_VOLT_PRESTART == false && STIM_LIB_STEPUP_IS_STARTED() == false)
	{
		stimLib_stepupStart();
	}
}

void stimLib_stimPulseStop(void)
{
	if (stimLib_stateGet() == stim_lib_state_stim_stopping)
	{
		stimLib_stimStopRaw();

		/* To save pulst start time */
		stimLib_stimPulseSetiing();

		stimLib_stateSet(stim_lib_state_session_idle);
	}
}

bool stimLib_stimPulseSetiing(void)
{
	return stimLib_paramPulseSettingRaw();
}

