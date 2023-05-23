#include "stim_lib.h"
#include "stim_lib_type.h"

#include "td_sys_fsm_state.h"
#include "td_debug.h"

typedef struct
{
	td_sys_state_t state;
} td_sys_state_data_t;
td_sys_state_data_t td_sys_fsm_state;
td_sys_state_t cur_state = TD_SYS_STATE_IDLE;

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

void td_Set_Sys_FSM_State_Start_FORCE()
{
	TD_CUR_SYS_STATE = TD_SYS_STATE_RUN;
	cur_state = TD_SYS_STATE_RUN;
}
void td_Set_Sys_FSM_State_Stop_FORCE()
{
	TD_CUR_SYS_STATE = TD_SYS_STATE_IDLE;
	cur_state = TD_SYS_STATE_IDLE;
}

void td_Set_Sys_FSM_State_Start()
{
	cur_state = TD_SYS_STATE_RUN;
}

void td_Set_Sys_FSM_State_Stop()
{
	cur_state = TD_SYS_STATE_IDLE;
}

void td_Sys_FSM_State_Handle(void)
{
	/* BUTTON PRESSED FLAG */
#if 0
	if (td_Btn_IsHandled() == true)
	{
		if (cur_state == TD_SYS_STATE_IDLE)
		{
			cur_state = TD_SYS_STATE_RUN;
		}
		else if (cur_state == TD_SYS_STATE_RUN)
		{
			cur_state = TD_SYS_STATE_IDLE;
		}
	}
#endif

	if (TD_CUR_SYS_STATE != cur_state)
	{
		td_Set_Sys_FSM_State(cur_state);
#if 0
		td_Btn_Handled_Clear();
#endif
	}
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

		/* STIM PAUSE */
		stimLib_stimPause();

		/* SESSION STOP */
		stimLib_stimSessionStop();

	case TD_SYS_STATE_RUN:

		/* SESSION START */
		stimLib_stimSessionStart();

		/* STIM START */
		stimLib_stimStart();

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
