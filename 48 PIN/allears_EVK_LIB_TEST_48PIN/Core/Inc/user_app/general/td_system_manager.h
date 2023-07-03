#ifndef	INC_TD_SYSTEM_MANAGER_H_
#define INC_TD_SYSTEM_MANAGER_H_

#include "td_types.h"
/*
 * SYSTEM STATE
 * */
#define TD_SYSTEM_UPDATE_F								td_cur_sys_fsm_state.system_update_f
#define TD_CUR_SYS_STATE								td_cur_sys_fsm_state.state
#define TD_NEXT_SYS_STATE								td_next_sys_fsm_state
#define TD_SYS_STATE_ACTIVE_CHNAGE(param)				{td_next_sys_fsm_state = param; td_startStimulationTimeoutControl(param);}

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
	uint8_t system_update_f;
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
void td_clearSystemControlParameters(void);
td_sys_state_t td_getSystemFSMState(void);
void td_setSystemFSMState(td_sys_state_t state);

/* PARAMETER CONTROL FUNCTION :: SCHEDULER */
void td_handleSystemParamUpdate(void);
void td_forceStopStimulation(void);
void td_startStimulationTimeoutControl(uint8_t start);
void td_handleStimulationTimeout(void);

/*
 * EXTERN
 * */
extern td_sys_state_data_t td_cur_sys_fsm_state;
extern td_sys_state_t td_next_sys_fsm_state;
extern td_stim_timout_state_t td_stim_timeout_state;
#endif	/* INC_TD_SYSTEM_MANAGER_H_ */

