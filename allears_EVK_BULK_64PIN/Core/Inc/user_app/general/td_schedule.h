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
void td_Set_GP_State_Awake(void);
void td_Set_GP_State_Sleep(void);
uint8_t td_Get_Current_GP_State(void);

void td_Group_Pulse_Mode_Control_Scheduler(void);

#endif /* INC_APP_TD_SCHEDULE_H_ */
