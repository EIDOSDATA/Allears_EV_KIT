#include "stim_lib_private.h"
#include "stim_lib_stim_cfg.h"
#include "stim_lib_state.h"

#include "td_debug.h"

#define STIM_LIB_CUR_STATE				exStimLib_state.cur_state

stim_lib_state_data_t exStimLib_state =
{ stim_lib_state_uninitialized,

{ 0, 0, 0, NULL }, /* 10Hz, Pulse Width 1ms, DAC 1, no callback */

{ false, false, 0, false, 0, false } };

stim_lib_state_t stimLib_stateGet()
{
	return STIM_LIB_CUR_STATE;
}

bool stimLib_stateSet(stim_lib_state_t set_state)
{
	bool rslt = false;

	if (STIM_LIB_CUR_STATE != set_state)
	{
		switch (set_state)
		{
		case stim_lib_state_uninitialized:
			if (STIM_LIB_CUR_STATE == stim_lib_state_idle)
			{
				rslt = true;
			}
			break;

		case stim_lib_state_idle:
			if (STIM_LIB_CUR_STATE == stim_lib_state_uninitialized || STIM_LIB_CUR_STATE == stim_lib_state_session_idle
					|| STIM_LIB_CUR_STATE == stim_lib_state_stimulating || STIM_LIB_CUR_STATE == stim_lib_state_stim_stopping)
			{
				rslt = true;
			}
			break;

			/*
			 * TODO:
			 * STIM TEST
			 * ADD POINT : State Simulating >> Session IDLE
			 * */
		case stim_lib_state_session_idle:
			if (STIM_LIB_CUR_STATE == stim_lib_state_idle || STIM_LIB_CUR_STATE == stim_lib_state_stim_stopping)
			{
				rslt = true;
			}
			break;

		case stim_lib_state_stimulating:
			if (STIM_LIB_CUR_STATE == stim_lib_state_session_idle)
			{
				rslt = true;
			}
			break;

		case stim_lib_state_stim_stopping:
			if (STIM_LIB_CUR_STATE == stim_lib_state_stimulating)
			{
				rslt = true;
			}
			break;

		case stim_lib_state_error:
			rslt = true;
			break;

		default:
			rslt = false;
			break;
		}

		if (rslt == true)
		{
			STIM_LIB_CUR_STATE = set_state;
		}

	}

	return rslt;

}

void stimLib_stateSigParamSet(stim_signal_cfg_t *cfg)
{
	if (stimLib_signalParamCheck(cfg) == false)
	{
		return;
	}

	STIM_LIB_STATE_SIG_FREQ = cfg->freq;
	STIM_LIB_STATE_SIG_PW = cfg->pulse_width;
	STIM_LIB_STATE_SIG_DEGREE = cfg->degree;
	STIM_LIB_STATE_SIG_PEAK_DET_CALLBACK = cfg->peak_detect_callback;
}

void stimLib_stateTrgParamSet(stim_trg_cfg_t *cfg)
{
	if (stimLib_triggerParamCheck(cfg) == false)
	{
		return;
	}

	STIM_LIB_STATE_TRG_VOLT_PRESTART = cfg->volt_prestart;

	STIM_LIB_STATE_TRG_OUT_ENABLE = cfg->trg_out_enable;
	STIM_LIB_STATE_TRG_OUT_ACT_POL = cfg->trg_out_active_pol;

	STIM_LIB_STATE_TRG_IN_ENABLE = cfg->trg_in_enable;
	STIM_LIB_STATE_TRG_IN_ACT_POL = cfg->trg_in_active_pol;
	STIM_LIB_STATE_TRG_IN_TOGGLED = cfg->trg_in_toggled;

}

bool stimLib_stateIsInSession(void)
{
	if (STIM_LIB_CUR_STATE == stim_lib_state_session_idle || STIM_LIB_CUR_STATE == stim_lib_state_stimulating)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool stimLib_stateSigParamCheck(void)
{
	if (stimLib_signalParamCheck(&STIM_LIB_STATE_SIG) == true && stimLib_triggerParamCheck(&STIM_LIB_STATE_TRG) == true)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void stimLib_stateParamClear(void)
{
	STIM_LIB_STATE_SIG_FREQ = 0;
	STIM_LIB_STATE_SIG_PW = 0;
	STIM_LIB_STATE_SIG_DEGREE = 0;
	STIM_LIB_STATE_SIG_PEAK_DET_CALLBACK = NULL;

	STIM_LIB_STATE_TRG_VOLT_PRESTART = false;
	STIM_LIB_STATE_TRG_OUT_ENABLE = false;
	STIM_LIB_STATE_TRG_OUT_ACT_POL = 0;
	STIM_LIB_STATE_TRG_IN_ENABLE = false;
	STIM_LIB_STATE_TRG_IN_ACT_POL = 0;
	STIM_LIB_STATE_TRG_IN_TOGGLED = false;
}

