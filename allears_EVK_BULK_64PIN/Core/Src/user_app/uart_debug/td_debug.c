#include "main.h"
#include "td_debug.h"

#ifdef	TD_DEBUG_ENABLED

void tdDebugInit(void)
{

}

void tdDebugDeInit(void)
{

}

void tdDebugPrintChar(uint8_t *data, uint16_t len)
{
	uint16_t i;

	if (len == 0)
	{
		return;
	}

	for (i = 0; i < len; i++)
	{
		TD_DEBUG_PRINT(("%c", data[i]));
	}

	TD_DEBUG_PRINT(("\n"));
}

void tdDebugPrintHex(uint8_t *data, uint16_t len)
{
	uint16_t i;

	if (len == 0)
	{
		return;
	}

	for (i = 0; i < len; i++)
	{
		TD_DEBUG_PRINT(("%02x ", data[i]));
	}

	TD_DEBUG_PRINT(("\n"));
}

#endif	/*  TD_DEBUG_PRINT_ENABLED */

