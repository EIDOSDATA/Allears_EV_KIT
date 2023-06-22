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

/* TIMER COUNTER VALUE :: INTERRUPT SCHEDULER */
td_stim_sched_group_pulse_param_t exTd_stimGroupPulse_param;

/* TIMER COUNTER VALUE :: POLLING SCHEDULER */
static uint32_t gSchdule_tick = 0;
/*
 * POLLING SCHEDULEER :: CALL BY MAIN.C
 * */
void td_runMainSchedule(void)
{
	btMsg_rcvData_handle();

	if (HAL_GetTick() - gSchdule_tick >= TD_SCHED_HANDLE_PERIOD)
	{
#ifndef TD_GPIO_UNUSED
		td_handleButton();
		td_handleSystemParamUpdate();
		td_handleStimulationTimeout();
#endif
		gSchdule_tick = HAL_GetTick();
	}
}

void td_setGroupPulseValue(void)
{
	if (TD_MODE_SIZE == TD_GP_SEQUENCE_POS)
	{
		TD_GP_SEQUENCE_POS = 0;
	}

	/* MANUAL MODE PARAMETER SETTING */
	if (TD_STIM_CUR_MODE == TD_MANUAL_MODE)
	{
		/* MANUAL GP ON TIME */
		if (TD_GP_SEQUENCE_POS == 0)
		{
			TD_CUR_MODE_FREQ = TD_MANUAL_PULSE_FREQ;
			TD_CUR_MODE_FREQ_HOLDING_TIME = TD_MANUAL_GP_ON_TIME;
		}
		/* MANUAL GP OFF TIME */
		else if (TD_GP_SEQUENCE_POS == 1)
		{
			TD_CUR_MODE_FREQ = 0;
			TD_CUR_MODE_FREQ_HOLDING_TIME = TD_MANUAL_GP_OFF_TIME;
		}
	}
	/* NON MANUAL MODE */
	else if (TD_STIM_CUR_MODE != TD_MANUAL_MODE)
	{
		TD_MODE_FREQ_UPDATE(TD_STIM_MODE_GET_FREQ(TD_STIM_CUR_MODE, TD_GP_SEQUENCE_POS));
		TD_MODE_FREQ_KEEPING_TIME_UPDATE(TD_STIM_MODE_GET_FREQ_HOLDING_TIME(TD_STIM_CUR_MODE, TD_GP_SEQUENCE_POS));
	}

	exTd_pulseCfg.freq = TD_CUR_MODE_FREQ;
	stimLib_stimParameterChange(&exTd_pulseCfg);

	TD_GP_SEQUENCE_POS++;
}

/*
 * GROUP PULSE CONTROL SCHEDULER :: CALL BY INTERRUPT SCHEDULER
 * */
void td_controlGroupPulseModeScheduler(void)
{
	/* TIME VALUE = 100ms * 10 >> 1ms * 1000 */
	if (td_isGroupPulseModeReady())
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
		if (TD_GP_SYS_STATE != TD_GP_NEXT_STATE || TD_GP_CTRL_TIMER_CNT == TD_GP_KEEPING_TIM)
		{
			td_setGroupPulseValue();
			if (TD_CUR_MODE_FREQ == 0)
			{
				TD_GP_KEEPING_TIM = (TD_CUR_MODE_FREQ_HOLDING_TIME * 10) + 2;
				TD_GP_NEXT_STATE = TD_GP_SLEEP;
			}
			else if (TD_CUR_MODE_FREQ != 0)
			{
				TD_GP_KEEPING_TIM = (TD_CUR_MODE_FREQ_HOLDING_TIME * 10) - 2;
				TD_GP_NEXT_STATE = TD_GP_AWAKE;
			}

			TD_GP_CTRL_TIMER_CNT = 0;

			TD_GP_SYS_STATE = TD_GP_NEXT_STATE;
		}

		/*
		 * STIM STOP
		 * */
		if (TD_GP_SYS_STATE == TD_GP_SLEEP && TD_GP_CTRL_TIMER_CNT == 0)
		{
			stimLib_stimPause();
			TD_GP_CTRL_TIMER_CNT = 0;
		}

		/*
		 * STIM START
		 * */
		else if (TD_GP_SYS_STATE == TD_GP_AWAKE && TD_GP_CTRL_TIMER_CNT == 0)
		{
			stimLib_stimStart();
			TD_GP_CTRL_TIMER_CNT = 0;
		}
		TD_GP_CTRL_TIMER_CNT++;
	}
}

void td_resetGroupPulseSchedulerParameters(void)
{
	TD_GP_SYS_STATE = TD_GP_STATE_MAX;
	TD_GP_NEXT_STATE = TD_GP_INIT;

	/* TIMER COUNTER VALUE :: INTERRUPT SCHEDULER */
	TD_GP_CTRL_TIMER_CNT = 0;
	TD_GP_SEQUENCE_POS = 0;
	TD_GP_KEEPING_TIM = 0;
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

