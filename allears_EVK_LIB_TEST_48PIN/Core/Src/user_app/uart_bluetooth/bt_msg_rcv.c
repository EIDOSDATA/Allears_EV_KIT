#include <string.h>
#include "bt_msg_private.h"

#include "td_stim_param_setting.h"
#include "td_stim_param_table.h"

#include "td_private.h"
#include "stim_lib_type.h"

#include "td_debug.h"

void bt_state_req(uint8 *msg)
{
	bt_msg_res(BT_STATE_REQ, BT_MSG_RES_OK, NULL, 0);

	bt_state_ind();
}

void bt_mode_set_req(uint8 *msg)
{
	bt_msg_res_t rsp_code;
	uint8 mode;

	rsp_code = BT_MSG_RES_OK;

	if (msg == NULL)
		return;

	mode = msg[BT_MSG_DATA_IX];

	TD_DEBUG_PRINT(("mode = %d\n", mode));

	if (msg[BT_MSG_LEN_IX] == 1)
	{
		/* Handle MSG */
		td_Stim_Mode_Config_Update(mode);
	}
	else
	{
		rsp_code = BT_MSG_RES_INVALID_PARAM;
	}

	bt_msg_res(BT_MODE_SET_REQ, rsp_code, NULL, 0);
}

void bt_level_set_req(uint8 *msg)
{
	bt_msg_res_t rsp_code;
	uint8 level;

	rsp_code = BT_MSG_RES_OK;

	if (msg == NULL)
		return;

	level = msg[BT_MSG_DATA_IX];

	TD_DEBUG_PRINT(("level = %d\n", level));

	if (td_Stim_Cur_Mode_Get() == 0)
	{
		rsp_code = BT_MSG_RES_INVALID_STATUS;
	}
	else if (msg[BT_MSG_LEN_IX] == 1 && (TD_BT_MSG_LEVEL_MIN <= (int8) level && level <= TD_BT_MSG_LEVEL_MAX))
	{
		/* Handle MSG */
		td_Stim_Level_Config_Update(level);
	}
	else
	{
		rsp_code = BT_MSG_RES_INVALID_PARAM;
	}

	bt_msg_res(BT_LEVEL_SET_REQ, rsp_code, NULL, 0);

}

void bt_stimul_req(uint8 *msg)
{
	bt_msg_res_t rsp_code;
	uint8 req;

	rsp_code = BT_MSG_RES_OK;

	if (msg == NULL)
		return;

	req = msg[BT_MSG_DATA_IX];

	TD_DEBUG_PRINT(("req = %d\n", req));

	if (td_Stim_Cur_Mode_Get() == 0 && td_Stim_Cur_Detection_Level_Get() == 0)
	{
		td_Stim_FSM_Stop();
		rsp_code = BT_MSG_RES_INVALID_STATUS;
	}
	else if (msg[BT_MSG_LEN_IX] == 1 && (AUL_BT_MSG_STIMUL_STOP == req || req == AUL_BT_MSG_STIMUL_START))
	{
		/* Handle MSG */
		td_Stim_Control(req);
	}
	else
	{
		rsp_code = BT_MSG_RES_INVALID_PARAM;
	}

	bt_msg_res(BT_STIMUL_REQ, rsp_code, NULL, 0);

}

void bt_elect_detect_req(uint8 *msg)
{
	bt_msg_res_t rsp_code;
	uint8 level;

	rsp_code = BT_MSG_RES_OK;

	if (msg == NULL)
		return;

	level = msg[BT_MSG_DATA_IX];
	(void) level;

	if (td_Stim_Detection_LV_Check_Is_Active() == false && td_Stim_Is_Started())
	{
		rsp_code = BT_MSG_RES_INVALID_STATUS;
	}
	else if (msg[BT_MSG_LEN_IX] == 1 && (TD_BT_MSG_STIM_DETECTION_LEVEL_MIN <= (int8) level && level <= TD_BT_MSG_STIM_DETECTION_LEVEL_MAX))
	{
		/* Handle MSG */
		td_Stim_Detection_Check_Start(level);
	}
	else
	{
		rsp_code = BT_MSG_RES_INVALID_PARAM;
	}

	bt_msg_res(BT_ELECT_DETECT_REQ, rsp_code, NULL, 0);

}

