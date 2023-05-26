#include <Inc_pub/stim_lib_type.h>
#include "stm32l4xx_hal.h"



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

