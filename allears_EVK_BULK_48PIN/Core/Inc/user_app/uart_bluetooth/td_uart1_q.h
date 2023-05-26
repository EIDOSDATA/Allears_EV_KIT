#ifndef	TD_UART1_Q_H

#define TD_UART1_Q_H


void tdUart1Q_init(void);

uint8 tdUart1Q_putData(uint8* data, uint8 data_len);

uint8 tdUart1Q_getData(uint8* data, uint8 data_len);

#endif	/* TD_UART1_Q_H */

