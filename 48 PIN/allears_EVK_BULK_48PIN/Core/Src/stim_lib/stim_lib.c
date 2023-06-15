#include <string.h>
#include "stm32l4xx_hal.h"

#include "stim_lib.h"
#include "stim_lib_common.h"
#include "stim_lib_st_init.h"
#include "stim_lib_init.h"
#include "stim_lib_private.h"
#include "stim_lib_stim_cfg.h"
#include "stim_lib_state.h"
#include "stim_lib_volt.h"
#include "stim_lib_error.h"

#include "td_debug.h"

#define STIM_LIB_SESSION_VALID_TIME					2000

/*
 BRIEF
 - Get the version of stimulation library
 */
uint8_t stimLib_verGet(uint8_t *buff, uint8_t len)
{
	uint8_t ver_len;

	if (buff == NULL || len == 0)
	{
		return 0;
	}

	if (strlen(STIM_LIB_VERSION) > len)
	{
		ver_len = strlen(STIM_LIB_VERSION);
	}
	else
	{
		ver_len = len;
	}

	memcpy(buff, (uint8_t*) STIM_LIB_VERSION, ver_len);

	return ver_len;
}

/*
 BRIEF
 - Get stimulation type applied to library
 */
stim_lib_stim_type_t stimLib_stimTypeGet(void)
{
	return STIM_LIB_STIMULATION_TYPE;
}

/*
 BRIEF
 - Initalize stimulation library. Preempt system resources and peripherals.
 */
stim_lib_rsp_t stimLib_stimInit(void)
{
	stim_lib_rsp_t rslt;

	TD_DEBUG_PRINT(("STIM_LIB: stimLib_stimInit()\n"));

	if (stimLib_stateGet() != stim_lib_state_uninitialized)
	{
		return stim_lib_stim_rsp_invalid_status;
	}

	rslt = stimLib_initResrc();

	if (rslt == stim_lib_stim_rsp_ok)
	{
		stimLib_stateSet(stim_lib_state_idle);
	}

	return rslt;
}

/*
 BRIEF
 - Deinitalize stimulation library. Release system resources and peripherals.
 */
stim_lib_rsp_t stimLib_stimDeinit(void)
{
	return stimLib_deinitResrc();
}

/*
 BRIEF
 - Get current state of stimulation library.
 */
stim_lib_state_t stimLib_stimCurStateGet(void)
{
	return stimLib_stateGet();
}

/* ********** TODAY QUEST ********** */
/*
 BRIEF
 - Setup signal pusle of stimulation configuration.
 */
stim_lib_rsp_t stimLib_stimSignalConfig(stim_signal_cfg_t *signal_cfg)
{
	if (signal_cfg == NULL || stimLib_signalParamCheck(signal_cfg) == false)
	{
		return stim_lib_stim_rsp_invalid_parameter;
	}

	if (stimLib_stateGet() == stim_lib_state_idle)
	{
		stimLib_stateSigParamSet(signal_cfg);

		return stim_lib_stim_rsp_ok;
	}
	else
	{
		return stim_lib_stim_rsp_invalid_status;
	}
}

/*
 BRIEF
 - Setup trigger of stimulation configuration.
 */
stim_lib_rsp_t stimLib_stimTriggerConfig(stim_trg_cfg_t *trg_cfg)
{
	if (trg_cfg == NULL || stimLib_triggerParamCheck(trg_cfg) == false)
	{
		return stim_lib_stim_rsp_invalid_parameter;
	}

	if (stimLib_stateGet() == stim_lib_state_idle)
	{
		stimLib_stateTrgParamSet(trg_cfg);

		return stim_lib_stim_rsp_ok;
	}
	else
	{
		return stim_lib_stim_rsp_invalid_status;
	}

}

/* ********** END OF TODAY QUEST ********** */

/*
 BRIEF
 - Start stimulation session.
 */
stim_lib_rsp_t stimLib_stimSessionStart(void)
{
	bool param_rslt;

	if (stimLib_stateGet() == stim_lib_state_idle)
	{
		param_rslt = stimLib_stateSigParamCheck();

		if (param_rslt == true)
		{
			param_rslt = stimLib_paramSetting();

			/* VOLTAGE CONTROL PRESTART */
			if (STIM_LIB_STATE_TRG_VOLT_PRESTART == true)
			{
				/*
				 * STEP UP VOLTAGE CONTROL START
				 *
				 * START STEPUP PULSE TIMER :: TIMER 1
				 * START STEPUP FEEDBACK TIMER :: TIMER 6
				 * */
				stimLib_stepupStart();
			}

			stimLib_stateSet(stim_lib_state_session_idle);
			TD_DEBUG_PRINT(("STIM_LIB: Session Start >> Session IDLE\n"));

			return stim_lib_stim_rsp_ok;
		}
		else
		{
			return stim_lib_stim_rsp_invalid_parameter;
		}
	}

	else
	{
		return stim_lib_stim_rsp_invalid_status;
	}
}

/*
 BRIEF
 - Stop stimulation session.
 */
stim_lib_rsp_t stimLib_stimSessionStop(void)
{
	uint32_t tick;

	tick = HAL_GetTick();

	while (stimLib_stateGet() == stim_lib_state_stim_stopping && stimLib_getTick() - tick < STIM_LIB_SESSION_VALID_TIME)
		;

	if (stimLib_getTick() - tick >= STIM_LIB_SESSION_VALID_TIME)
	{
		/* ERROR */
		stimLib_errorHandle();

		return stim_lib_stim_rsp_system_fault;
	}

	if (stimLib_stateGet() == stim_lib_state_session_idle)
	{
		stimLib_stepupStop();

		stimLib_paramClear();

		stimLib_stateSet(stim_lib_state_idle);

		return stim_lib_stim_rsp_ok;
	}
	else
	{
		return stim_lib_stim_rsp_invalid_status;
	}
}

