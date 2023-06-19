#include "stim_lib.h"
#include "stim_lib_st_inc.h"
#include "stim_lib_type.h"

#include "td_system_manager.h"
#include "td_stim_param_setting.h"
#include "td_stim_param_table.h"
#include "td_btn.h"
#include "bt_msg_private.h"
#include "bt_msg.h"

#include "td_debug.h"

extern TIM_HandleTypeDef htim16;

/* SYSTEM STATUS */
td_sys_state_data_t td_cur_sys_fsm_state;
td_sys_state_t td_next_sys_fsm_state = TD_SYS_STATE_IDLE;

/* TIMEOUT */
td_stim_timout_state_t td_stim_timeout_state;

/* while out code*/
void td_Sys_FSM_State_Init(void)
{
	TD_CUR_SYS_STATE = TD_SYS_STATE_MAX;
	td_Set_Sys_FSM_State(TD_SYS_STATE_INIT);
}

td_sys_state_t td_Get_Sys_FSM_State(void)
{
	return TD_CUR_SYS_STATE;
}

void td_Set_Sys_FSM_State(td_sys_state_t state)
{
	if (TD_CUR_SYS_STATE == state || state >= TD_SYS_STATE_MAX)
	{
		return;
	}

	switch (state)
	{
	case TD_SYS_STATE_INIT:
		break;
	case TD_SYS_STATE_IDLE:

		TD_DEBUG_PRINT(("STIM STOP\r\n"));
		TD_DEBUG_PRINT(("\r\n"));

		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

		/* GROUP PULSE SCHEDULER TIMER */
		HAL_TIM_Base_Stop_IT(&htim16);

		/* STIM PAUSE */
		stimLib_stimPause();

		/* SESSION STOP */
		stimLib_stimSessionStop();

		break;

	case TD_SYS_STATE_RUN:

		TD_DEBUG_PRINT(("STIM START\r\n"));
		TD_DEBUG_PRINT(("\r\n"));

		/* SESSION START */
		stimLib_stimSessionStart();

		/* STIM START */
		stimLib_stimStart();
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

		HAL_TIM_Base_Start_IT(&htim16);

		break;

	case TD_SYS_STATE_ERROR:
		TD_DEBUG_PRINT(("ERROR TD STATE: %d\n", state));
		break;

	default:
		/* Cannot be here */
		TD_DEBUG_PRINT(("UNKNOWN TD STATE: %d\n", state));
		break;
	}
	TD_CUR_SYS_STATE = state;
}

/*
 * PARAMETER CONTROL FUNCTION
 * */
void td_Sys_Param_Update_Handle(void)
{
	/* BUTTON PRESSED FLAG */
	if (td_Start_Btn_IsHandled() == true)
	{
		if (TD_NEXT_SYS_STATE == TD_SYS_STATE_IDLE)
		{
			TD_NEXT_SYS_STATE = TD_SYS_STATE_RUN;
		}
		else if (TD_NEXT_SYS_STATE == TD_SYS_STATE_RUN)
		{
			TD_NEXT_SYS_STATE = TD_SYS_STATE_IDLE;
		}
	}

	/* Send state change ind */
	if (TD_CUR_SYS_STATE != TD_NEXT_SYS_STATE || TD_STIM_PREV_MODE != TD_STIM_CUR_MODE || TD_STIM_PREV_LEVEL != TD_STIM_CUR_LEVEL /*|| ElecDetEndFlag == 1*/)
	{
		TD_DEBUG_PRINT(("STIM STATE CHANGE\r\n"));
		TD_DEBUG_PRINT(("START: %d >> %d\r\n", TD_CUR_SYS_STATE, TD_NEXT_SYS_STATE));
		TD_DEBUG_PRINT(("MODE: %d >> %d\r\n", TD_STIM_PREV_MODE, TD_STIM_CUR_MODE));
		TD_DEBUG_PRINT(("LEVLE: %d >> %d\r\n", TD_STIM_PREV_LEVEL, TD_STIM_CUR_LEVEL));
		/* TD_DEBUG_PRINT(("El Det: %d\n", ElecDetFlag)); */

		td_Set_Sys_FSM_State(TD_NEXT_SYS_STATE);
		td_Start_Btn_Handled_Clear();

		TD_SYS_STATE_ACTIVE_CHNAGE(TD_NEXT_SYS_STATE);
		TD_STIM_STATE_MODE_UPDATE(TD_STIM_CUR_MODE);
		TD_STIM_STATE_LEVEL_UPDATE(TD_STIM_CUR_LEVEL);

		/*
		 if (ElecDetEndFlag == 1)
		 {
		 TD_STIM_STATE_ELDT_LEVEL_UPDATE(0);
		 }
		 */
		bt_state_ind();
	}
}

void td_Stim_Force_Stop(void)
{
	td_Stim_Control(0);
	bt_state_ind();
}

void td_Stim_Timeout_Ctrl(uint8_t start)
{
	if (TD_STIM_TIMEOUT_STARTED != start)
	{
		TD_STIM_TIMEOUT_CNT = 0;
		TD_STIM_TIMEOUT_STARTED = start;
		TD_DEBUG_PRINT(("STIM TIMEOUT STARTED : %d\r\n", TD_STIM_TIMEOUT_STARTED));
	}
}

void td_Stim_Timeout_Handle(void)
{
	if (TD_STIM_TIMEOUT_STARTED == true && TD_STIM_TIMEOUT_CNT <= TD_STIM_TIMEOUT_THRESHOLD_VALUE)
	{
		TD_STIM_TIMEOUT_CNT++;

		if (TD_STIM_TIMEOUT_CNT == TD_STIM_TIMEOUT_THRESHOLD_VALUE)
		{
			TD_DEBUG_PRINT(("STIM TIMEOUT: STOP\r\n"));
			td_Stim_Force_Stop();
		}
	}
}
