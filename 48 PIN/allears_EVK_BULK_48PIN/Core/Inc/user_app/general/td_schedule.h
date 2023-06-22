/*
 * td_schedule.h
 *
 *  Created on: Oct 24, 2022
 *      Author: ECHO
 */

#ifndef INC_APP_TD_SCHEDULE_H_
#define INC_APP_TD_SCHEDULE_H_

#include "td_types.h"

#define TD_GP_SYS_STATE					exTd_stimGroupPulse_param.gp_sys_state
#define TD_GP_NEXT_STATE				exTd_stimGroupPulse_param.gp_next_state

#define TD_GP_CTRL_TIMER_CNT			exTd_stimGroupPulse_param.gp_ctrl_cnt
#define TD_GP_SEQUENCE_POS				exTd_stimGroupPulse_param.gp_sequence_pos
#define TD_GP_KEEPING_TIM				exTd_stimGroupPulse_param.pwm_keeping_tim

#define TD_SCHED_HANDLE_PERIOD			10 /* 10 ms */

/*
 * GROUP PULSE CONTROL TYPEDEF
 * */
typedef enum
{
	TD_GP_SLEEP = 0, TD_GP_AWAKE = 1, TD_GP_INIT = 2, TD_GP_STATE_MAX
} td_stim_group_pulse_state_t;

typedef struct
{
	uint8_t gp_sys_state;
	uint8_t gp_next_state;

	uint16_t gp_ctrl_cnt;
	uint8_t gp_sequence_pos;
	uint8_t pwm_keeping_tim;

} td_stim_sched_group_pulse_param_t;

/*
 * POLLING SCHEDULEER :: CALL BY MAIN.C
 * */
void td_runMainSchedule(void);

/*
 * INTERRUPT SCHEDULER
 * CONTROL TARGET >> GROUP PULSE, STEPUP CONTROL
 * */
/*
 * GP STATUS SETTING
 * */
void td_setGroupPulseValue(void);
void td_controlGroupPulseModeScheduler(void);
void td_resetGroupPulseSchedulerParameters(void);

/*
 * EXTERN
 * */
extern td_stim_sched_group_pulse_param_t exTd_stimGroupPulse_param;

#endif /* INC_APP_TD_SCHEDULE_H_ */
