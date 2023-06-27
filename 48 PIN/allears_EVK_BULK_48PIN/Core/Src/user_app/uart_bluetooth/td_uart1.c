#include <string.h>

#include "main.h"

#include "td_private.h"
#include "bt_msg_private.h"
#include "td_uart1.h"
#include "td_uart1_q.h"
#include "td_debug.h"

#define TD_UART1_DMA_TX_BUF_SIZE				BT_MSG_SIZE_MAX
#define TD_UART1_DMA_RX_BUF_SIZE				BT_MSG_SIZE_MAX
#define TD_UART1_DMA_TX_WAIT_TIME				0xFFFF

#define TD_UART1_DMA_TX_BUF						gTdUart1_dmaTxBuf
#define TD_UART1_DMA_RX_BUF						gTdUart1_dmaRxBuf

#define TD_UART_DMA_TX_BUF_CLEAR()				memset(TD_UART1_DMA_TX_BUF, 0, TD_UART1_DMA_TX_BUF_SIZE)
#define TD_UART_DMA_RX_BUF_CLEAR()				memset(TD_UART1_DMA_RX_BUF, 0, TD_UART1_DMA_RX_BUF_SIZE)

uint8_t gTdUart1_dmaTxBuf[TD_UART1_DMA_TX_BUF_SIZE];
uint8_t gTdUart1_dmaRxBuf[TD_UART1_DMA_RX_BUF_SIZE];

void tdUart1_init(void)
{
	tdUart1Q_init();

	memset(TD_UART1_DMA_TX_BUF, 0, TD_UART1_DMA_TX_BUF_SIZE);
	memset(TD_UART1_DMA_RX_BUF, 0, TD_UART1_DMA_RX_BUF_SIZE);

	tdUart1_receiveData();
}

void tdUart1_sendData(uint8_t *data, uint8_t size)
{
	uint32 i;
	UART_HandleTypeDef *uart_h = tdUsart1_handlerGet();

	if (data == NULL || size == 0 || uart_h == NULL)
	{
		TD_DEBUG_PRINT(("tdUart1_sendData() ERR: PARAM\n"));
		return;
	}

#if 0
	if(uart_h->gState != HAL_UART_STATE_READY)
	{
		TD_DEBUG_PRINT(("tdUart1_sendData() ERR: BUSY\n"));
		return;
	}
#else
	/* To be used in interrupt handler also */
	for (i = 0; (i < TD_UART1_DMA_TX_WAIT_TIME) && (uart_h->gState != HAL_UART_STATE_READY); i++)
		;

	if (i >= TD_UART1_DMA_TX_WAIT_TIME)
	{
		TD_DEBUG_PRINT(("tdUart1_sendData() ERR: BUSY\n"));
		return;
	}
#endif

	TD_UART_DMA_TX_BUF_CLEAR();

	if (size > TD_UART1_DMA_TX_BUF_SIZE)
		size = TD_UART1_DMA_TX_BUF_SIZE;

	memcpy(TD_UART1_DMA_TX_BUF, data, size);

	HAL_UART_Transmit_DMA(uart_h, TD_UART1_DMA_TX_BUF, size);
	__HAL_DMA_DISABLE_IT(uart_h->hdmatx, DMA_IT_HT);

}

void tdUart1_receiveData(void)
{
	UART_HandleTypeDef *uart_h = tdUsart1_handlerGet();

	if (uart_h == NULL)
	{
		TD_DEBUG_PRINT(("tdUart1_receiveData() ERR: PARAM\n"));
		return;
	}

	if (uart_h->RxState != HAL_UART_STATE_READY)
	{
		TD_DEBUG_PRINT(("tdUart1_receiveData() ERR: BUSY\n"));
		return;
	}

	TD_UART_DMA_RX_BUF_CLEAR();

	HAL_UARTEx_ReceiveToIdle_DMA(uart_h, TD_UART1_DMA_RX_BUF, TD_UART1_DMA_RX_BUF_SIZE);
	__HAL_DMA_DISABLE_IT(uart_h->hdmarx, DMA_IT_HT);
}

/* Rx complete when Idle */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART1)
	{
#ifdef TD_UART_DEBUG
		TD_DEBUG_PRINT(("USART1 Rx event\n"));
#endif
		memcpy(TD_UART1_DMA_RX_BUF, huart->pRxBuffPtr, Size);

		HAL_UARTEx_ReceiveToIdle_DMA(huart, TD_UART1_DMA_RX_BUF, TD_UART1_DMA_RX_BUF_SIZE);
		__HAL_DMA_DISABLE_IT(huart->hdmarx, DMA_IT_HT);

		tdUart1Q_putData(TD_UART1_DMA_RX_BUF, Size);

		TD_UART_DMA_RX_BUF_CLEAR();
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
#ifdef TD_UART_DEBUG
	if (huart->Instance == USART1)
	{
		TD_DEBUG_PRINT(("USART1 Rx comlete\n"));
		TD_DEBUG_PRINT(("Something wrong\n"));
	}
#endif
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
#ifdef TD_UART_DEBUG
	if (huart->Instance == USART1)
	{
		TD_DEBUG_PRINT(("USART1 Rx Half comlete\n"));
		TD_DEBUG_PRINT(("Something wrong\n"));
	}
#endif
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
#ifdef TD_UART_DEBUG
	if (huart->Instance == USART1)
	{
		TD_DEBUG_PRINT(("USART1 Tx comlete\n"));
	}
#endif
}

void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
#ifdef TD_UART_DEBUG
	if (huart->Instance == USART1)
	{
		TD_DEBUG_PRINT(("USART1 Tx Half comlete\n"));
		TD_DEBUG_PRINT(("Something wrong\n"));
	}
#endif
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
#ifdef TD_UART_DEBUG
	if (huart->Instance == USART1)
	{
		TD_DEBUG_PRINT(("USART1 Error: %lx\n", huart->ErrorCode));
	}
#endif
}

