/* STM32 Debug port retarget */

#include <stdio.h>
#include "main.h"
#include "td_debug.h"

#define TD_RETARGET_UART					USART3

/*
 This fucntion is used in other libraries, so must be defined although in case DEBUG_PRINT_ENABLED is not defined
 */

#ifdef __GNUC__

int __io_putchar(int ch);

/* With GCC, printf calls _write() */
int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		__io_putchar(*ptr++);
	}
	return len;
}

int __io_putchar(int ch)
#else	/* Keil */

struct __FILE { int handle; };

FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f)

#endif /* __GNUC__ */
{
	if (ch == '\n')
	{
		/* output CR  */
		while (!(TD_RETARGET_UART->ISR & UART_FLAG_TXE))
			;

		TD_RETARGET_UART->TDR = '\r';
	}

	while (!(TD_RETARGET_UART->ISR & UART_FLAG_TXE))
		;

	TD_RETARGET_UART->TDR = (uint8_t) ch;

	return ch;
}

