/*
 * td_schedule.h
 *
 *  Created on: Oct 24, 2022
 *      Author: ECHO
 */

#ifndef INC_APP_TD_SCHEDULE_H_
#define INC_APP_TD_SCHEDULE_H_

#include "td_types.h"

#define TD_SCHED_HANDLE_PERIOD			10 /* 10 ms */

typedef enum
{
	TD_GP_SLEEP = 0, TD_GP_AWAKE = 1, TD_GP_INIT = 2, TD_GP_STATE_MAX
} td_stim_group_pulse_state_t;

/*
 * POLLING SCHEDULEER :: CALL BY MAIN.C
 * */
void td_Schedule(void);

/*
 * INTERRUPT SCHEDULER
 * CONTROL TARGET >> GROUP PULSE, STEPUP CONTROL
 * */
/*
 * GP STATUS SETTING
 * */
void td_Group_Pulse_Value_Setting(void);
void td_Group_Pulse_Mode_Control_Scheduler(void);

#endif /* INC_APP_TD_SCHEDULE_H_ */
