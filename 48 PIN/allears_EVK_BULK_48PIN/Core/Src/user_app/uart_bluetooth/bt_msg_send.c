#include <string.h>
#include "bt_msg_private.h"

#include "td_private.h"
#include "td_uart1.h"
#include "td_stim_param_setting.h"

#include "td_debug.h"

#define BT_MSG_RES_LEN_DEFAULT				2
#define BT_MSG_RES_DATA_LEN_MAX				15

#define BT_MSG_SEND_BUF						gBtMsg_sendBuf

#define BT_MSG_SEND()						tdUart1_sendData(BT_MSG_SEND_BUF, BT_MSG_SIZE_MAX)

uint8 gBtMsg_sendBuf[BT_MSG_SIZE_MAX];

static inline void bt_msg_send_init_INLINE(void)
{
	memset(BT_MSG_SEND_BUF, 0x00, BT_MSG_SIZE_MAX);

	BT_MSG_SEND_BUF[BT_MSG_START_CODE_IX] = BT_MSG_START_CODE;
}

void bt_msg_res(bt_msg_type_t type, bt_msg_res_t res, uint8 *data, uint8 data_len)
{
	if (type >= BT_MSG_TYPE_MAX || res >= BT_MSG_RES_MAX)
		return;

	if (data == NULL)
		data_len = 0;
	else if (data_len > BT_MSG_RES_DATA_LEN_MAX)
		data_len = BT_MSG_RES_DATA_LEN_MAX;

	bt_msg_send_init_INLINE();

	BT_MSG_SEND_BUF[BT_MSG_TYPE_IX] = BT_MSG_RES;
	BT_MSG_SEND_BUF[BT_MSG_LEN_IX] = BT_MSG_RES_LEN_DEFAULT + data_len;
	BT_MSG_SEND_BUF[BT_MSG_DATA_IX] = type;
	BT_MSG_SEND_BUF[BT_MSG_DATA_IX + 1] = res;

	if (data_len > 0)
	{
		memcpy(&BT_MSG_SEND_BUF[BT_MSG_DATA_IX + BT_MSG_RES_LEN_DEFAULT], data, data_len);
	}

	BT_MSG_SEND();
}

void bt_state_ind(void)
{
	bt_msg_send_init_INLINE();

#if 1
	BT_MSG_SEND_BUF[BT_MSG_TYPE_IX] = BT_STATE_IND;
	BT_MSG_SEND_BUF[BT_MSG_LEN_IX] = 4;
	BT_MSG_SEND_BUF[BT_MSG_DATA_IX] = td_isStimulationStarted(); /* stimul start */
	BT_MSG_SEND_BUF[BT_MSG_DATA_IX + 1] = td_getCurrentStimulationMode(); /* mode */
	BT_MSG_SEND_BUF[BT_MSG_DATA_IX + 2] = td_getCurrentStimulationLevel(); /* level */
	BT_MSG_SEND_BUF[BT_MSG_DATA_IX + 3] = td_getStimDetectionLevel(); /* elect detect */
#endif

	BT_MSG_SEND();
}

