#include "stim_lib.h"
#include "stim_lib_state.h"
#include "stim_lib_volt.h"
#include "stim_lib_stim_cfg.h"
#include "stim_lib_error.h"

#include "td_debug.h"

void stimLib_errorHandle(void)
{
	/*
	 TODO:

	 1. Stop voltage
	 2. Stop Stimulation
	 3. Set state to error
	 ...
	 */
	stimLib_stepupStop();
	stimLib_stimPulseStop();
	stimLib_paramClear();
	stimLib_stateSet(stim_lib_state_error);

	sitmLib_appErrorHandle_callback();

	TD_DEBUG_PRINT(("STIM_LIB: ERROR!!\n"));
}

void sitmLib_appErrorHandle_callback(void)
{
}
