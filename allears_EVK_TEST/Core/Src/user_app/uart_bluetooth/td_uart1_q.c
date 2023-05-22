#include <string.h>

#include "td_private.h"
#include "td_uart1.h"
#include "bt_msg_private.h"


#define TD_UART1_Q_BUF_SIZE					BT_MSG_SIZE_MAX
#define TD_UART1_Q_BUF_NUM					2
#define TD_UART1_Q_BUF_NUM_DEFUALT			0					/* Start from index 0 */


#define TD_UART1_Q_STATE					gTdUart1QState
#define TD_UART1_Q_STATE_BUF(num)			gTdUart1QState.td_uart1_q_buf[num]
#define TD_UART1_Q_STATE_BUF_LEN(num)		gTdUart1QState.td_uart1_q_buf_len[num]
#define TD_UART1_Q_FRONT					gTdUart1QState.cur_q_front
#define TD_UART1_Q_REAR						gTdUart1QState.cur_q_rear
#define TD_UART1_Q_ITEM_NUM					gTdUart1QState.item_num


typedef struct
{
	uint8 td_uart1_q_buf[TD_UART1_Q_BUF_NUM][TD_UART1_Q_BUF_SIZE];
	uint8 td_uart1_q_buf_len[TD_UART1_Q_BUF_NUM];
	uint8 cur_q_front;
	uint8 cur_q_rear;
	uint8 item_num;
} td_uart1_q_state_data_t;


td_uart1_q_state_data_t gTdUart1QState;


static inline void td_uart1_q_buf_clear(uint8 i)
{
	if(i < TD_UART1_Q_BUF_NUM)
	{
		memset(TD_UART1_Q_STATE_BUF(i), 0x00, TD_UART1_Q_BUF_SIZE);
		TD_UART1_Q_STATE_BUF_LEN(i) = 0;
	}
}

void tdUart1Q_init(void)
{
	uint8 i;

	for(i=0; i<TD_UART1_Q_BUF_NUM; i++)
	{
		td_uart1_q_buf_clear(i);
	}

	TD_UART1_Q_FRONT = 0;
	TD_UART1_Q_REAR = 0;
	TD_UART1_Q_ITEM_NUM = 0;
}

uint8 tdUart1Q_putData(uint8* data, uint8 data_len)
{
	if(data == NULL || data_len == 0)
		return 0;

	if(data_len > TD_UART1_Q_BUF_SIZE)
		data_len = TD_UART1_Q_BUF_SIZE;

	TD_UART1_Q_REAR = (TD_UART1_Q_REAR + 1) % TD_UART1_Q_BUF_NUM;

	memcpy(TD_UART1_Q_STATE_BUF(TD_UART1_Q_REAR), data, data_len);
	TD_UART1_Q_STATE_BUF_LEN(TD_UART1_Q_REAR) = data_len;

	TD_UART1_Q_ITEM_NUM++;

	return data_len;
}

uint8 tdUart1Q_getData(uint8* data, uint8 data_len)
{
	if(data == NULL || data_len == 0 || TD_UART1_Q_ITEM_NUM == 0)
		return 0;

	TD_UART1_Q_FRONT = (TD_UART1_Q_FRONT + 1) % TD_UART1_Q_BUF_NUM;

	if(TD_UART1_Q_STATE_BUF_LEN(TD_UART1_Q_FRONT) < data_len)
		data_len = TD_UART1_Q_STATE_BUF_LEN(TD_UART1_Q_FRONT);

	memcpy(data, TD_UART1_Q_STATE_BUF(TD_UART1_Q_FRONT), data_len);

	TD_USART1_MUTEX_LOCK();
		
	TD_UART1_Q_ITEM_NUM--;

	TD_USART1_MUTEX_UNLOCK();

	td_uart1_q_buf_clear(TD_UART1_Q_FRONT);

	return data_len;
}

