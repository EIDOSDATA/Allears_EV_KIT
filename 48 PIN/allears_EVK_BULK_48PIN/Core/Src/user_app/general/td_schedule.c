/*
 * td_schedule.c
 *
 *  Created on: 2023. 3. 8.
 *      Author: eidos
 */
#include "stim_lib.h"
#include "stim_lib_st_inc.h"
#include "stim_lib_type.h"

#include "td_schedule.h"
#include "td_btn.h"

#include "td_system_manager.h"
#include "td_stim_param_setting.h"
#include "td_stim_param_table.h"

#include "td_uart1.h"
#include "td_debug.h"
#include "td_private.h"
#include "bt_msg_private.h"

static uint8_t GP_SYS_STATE = TD_GP_STATE_MAX;
uint8_t gp_cur_stat = TD_GP_INIT;

/* TIMER COUNTER VALUE :: INTERRUPT SCHEDULER */
uint16_t gp_ctrl_cnt_tim16 = 0;
uint8_t gp_sequence_pos = 0;
uint16_t pwm_holding_tim = 0; // = (TD_CUR_MODE_FREQ_HOLDING_TIME * 10) - 1;

/* TIMER COUNTER VALUE :: POLLING SCHEDULER */
static uint32_t schdule_tick = 0;
/*
 * POLLING SCHEDULEER :: CALL BY MAIN.C
 * */
void td_Schedule(void)
{

	btMsg_rcvData_handle();

	if (HAL_GetTick() - schdule_tick >= TD_SCHED_HANDLE_PERIOD)
	{
#ifndef TD_GPIO_UNUSED
		td_Btn_Handle();
		td_Sys_Param_Update_Handle();
		td_Stim_Timeout_Handle();
#endif
		schdule_tick = HAL_GetTick();
	}
}

void td_Group_Pulse_Value_Setting(void)
{
	if (TD_MODE_SIZE == gp_sequence_pos)
	{
		gp_sequence_pos = 0;
	}

	/* MANUAL MODE PARAMETER SETTING */
	if (TD_STIM_CUR_MODE == TD_MANUAL_MODE)
	{
		/* MANUAL GP ON TIME */
		if (gp_sequence_pos == 0)
		{
			TD_CUR_MODE_FREQ = TD_MANUAL_PULSE_FREQ;
			TD_CUR_MODE_FREQ_HOLDING_TIME = TD_MANUAL_GP_ON_TIME;
		}
		/* MANUAL GP OFF TIME */
		else if (gp_sequence_pos == 1)
		{
			TD_CUR_MODE_FREQ = 0;
			TD_CUR_MODE_FREQ_HOLDING_TIME = TD_MANUAL_GP_OFF_TIME;
		}
	}
	/* NON MANUAL MODE */
	else if (TD_STIM_CUR_MODE != TD_MANUAL_MODE)
	{
		TD_MODE_FREQ_UPDATE(TD_STIM_MODE_GET_FREQ(TD_STIM_CUR_MODE, gp_sequence_pos));
		TD_MODE_FREQ_HOLDING_TIME_UPDATE(TD_STIM_MODE_GET_FREQ_HOLDING_TIME(TD_STIM_CUR_MODE, gp_sequence_pos));
	}

	ex_pulse_data.freq = TD_CUR_MODE_FREQ;
	stimLib_stimParameterChange(&ex_pulse_data);

	gp_sequence_pos++;
}

/*
 * GROUP PULSE CONTROL SCHEDULER :: CALL BY INTERRUPT SCHEDULER
 * */
void td_Group_Pulse_Mode_Control_Scheduler(void)
{
	/* TIME VALUE = 100ms * 10 >> 1ms * 1000 */
	if (td_GP_Mode_Is_Ready())
	{
		/*
		 * STIM PARAMETER UPDATE
		 * STATUS TIME OUT
		 * */
#ifdef DEBUG
#if 0
		TD_DEBUG_PRINT(("IN GP SCHEDULER : TD_STIM_CUR_MODE : %d\r\n",TD_STIM_CUR_MODE));
		TD_DEBUG_PRINT(("\r\n"));
#endif
#endif
		if (GP_SYS_STATE != gp_cur_stat || gp_ctrl_cnt_tim16 == pwm_holding_tim)
		{
			td_Group_Pulse_Value_Setting();
			if (TD_CUR_MODE_FREQ == 0)
			{
				pwm_holding_tim = (TD_CUR_MODE_FREQ_HOLDING_TIME * 10) + 2;
				gp_cur_stat = TD_GP_SLEEP;
			}
			else if (TD_CUR_MODE_FREQ != 0)
			{
				pwm_holding_tim = (TD_CUR_MODE_FREQ_HOLDING_TIME * 10) - 2;
				gp_cur_stat = TD_GP_AWAKE;
			}

			gp_ctrl_cnt_tim16 = 0;

			GP_SYS_STATE = gp_cur_stat;
		}

		/*
		 * STIM STOP
		 * */
		if (GP_SYS_STATE == TD_GP_SLEEP && gp_ctrl_cnt_tim16 == 0)
		{
			stimLib_stimPause();
			gp_ctrl_cnt_tim16 = 0;
		}

		/*
		 * STIM START
		 * */
		else if (GP_SYS_STATE == TD_GP_AWAKE && gp_ctrl_cnt_tim16 == 0)
		{
			stimLib_stimStart();
			gp_ctrl_cnt_tim16 = 0;
		}
		gp_ctrl_cnt_tim16++;
	}
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

