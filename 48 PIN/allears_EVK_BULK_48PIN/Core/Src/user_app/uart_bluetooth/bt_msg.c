#include "td_private.h"
#include "bt_msg_private.h"
#include "td_uart1.h"
#include "td_uart1_q.h"

#include "td_debug.h"

#define BT_MSG_RCV_BUF_SIZE						BT_MSG_SIZE_MAX

#ifdef TD_DEBUG_ENABLED
#define BT_MSG_RCV_FLUSH_TIMEOUT			5000 
#else
#define BT_MSG_RCV_FLUSH_TIMEOUT			1000
#endif

#define BT_MSG_SEND_NUM							5

#define BT_MSG_RCV_IX							gBtMsgState.ix
#define BT_MSG_RCV_BUF							gBtMsgState.buf
#define BT_MSG_RCV_FLUSH_CHK_TIME				gBtMsgState.flush_chk_time
#define BT_MSG_RCV_HANDSHAKING_TIME				gBtMsgState.handshaking_time

typedef struct
{
	uint8 ix;
	uint8 buf[BT_MSG_RCV_BUF_SIZE];
	uint32 flush_chk_time;
	uint32 handshaking_time;
} bt_msg_rcv_state_t;

bt_msg_rcv_state_t gBtMsgState;

static __INLINE void bt_msg_rcv_data_init_INLINE(void)
{
	for (BT_MSG_RCV_IX = 0; BT_MSG_RCV_IX < BT_MSG_SIZE_MAX; BT_MSG_RCV_IX++)
	{
		BT_MSG_RCV_BUF[BT_MSG_RCV_IX] = 0;
	}

	BT_MSG_RCV_IX = 0;
}

static void bt_msg_handle(uint8 *msg);

void btMsg_init(void)
{
	bt_msg_rcv_data_init_INLINE();
}

void btMsg_rcvData_handle(void)
{
	/*
	 Reason that statics are used is to fast tdUart1Q_getData() call.
	 Because tdUart1Q_getData() disables USRAT1 interrupt internally.
	 */
	static uint8 st_msg_rcv_data[BT_MSG_RCV_BUF_SIZE];
	static uint8 st_msg_rcv_data_len;
	uint8 i;

	do
	{
		st_msg_rcv_data_len = tdUart1Q_getData(st_msg_rcv_data,
		BT_MSG_RCV_BUF_SIZE);
		i = BT_MSG_RCV_BUF_SIZE;

		if (st_msg_rcv_data_len > 0)
		{
			i = 0;

			/* Refresh buffer flush timeout */
			BT_MSG_RCV_FLUSH_CHK_TIME = HAL_GetTick();
		}

		while (st_msg_rcv_data_len > i)
		{
			if ((st_msg_rcv_data[i] == BT_MSG_START_CODE) || BT_MSG_RCV_IX != 0)
			{
				BT_MSG_RCV_BUF[BT_MSG_RCV_IX] = st_msg_rcv_data[i];
				BT_MSG_RCV_IX++;
				i++;

				if (BT_MSG_RCV_IX == BT_MSG_RCV_BUF_SIZE)
				{
					bt_msg_handle(BT_MSG_RCV_BUF);

					bt_msg_rcv_data_init_INLINE();
				}
			}
		}

	} while (st_msg_rcv_data_len > 0);

	if (BT_MSG_RCV_IX > 0)
	{
		/* Check flush timeout */
		if (HAL_GetTick() - BT_MSG_RCV_FLUSH_CHK_TIME > BT_MSG_RCV_FLUSH_TIMEOUT)
		{
			TD_DEBUG_PRINT(("BT_MSG: Flush timeout\n"));
			TD_DEBUG_PRINT_DATA_HEX(BT_MSG_RCV_BUF, BT_MSG_RCV_IX);

			bt_msg_rcv_data_init_INLINE();
		}
	}

}

static void bt_msg_handle(uint8 *msg)
{
	if (msg == NULL)
		return;

	switch (msg[BT_MSG_TYPE_IX])
	{
	case BT_STATE_REQ:
		TD_DEBUG_PRINT(("BT_STATE_REQ\n"));
		bt_state_req(msg);
		break;

	case BT_MODE_SET_REQ:
		TD_DEBUG_PRINT(("BT_MODE_SET_REQ\n"));
		bt_mode_set_req(msg);
		break;

	case BT_LEVEL_SET_REQ:
		TD_DEBUG_PRINT(("BT_LEVEL_SET_REQ\n"));
		bt_level_set_req(msg);
		break;

	case BT_STIMUL_REQ:
		TD_DEBUG_PRINT(("BT_STIMUL_REQ\n"));
		bt_stimul_req(msg);
		break;

	case BT_ELECT_DETECT_REQ:
		TD_DEBUG_PRINT(("BT_ELECT_DETECT_REQ\n"));
		bt_elect_detect_req(msg);
		break;

	case BT_MAN_MODE_REQ:
		TD_DEBUG_PRINT(("BT_MAN_MODE_REQ\n"));
		bt_man_mode_req(msg);
		break;

	case BT_FW_VER_REQ:
		TD_DEBUG_PRINT(("BT_FW_VER_REQ\n"));
		bt_fw_ver_req();
		break;

	case BT_STIM_LIB_VER_REQ:
		TD_DEBUG_PRINT(("BT_STIM_LIB_VER_REQ\n"));
		bt_stim_lib_ver_req();
		break;

	case BT_STIM_STIMUL_TYPE_REQ:
		TD_DEBUG_PRINT(("BT_STIM_STIMUL_TYPE_REQ\n"));
		bt_stim_stimul_type_req();
		break;

	case BT_STIM_SYS_RESET_REQ:
		TD_DEBUG_PRINT(("BT_STIM_SYS_RESET_REQ\n"));
		bt_stim_sys_reset_req();
		break;

	case BT_STIM_TRG_CFG_REQ:
		TD_DEBUG_PRINT(("BT_STIM_TRG_CFG_REQ\n"));
		bt_stim_trg_cfg_req(msg);
		break;

	default:
		TD_DEBUG_PRINT(("Unknown BT MSG: %d\n", msg[BT_MSG_TYPE_IX]));
		bt_msg_res(msg[BT_MSG_TYPE_IX], BT_MSG_RES_UNKNOWN_MSG, NULL, 0);
		break;
	}
}