void bt_man_mode_req(uint8 *msg)
{
	(void) msg;

	bt_msg_res_t rsp_code;
	uint8 i;

	if (msg == NULL)
	{
		return;
	}

	/* Check paramters */
	do
	{
		rsp_code = BT_MSG_RES_INVALID_PARAM;

		TD_MANUAL_PULSE_FREQ = 0;
		TD_MANUAL_PULSE_WIDTH = 0;
		TD_MANUAL_TARGET_VOLTAGE = 0;
		TD_MANUAL_TARGET_DAC = 0;
		TD_MANUAL_GP_OFF_TIME = 0;
		TD_MANUAL_GP_ON_TIME = 0;

		/* Length */
		if (msg[BT_MSG_LEN_IX] != 6)
		{
			break;
		}

		i = BT_MSG_DATA_IX;

		/* Frequency: 1byt. Range: 1 ~ 50.  Step: 1, 10, 20, ... 50 */
		TD_MANUAL_PULSE_FREQ = msg[i];

		if (1 > TD_MANUAL_PULSE_FREQ || TD_MANUAL_PULSE_FREQ > 50)
		{
			break;
		}

		if (TD_MANUAL_PULSE_FREQ >= 10)
		{
			TD_MANUAL_PULSE_FREQ = (TD_MANUAL_PULSE_FREQ / 10) * 10;
		}
		i++;

		/* Pulse width: size 2. Range: 100 ~ 1000. Step: 100 */
		TD_MANUAL_PULSE_WIDTH = msg[i] | ((uint16) msg[i + 1] << 8);

		if (100 > TD_MANUAL_PULSE_WIDTH || TD_MANUAL_PULSE_WIDTH > 1000)
		{
			break;
		}

		TD_MANUAL_PULSE_WIDTH = (TD_MANUAL_PULSE_WIDTH / 100) * 100;

		i += 2;

		/* Level: size 1. Range: 0 ~ 50. Step: 5 */
#ifdef STIM_LIB_EVKIT_CV
		TD_MANUAL_TARGET_VOLTAGE = msg[i];

		if (TD_MANUAL_TARGET_VOLTAGE > 50)
		{
			break;
		}
		TD_MANUAL_TARGET_VOLTAGE = (TD_MANUAL_TARGET_VOLTAGE / 5) * 5;
#endif
#ifdef STIM_LIB_EVKIT_CC
		TD_MANUAL_TARGET_DAC = msg[i];
		if (TD_MANUAL_TARGET_DAC > 255)
		{
			break;
		}
#endif
		i++;

		/* Group pulse: on/off each 1 size. Range: 1 ~ 60. Step: 1 */
		TD_MANUAL_GP_OFF_TIME = msg[i];
		i++;
		TD_MANUAL_GP_ON_TIME = msg[i];

		if (TD_MANUAL_GP_OFF_TIME > 60 || TD_MANUAL_GP_ON_TIME > 60)
		{
			break;
		}

		if (TD_MANUAL_GP_OFF_TIME == 0 || TD_MANUAL_GP_ON_TIME == 0)
		{
			/* Group pulse is disabled */
			TD_MANUAL_GP_OFF_TIME = 0;
			TD_MANUAL_GP_ON_TIME = 0;
		}

		rsp_code = BT_MSG_RES_OK;

	} while (false);

	if (rsp_code == BT_MSG_RES_OK)
	{
		TD_DEBUG_PRINT(("Params OK\n"));
	}

	TD_DEBUG_PRINT(("PULSE FREQ: %d\r\n", TD_MANUAL_PULSE_FREQ));
	TD_DEBUG_PRINT(("PULSE WIDTH: %d\r\n", TD_MANUAL_PULSE_WIDTH));
#ifdef STIM_LIB_EVKIT_CV
	TD_DEBUG_PRINT(("TARGET VOLTAGE: %d\r\n", TD_MANUAL_TARGET_VOLTAGE));
#else
	TD_DEBUG_PRINT(("TARGET DAC: %d\r\n", TD_MANUAL_TARGET_DAC));
#endif
	TD_DEBUG_PRINT(("GO IFF TIME: %d\r\n", TD_MANUAL_GP_OFF_TIME));
	TD_DEBUG_PRINT(("GP ON TIME: %d\r\n", TD_MANUAL_GP_ON_TIME));

	td_Stim_Manual_Mode_Start();
}

void bt_fw_ver_req(void)
{
	bt_msg_res(BT_FW_VER_REQ, BT_MSG_RES_OK, (uint8*) "FW_TEST_VER", strlen("FW_TEST_VER"));
}

void bt_stim_lib_ver_req(void)
{
	bt_msg_res(BT_STIM_LIB_VER_REQ, BT_MSG_RES_OK, (uint8*) "LIB_TEST_VER", strlen("LIB_TEST_VER"));
}