/*
 BRIEF
 - Start stimulation signal output in a session.
 */
stim_lib_rsp_t stimLib_stimStart(void)
{

	if (stimLib_stateGet() == stim_lib_state_session_idle)
	{
		stimLib_stimPulseStart();
		stimLib_stateSet(stim_lib_state_stimulating);

		return stim_lib_stim_rsp_ok;
	}
	else
	{
		return stim_lib_stim_rsp_invalid_status;
	}
}

/*
 BRIEF
 - Pause stimulation signal output in a session.
 */
stim_lib_rsp_t stimLib_stimPause(void)
{
	if (stimLib_stateGet() == stim_lib_state_stimulating)
	{
		/*
		 * TODO:
		 * STIM ENDPOINT TEST
		 * */
#if 0
		/*
		 * CALL STIMULATION STOP FUNCTION
		 * */
		stimLib_stimPulseStop();

		/* To save puls start time */
		stimLib_stimPulseSetiing();

		stimLib_stateSet(stim_lib_state_session_idle);
#else
		stimLib_stimStopDelayRaw();

#endif

		return stim_lib_stim_rsp_ok;
	}
	else
	{
		return stim_lib_stim_rsp_invalid_status;
	}
}

/*
 * TODO:
 * STIM TEST
 * */
#if 0
stim_lib_rsp_t stimLib_stimIntensiveChange(stim_signal_cfg_t *signal_cfg)
{
	if (signal_cfg == NULL || stimLib_signalParamCheck(signal_cfg) == false)
	{
		return stim_lib_stim_rsp_invalid_parameter;
	}

	if (stimLib_stateGet() == stim_lib_state_idle || stimLib_stateGet() == stim_lib_state_session_idle
			|| stimLib_stateGet() == stim_lib_state_stimulating)
	{
		stimLib_stateSigParamSet(signal_cfg);

		if (stimLib_stateGet() == stim_lib_state_stimulating)
		{
			stimLib_stimStopRaw();
			for (int i = 0; i < 0xffff; i++)
				;
		}

		while (stimLib_paramPulseSettingRaw() != true)
			;
		/*
		 stimLib_stimPulseSetiing();
		 for (int i = 0; i < 0x1ffff; i++)
		 ;
		 */

		//if (stimLib_stateGet() == stim_lib_state_stimulating || stimLib_stateGet() == stim_lib_state_session_idle)
		if (stimLib_stateGet() == stim_lib_state_stimulating)
		{
			stimLib_pulseConfigRaw();
			for (int i = 0; i < 0xffff; i++)
				;
			stimLib_stimStartRaw();
		}
		return stim_lib_stim_rsp_ok;
	}
	else
	{
		return stim_lib_stim_rsp_invalid_status;
	}
}
#endif

#if 0
/*
 * TODO:
 * STIM TEST
 * */
stim_lib_rsp_t stimLib_stimIntensiveChange(stim_signal_cfg_t *signal_cfg)
{
	if (signal_cfg == NULL || stimLib_signalParamCheck(signal_cfg) == false)
	{
		return stim_lib_stim_rsp_invalid_parameter;
	}

	if (stimLib_stateGet() == stim_lib_state_idle || stimLib_stateGet() == stim_lib_state_session_idle
			|| stimLib_stateGet() == stim_lib_state_stimulating)
	{
		stimLib_stateSigParamSet(signal_cfg);

		if (stimLib_stateGet() == stim_lib_state_stimulating)
		{
			stimLib_stimStopDelayRaw();
			stimLib_stateSet(stim_lib_state_stim_stopping);
#if 0
			stimLib_stimStopRaw();
			for (int i = 0; i < 0xffff; i++)
				;
#endif
		}

		stimLib_stimPulseSetiing();

		//if (stimLib_stateGet() == stim_lib_state_stimulating || stimLib_stateGet() == stim_lib_state_session_idle)
		if (stimLib_stateGet() == stim_lib_state_session_idle)
		{
			stimLib_stimPulseStart();
		}
		return stim_lib_stim_rsp_ok;
	}
	else
	{
		return stim_lib_stim_rsp_invalid_status;
	}
}

#endif

#if 1
stim_lib_rsp_t stimLib_stimIntensiveChange(stim_signal_cfg_t *signal_cfg)
{
	if (signal_cfg == NULL || stimLib_signalParamCheck(signal_cfg) == false)
	{
		return stim_lib_stim_rsp_invalid_parameter;
	}

	if (stimLib_stateGet() == stim_lib_state_idle || stimLib_stateGet() == stim_lib_state_session_idle
			|| stimLib_stateGet() == stim_lib_state_stimulating)
	{
		stimLib_stateSigParamSet(signal_cfg);
		stimLib_stimPulseSetiing();
		stimLib_pulseConfigRaw();

		for (int i = 0; i < 0x13880; i++)
			;
		if (stimLib_stateGet() == stim_lib_state_stimulating)
		{
			stimLib_stimStartRaw();
		}

		return stim_lib_stim_rsp_ok;
	}
	else
	{
		return stim_lib_stim_rsp_invalid_status;
	}
}
#endif
