#ifndef	TD_UART1_H

#define TD_UART1_H

#define TD_COMM_USART1_RX_DMA						DMA1_Channel4_IRQn
#define TD_USART1_MUTEX_LOCK()						HAL_NVIC_DisableIRQ(TD_COMM_USART1_RX_DMA)
#define TD_USART1_MUTEX_UNLOCK()					HAL_NVIC_EnableIRQ(TD_COMM_USART1_RX_DMA)


void tdUart1_init(void);

void tdUart1_sendData(uint8_t* data, uint8_t size);

void tdUart1_receiveData(void);


#endif	/* TD_UART1_H */