void bt_stim_stimul_type_req(void)
{
	uint8 type;
#ifdef STIM_LIB_EVKIT_CC
	type = BT_MSG_STIMUL_TYPE_CONST_CURR;
#endif
#ifdef STIM_LIB_EVKIT_CV
	type = BT_MSG_STIMUL_TYPE_CONST_VOLT;
#endif
	bt_msg_res(BT_STIM_STIMUL_TYPE_REQ, BT_MSG_RES_OK, &type, 1);
}

void bt_stim_sys_reset_req(void)
{
	bt_msg_res(BT_STIM_SYS_RESET_REQ, BT_MSG_RES_OK, NULL, 0);
}

void bt_stim_trg_cfg_req(uint8 *msg)
{
	(void) msg;

	/*
	 * TODO:
	 * CHECK PLS
	 * */
	bt_msg_res_t rsp_code;
	uint8 i;

	if (msg == NULL)
	{
		return;
	}

	/* Check paramters */
	do
	{
		rsp_code = BT_MSG_RES_INVALID_PARAM;

		TD_MANUAL_VOLT_PRESTART = false;
		TD_MANUAL_TRG_OUT_ENA = false;
		TD_MANUAL_TRG_OUT_ACT_POL = 1;
		TD_MANUAL_TRG_IN_ENA = false;
		TD_MANUAL_TRG_IN_ACT_POL = 1;
		TD_MANUAL_TRG_IN_TOGGLED = false;

		/* Length */
		if (msg[BT_MSG_LEN_IX] != 6)
		{
			break;
		}

		i = BT_MSG_DATA_IX;

		/* Voltage pre-start: 1byt. true : pre-start / false : do not pre-start :: 0 */
		TD_MANUAL_VOLT_PRESTART = msg[i];
		if (true != TD_MANUAL_VOLT_PRESTART && false != TD_MANUAL_VOLT_PRESTART)
		{
			break;
		}
		i++;

		/* Trigger Output Enable :: 1 */
		TD_MANUAL_TRG_OUT_ENA = msg[i];
		if (true != TD_MANUAL_TRG_OUT_ENA && false != TD_MANUAL_TRG_OUT_ENA)
		{
			break;
		}
		i++;

		/* Trigger Output Active Pol Setting :: 2 */
		TD_MANUAL_TRG_OUT_ACT_POL = msg[i];
		if (0 != TD_MANUAL_TRG_OUT_ACT_POL && 1 != TD_MANUAL_TRG_OUT_ACT_POL)
		{
			break;
		}
		i++;

		/* Trigger Input Enable :: 3*/
		TD_MANUAL_TRG_IN_ENA = msg[i];
		if (true != TD_MANUAL_TRG_IN_ENA && false != TD_MANUAL_TRG_IN_ENA)
		{
			break;
		}
		i++;

		/* Trigger Input Active Pol Setting :: 4 */
		TD_MANUAL_TRG_IN_ACT_POL = msg[i];
		if (0 != TD_MANUAL_TRG_IN_ACT_POL && 1 != TD_MANUAL_TRG_IN_ACT_POL)
		{
			break;
		}
		i++;

		/* Trigger Input Toggle Active Setting :: 5 */
		TD_MANUAL_TRG_IN_TOGGLED = msg[i];
		if (true != TD_MANUAL_TRG_IN_TOGGLED && false != TD_MANUAL_TRG_IN_TOGGLED)
		{
			break;
		}

		rsp_code = BT_MSG_RES_OK;

	} while (false);

	if (rsp_code == BT_MSG_RES_OK)
	{
		TD_DEBUG_PRINT(("Params OK\n"));
	}

	TD_DEBUG_PRINT(("Pre-Start: %d\r\n", TD_MANUAL_VOLT_PRESTART));
	TD_DEBUG_PRINT(("Trigger Output ENA: %d\r\n", TD_MANUAL_TRG_OUT_ENA));
	TD_DEBUG_PRINT(("Trigger Output Act POL: %d\r\n", TD_MANUAL_TRG_OUT_ACT_POL));
	TD_DEBUG_PRINT(("Trigger Input ENA: %d\r\n", TD_MANUAL_TRG_IN_ENA));
	TD_DEBUG_PRINT(("Trigger Input Act POL: %d\r\n", TD_MANUAL_TRG_IN_ACT_POL));
	TD_DEBUG_PRINT(("Trigger Input Toggled : %d\r\n", TD_MANUAL_TRG_IN_TOGGLED));

	td_Stim_Trigger_Config_Update();
}
