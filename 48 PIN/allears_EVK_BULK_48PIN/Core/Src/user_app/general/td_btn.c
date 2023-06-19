/*
 * td_btn.c
 *
 *  Created on: Oct 24, 2022
 *      Author: ECHO
 */
#include <td_system_manager.h>
#include "stim_lib.h"
#include "stim_lib_st_inc.h"
#include "stim_lib_type.h"

#include "td_btn.h"
#include "td_stim_param_setting.h"
#include "td_stim_param_table.h"

#include "td_debug.h"

/*
 * STEP UP TEST
 */
int td_volt_ctrl_pulse = 0;

td_btn_state_data_t td_start_btn_state;
td_btn_state_data_t td_stim_up_btn_state;
td_btn_state_data_t td_stim_down_btn_state;

bool td_Start_Btn_IsHandled(void)
{
	return TD_START_BTN_STATE_HANDLED;
}

void td_Start_Btn_Handled_Clear(void)
{
	TD_START_BTN_STATE_HANDLED = false;
}

void td_Start_Btn_HandleEnable(bool enable)
{
	TD_START_BTN_STATE_HANDLE_ENABLE = enable;
}

/*
 * START BTN ACTION
 * */
void td_Start_Btn_Action(void)
{
	bool start_pressed;
	start_pressed = TD_START_BTN_IS_PRESSED();

	if (start_pressed != TD_START_BTN_STATE_PRESSED)
	{
		TD_START_BTN_STATE_PRESSED = start_pressed;
#ifdef DEBUG
		TD_DEBUG_PRINT(("START BTN : %d\n", start_pressed));
#endif
		if (TD_START_BTN_STATE_PRESSED == false)
		{
		}
		else
		{
			/* BUTTON PRESSED, Send Signal >> FUNCTION td_Start_Btn_IsHandled >> td_fsm_state.c */
			TD_START_BTN_STATE_HANDLED = true;
			TD_START_BTN_STATE_HELD_TICK = 0;
		}
	}
	/* Check held */
	else if (TD_START_BTN_STATE_PRESSED == true)
	{
		if (TD_START_BTN_STATE_HELD_TICK == TD_BTN_HELD_TIME)
		{
#ifdef DEBUG
			TD_DEBUG_PRINT(("START BTN : %d\n", start_pressed));
#endif
			TD_START_BTN_STATE_HELD_TICK++;
		}
		else if (TD_START_BTN_STATE_HELD_TICK < TD_BTN_HELD_TIME)
		{
			TD_START_BTN_STATE_HELD_TICK++;
		}
		else
		{
			/* Ignored */
		}
	}
}

/*
 * STIM UP BTN ACTION
 * */
void td_StimUp_Btn_Action(void)
{
	bool up_pressed;
	up_pressed = TD_STIM_UP_BTN_IS_PRESSED();

	if (up_pressed != TD_STIM_UP_BTN_STATE_PRESSED)
	{
		TD_STIM_UP_BTN_STATE_PRESSED = up_pressed;
		TD_VOLTAGE_CTRL_PULSE = TIM1->CCR1;
#ifdef DEBUG
		TD_DEBUG_PRINT(("UP BTN : %d\n", up_pressed));
#endif
		if (TD_STIM_UP_BTN_STATE_PRESSED == false)
		{
		}
		else
		{
			/* BUTTON PRESSED, Send Signal >> STEP UP PULSE WIDTH CONTROL */
#ifdef TD_STEPUP_ADC_TUNNING
			TD_VOLTAGE_CTRL_PULSE++;
			if (TD_VOLTAGE_CTRL_PULSE == 0)
			{
				TD_VOLTAGE_CTRL_PULSE = 65535;
			}
			TIM1->CCR1 = TD_VOLTAGE_CTRL_PULSE;
			TD_DEBUG_PRINT(("STEP UP VPW : %d\n", TD_VOLTAGE_CTRL_PULSE));
#else
			TD_RAW_STIM_LEVEL++;
			if (TD_RAW_STIM_LEVEL > TD_STIM_LEVEL_MAX)
			{
				TD_RAW_STIM_LEVEL = TD_STIM_LEVEL_MAX;
			}
			td_Stim_Level_Config_Update(TD_RAW_STIM_LEVEL);
			TD_DEBUG_PRINT(("STIM LEVEL : %d\n", TD_RAW_STIM_LEVEL));
#endif
			TD_STIM_UP_BTN_STATE_HANDLED = true;
			TD_STIM_UP_BTN_STATE_HELD_TICK = 0;
		}
	}
	/* Check held */
	else if (TD_STIM_UP_BTN_STATE_PRESSED == true)
	{
		if (TD_STIM_UP_BTN_STATE_HELD_TICK == TD_BTN_HELD_TIME)
		{
#ifdef DEBUG
			TD_DEBUG_PRINT(("UP BTN : %d\n", up_pressed));
#endif
			TD_STIM_UP_BTN_STATE_HELD_TICK++;
		}
		else if (TD_STIM_UP_BTN_STATE_HELD_TICK < TD_BTN_HELD_TIME)
		{
			TD_STIM_UP_BTN_STATE_HELD_TICK++;
		}
		else
		{
			/* Ignored */
		}
	}
}

