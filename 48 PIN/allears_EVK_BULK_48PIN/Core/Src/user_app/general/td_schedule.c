/*
 * td_schedule.c
 *
 *  Created on: 2023. 3. 8.
 *      Author: eidos
 */

#include "stim_lib.h"

#include "td_schedule.h"
#include "td_btn.h"
#include "td_stim_param_setting.h"
#include "td_sys_fsm_state.h"
#include "td_uart1.h"
#include "td_debug.h"
#include "td_private.h"
#include "bt_msg_private.h"

typedef enum
{
	td_gp_sleep = 0, td_gp_awake = 1, td_gp_init = 2, td_gp_state_max
} td_stim_group_pulse_state_t;
uint8_t gp_stat = td_gp_init;

/* TIMER COUNTER VALUE :: INTERRUPT SCHEDULER */
uint8_t stepup_feedback_cnt_tim6 = 0;
uint16_t gp_ctrl_cnt_tim16 = 1;

/* TIMER COUNTER VALUE :: POLLING SCHEDULER */
static uint32_t schdule_tick = 0;

/*
 * GP STATUS SETTING
 * */
void td_Set_GP_State_Awake(void)
{
	gp_stat = td_gp_awake;
}

void td_Set_GP_State_Sleep(void)
{
	gp_stat = td_gp_sleep;
}

uint8_t td_Get_Current_GP_State(void)
{
	return gp_stat;
}

/*
 * POLLING SCHEDULEER :: CALL BY MAIN.C
 * */
void td_Schedule(void)
{

	btMsg_rcvData_handle();

	if (HAL_GetTick() - schdule_tick >= TD_SCHED_HANDLE_PERIOD)
	{
#ifndef TD_GPIO_UNUSED
		td_Sys_FSM_State_Handle();
		td_Btn_Handle();
		//td_Led_Handle();

		/* STIM */
		td_Stim_Param_Update_Handle();
		td_Stim_Timeout_Handle();
#endif
		schdule_tick = HAL_GetTick();
	}
}

/*
 * GROUP PULSE CONTROL SCHEDULER :: CALL BY INTERRUPT SCHEDULER
 * */
void td_Group_Pulse_Mode_Control_Scheduler(void)
{
	/* TIME VALUE = 100ms * 10 >> 1ms * 1000 */
	uint32_t pwm_disable_tim = (TD_RAW_GROUP_PULSE_DISABLE_TIME * 10) - 1;
	uint32_t pwm_enable_tim = (TD_RAW_GROUP_PULSE_ENABLE_TIME * 10);

#if 1
	/* CHANGE Group Pulse MODE */
	if (td_GP_Mode_Is_Ready())
	{
		/* STATUS TIME OUT */
		if (td_Get_Current_GP_State() == td_gp_init)
		{
			//td_Stim_Stop();
			gp_ctrl_cnt_tim16 = 0;
			gp_stat = td_gp_awake;
		}

		/* IF :: STATUS SLEEP >> AWAKE */
		if (td_Get_Current_GP_State() == td_gp_sleep && gp_ctrl_cnt_tim16 == pwm_disable_tim)
		{
			//TIM2->CCER = 0x1112;
			stimLib_stimStart();
			gp_stat = td_gp_awake;
			gp_ctrl_cnt_tim16 = 0;
		}

		/* IF :: STATUS AWAKE >> SLEEP */
		else if (td_Get_Current_GP_State() == td_gp_awake && gp_ctrl_cnt_tim16 == pwm_enable_tim)
		{
			//TIM2->CCER = 0x0000;
			stimLib_stimPause();
			gp_stat = td_gp_sleep;
			gp_ctrl_cnt_tim16 = 0;
		}
		gp_ctrl_cnt_tim16++;
	}
#endif
}

#if 0
/*********** PULSE CONTROL TIMER STATUS CALLBACK ***********/
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2)
	{
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			TD_SHELL_PRINT(("DMA CMPLT 1\n"));
		}

		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			td_Non_Conv_ADC2_Buff_Read();
			//td_ADC_PeakDetection();
			//td_PeakDetection_ADC_Data_Print();
		}

		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
			TD_SHELL_PRINT(("DMA CMPLT 4\n"));
		}
	}
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2)
	{
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			TD_SHELL_PRINT(("DMA HALF CMPLT 1\n"));
		}

		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
			TD_SHELL_PRINT(("DMA HALF CMPLT 4\n"));
		}
	}
}

void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2)
	{
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			TD_SHELL_PRINT(("DMA ERROR: TIM2_CH1\n"));
		}

		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
			TD_SHELL_PRINT(("DMA ERROR: TIM2_CH4\n"));
		}
	}
}
/*********** END OF PULSE CONTROL TIMER STATUS CALLBACK ***********/
#endif

