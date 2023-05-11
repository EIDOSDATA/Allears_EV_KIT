#include "stm32l4xx_hal.h"

#include "stim_lib.h"
#include "stim_lib_state.h"
#include "stim_lib_volt.h"
#include "stim_lib_stim_cfg.h"
#include "stim_lib_type.h"
#include "stim_lib_common.h"

#include "td_debug.h"

bool stimLib_chkSystem(void)
{
#ifdef	STM32L412xx
	return true;
#else
	return false;
#endif

}

uint32_t stimLib_getTick(void)
{
	return HAL_GetTick();
}

void stimLib_delay(uint32_t ms)
{
	HAL_Delay(ms);
}

void sitmLib_emergencyStop(void)
{
	if (stimLib_stateGet() == stim_lib_state_uninitialized)
	{
		return;
	}
	stimLib_stepupStop();
	stimLib_stimStopRaw();
	stimLib_paramClear();

	stimLib_stateSet(stim_lib_state_idle);

	TD_DEBUG_PRINT(("STIM_LIB: EMERGENCY STOP!!\n"));
}

