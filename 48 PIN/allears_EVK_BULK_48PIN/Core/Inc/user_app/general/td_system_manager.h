#ifndef	INC_TD_SYSTEM_MANAGER_H_
#define INC_TD_SYSTEM_MANAGER_H_
#include <stdint.h>
#include <stddef.h>

#include <stdbool.h>

/*
 * SYSTEM STATE
 * */
#define TD_CUR_SYS_STATE								td_cur_sys_fsm_state.state
#define TD_NEXT_SYS_STATE								td_next_sys_fsm_state
#define TD_SYS_STATE_ACTIVE_CHNAGE(param)				{td_next_sys_fsm_state = param; td_Stim_Timeout_Ctrl(param);}

/*
 * TIME OUT
 * */
#define TD_STIM_TIMEOUT_STARTED							td_stim_timeout_state.timeout_started
#define TD_STIM_TIMEOUT_CNT								td_stim_timeout_state.timeout_cnt
#define TD_STIM_TIMEOUT_THRESHOLD_VALUE					(3600 * 100)		/* Sec */

typedef enum
{
	TD_SYS_STATE_IDLE = 0x00, TD_SYS_STATE_RUN, TD_SYS_STATE_INIT, TD_SYS_STATE_ERROR, TD_SYS_STATE_MAX
} td_sys_state_t;

/*
 * SYSTEM STATE
 * */
typedef struct
{
	td_sys_state_t state;
} td_sys_state_data_t;

/*
 * TIMEOUT STRUCT
 * */
typedef struct
{
	bool timeout_started;
	uint16_t timeout_cnt;
} td_stim_timout_state_t;

/*
 * SYSTEM FSM STATE
 * */
void td_Sys_FSM_State_Init(void);
td_sys_state_t td_Get_Sys_FSM_State(void);
void td_Set_Sys_FSM_State(td_sys_state_t state);

/* PARAMETER CONTROL FUNCTION :: SCHEDULER */
void td_Sys_Param_Update_Handle(void);
void td_Stim_Force_Stop(void);
void td_Stim_Timeout_Ctrl(uint8_t start);
void td_Stim_Timeout_Handle(void);

/*
 * EXTERN
 * */
extern td_sys_state_data_t td_cur_sys_fsm_state;
extern td_sys_state_t td_next_sys_fsm_state;
extern td_stim_timout_state_t td_stim_timeout_state;
#endif	/* INC_TD_SYSTEM_MANAGER_H_ */