/*
 * STIM DOWN BTN ACTION
 * */
void td_StimDown_Btn_Action(void)
{
	bool down_pressed;
	down_pressed = TD_STIM_DOWN_BTN_IS_PRESSED();

	if (down_pressed != TD_STIM_DOWN_BTN_STATE_PRESSED)
	{
		TD_STIM_DOWN_BTN_STATE_PRESSED = down_pressed;
		TD_VOLTAGE_CTRL_PULSE = TIM1->CCR1;
#ifdef DEBUG
		TD_DEBUG_PRINT(("DOWN BTN : %d\n", down_pressed));
#endif
		if (TD_STIM_DOWN_BTN_STATE_PRESSED == false)
		{
		}
		else
		{
#ifdef TD_STEPUP_ADC_TUNNING
			/* BUTTON PRESSED, Send Signal >> STEP UP PULSE WIDTH CONTROL */
			TD_VOLTAGE_CTRL_PULSE--;
			if (TD_VOLTAGE_CTRL_PULSE == 65535)
			{
				TD_VOLTAGE_CTRL_PULSE = 0;
			}
			TIM1->CCR1 = TD_VOLTAGE_CTRL_PULSE;
			TD_DEBUG_PRINT(("STEP UP VPW : %d\n", TD_VOLTAGE_CTRL_PULSE));
#else
			TD_RAW_STIM_LEVEL--;
			if (TD_RAW_STIM_LEVEL == 255)
			{
				TD_RAW_STIM_LEVEL = 0;
			}
			td_Stim_Level_Config_Update(TD_RAW_STIM_LEVEL);
			TD_DEBUG_PRINT(("STIM LEVEL : %d\n", TD_RAW_STIM_LEVEL));
#endif
			TD_STIM_DOWN_BTN_STATE_HANDLED = true;
			TD_STIM_DOWN_BTN_STATE_HELD_TICK = 0;
		}
	}
	/* Check held */
	else if (TD_STIM_DOWN_BTN_STATE_PRESSED == true)
	{
		if (TD_STIM_DOWN_BTN_STATE_HELD_TICK == TD_BTN_HELD_TIME)
		{
#ifdef DEBUG
			TD_DEBUG_PRINT(("DOWN BTN : %d\n", down_pressed));
#endif
			TD_STIM_DOWN_BTN_STATE_HELD_TICK++;
		}
		else if (TD_STIM_DOWN_BTN_STATE_HELD_TICK < TD_BTN_HELD_TIME)
		{
			TD_STIM_DOWN_BTN_STATE_HELD_TICK++;
		}
		else
		{
			/* Ignored */
		}
	}
}

void td_Btn_Handle(void)
{
	td_Start_Btn_Action();
	td_StimUp_Btn_Action();
	td_StimDown_Btn_Action();
}
