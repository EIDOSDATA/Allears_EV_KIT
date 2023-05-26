#ifndef TD_DEBUG_H
#define TD_DEBUG_H

#ifdef	TD_DEBUG_ENABLED
#include <stdio.h>

/* Enable debug print as each module in here */
#define TD_DEBUG_INIT()											tdDebugInit()
#define TD_DEBUG_DEINIT()										tdDebugDeInit()
#define TD_DEBUG_PRINT(x)										printf x

#define TD_DEBUG_PRINT_DATA_CHAR(data, len)						tdDebugPrintChar(data, len)
#define TD_DEBUG_PRINT_DATA_HEX(data, len)						tdDebugPrintHex(data, len)

void tdDebugInit(void);

void tdDebugDeInit(void);

void tdDebugPrintChar(uint8_t *data, uint16_t len);

void tdDebugPrintHex(uint8_t *data, uint16_t len);

#else

#define	TD_DEBUG_INIT()
#define	TD_DEBUG_DEINIT()
#define	TD_DEBUG_PRINT(x)					
#define	TD_DEBUG_PRINT_DATA_CHAR(data, len)
#define	TD_DEBUG_PRINT_DATA_HEX(data, len)

#endif	/* TD_DEBUG_ENABLED */

#endif	/* TD_DEBUG_H */

