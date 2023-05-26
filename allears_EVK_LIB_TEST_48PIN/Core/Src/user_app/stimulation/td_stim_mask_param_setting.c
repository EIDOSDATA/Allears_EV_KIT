/*
 * td_stim_mask_param_setting.c
 *
 *  Created on: 2023. 3. 10.
 *      Author: eidos
 */
#include "stim_lib.h"
#include "stim_lib_st_inc.h"
#include "stim_lib_type.h"

#include "td_stim_param_table.h"
#include "td_stim_param_setting.h"

#include "td_sys_fsm_state.h"
#include "bt_msg_private.h"

#include "td_debug.h"

/*
 * STIMULATE PWM PULSE PARAMETER :: FREQ, PULSE WIDTH, DEGREE(Voltage or Current*(DAC)), Trigger Setting
 *
 * LIB CONTROL VALUE
 * */
stim_signal_cfg_t ex_pulse_data;
stim_trg_cfg_t ex_trg_data;

/*
 * BLE MANUAL MODE
 * 1. GROUP PULSE PARAMETER :: RAW STRUCT
 * 2. MANUAL CONTROL PULSE PARAM
 * 3. MANUAL CONTROL TRIGGER PARAM
 * */
td_stim_gpmode_ctrl_param_t ex_raw_gpmode_ctrl_param;
td_stim_manual_param_t ex_manual_param;
td_stim_trigger_param_t ex_man_trg_param;

typedef struct
{
	/* STIM MODE */
	uint8_t cur_mode;
	uint8_t prev_mode;

	/* STIM LEVEL */
	uint8_t cur_level;
	uint8_t prev_level;

	/* STIM DETECTION LEVEL */
	uint8_t cur_detect_level;
	uint8_t prev_detect_level;
	uint8_t detection_level; // DETECTION LEVEL GET :: REQUEST

	uint8_t active;
} td_stim_state_buff_t;

typedef struct
{
	bool timeout_started;
	uint16_t timeout_cnt;
} td_stim_timout_state_t;

td_stim_state_buff_t td_stim_state;
td_stim_timout_state_t td_stim_timeout_state;

/*
 * INIT
 * */
void td_Stim_Mask_Init(void)
{
	TD_STIM_CUR_MODE = 0;
	TD_STIM_PREV_MODE = 0;
	TD_STIM_CUR_LEVEL = 0;
	TD_STIM_PREV_LEVEL = 0;
	TD_STIM_ACTIVE = 0;
}

/*
 * PARAMETER CHECK FUNCTION :: PARAMETER
 * */
uint8_t td_Stim_Is_Started(void)
{
	return TD_STIM_ACTIVE;
}
uint8_t td_Stim_Cur_Mode_Get(void) // RETURN STIM STATE
{
	return TD_STIM_CUR_MODE;
}
uint8_t td_Stim_Cur_Level_Get(void) // RETURN MODE LEVEL
{
	return TD_STIM_CUR_LEVEL;
}
uint8_t td_Stim_Cur_Voltage_Get(void) // RETURN DAC LEVEL
{
	return ex_pulse_data.degree;
}
uint8_t td_GP_Mode_Is_Ready(void) // RETURN GROUP PULSE MODE FLAG
{
	uint8_t gp_ready_f = 0;

	if (td_Get_Sys_FSM_State() == TD_SYS_STATE_RUN && TD_RAW_GROUP_PULSE_F == 1)
	{
		gp_ready_f = 1;
	}
	else
	{
		gp_ready_f = 0;
	}

	return gp_ready_f;
}

/*
 * STIM DETECTION LEVEL CHECK :: PARAMETER
 * */
uint8_t td_Stim_Cur_Detection_Level_Get(void)
{
	return TD_STIM_CUR_DETECTOIN_LEVEL;
}

uint8_t td_Stim_Cur_Detection_State_Get(void)
{
#ifdef STIMDETECTION
	return ElecDetFlag;
#endif
	return 0;
}

uint8_t td_Stim_Detection_LV_Check_Is_Active(void)
{
	return (TD_STIM_CUR_DETECTOIN_LEVEL > 0);
}

uint8_t td_Stim_Detection_Level(void)
{
	return TD_STIM_DETECTION_LEVEL;
}

/*
 * MODE CONFIG DATA GET FUNCTION
 * */
void td_Stim_Mode_Config_Get(uint8_t mode)
{
	if (TD_STIM_MODE_CFG_TABLE == NULL)
	{
		return;
	}

	if (mode > TD_STIM_MODE_LAST)
	{
		mode = 0;
	}

	TD_STIM_MODE_CFG_TABLE->gp_mode_enable = TD_STIM_MODE_CFG_GP_ENABLE_GET(mode);
	TD_STIM_MODE_CFG_TABLE->gp_off_time = TD_STIM_MODE_CFG_GP_OFF_TIME_GET(mode);
	TD_STIM_MODE_CFG_TABLE->gp_on_time = TD_STIM_MODE_CFG_GP_ON_TIME_GET(mode);
}

/*
 * MODE CONFIG FUNCTION :: Action : BT_MODE_SET_REQ
 * */
void td_Stim_Mode_Config_Update(uint8_t mode)
{
	if (mode > TD_STIM_MODE_LAST || TD_STIM_CUR_MODE == mode)
	{
		TD_DEBUG_PRINT(("STIM MODE SET IS IGNORED: %d\r\n", mode));
		return;
	}

	/* CHANGE TO NEW MODE */
	TD_RAW_MODE_SET_F = TD_STIM_OP_NORMAL;
	TD_RAW_STIM_MODE = mode;

	/* APPLY PULSE FREQ */
	ex_pulse_data.freq = TD_STIM_MODE_CFG_PULSE_FREQ_GET(mode);

	/* APPLY GROUP PULSE DATA */
	TD_RAW_GROUP_PULSE_F = TD_STIM_MODE_CFG_GP_ENABLE_GET(mode);
	TD_RAW_GROUP_PULSE_DISABLE_TIME = TD_STIM_MODE_CFG_GP_OFF_TIME_GET(mode);
	TD_RAW_GROUP_PULSE_ENABLE_TIME = TD_STIM_MODE_CFG_GP_ON_TIME_GET(mode);

	/* RESET STIM LEVEL */
	TD_STIM_STATE_LEVEL_UPDATE(0);
	ex_pulse_data.pulse_width = TD_STIM_LEVEL_CFG_PW_GET(0);

#ifdef STIM_LIB_EVKIT_CV
	ex_pulse_data.degree = TD_STIM_LEVEL_CFG_VOLT_GET(0);
#endif
#ifdef STIM_LIB_EVKIT_CC
	ex_pulse_data.degree = TD_STIM_LEVEL_CFG_DAC_GET(0);
#endif

	TD_RAW_STIM_LEVEL = 0;

	/* UPDATE MODE */
	TD_STIM_STATE_MODE_UPDATE(mode);

	/* STIM STOP :: SYSTEM FSM */
	td_Set_Sys_FSM_State_Stop();
}

/*
 * LEVEL CONFIG GET FUNCTION
 * */
void td_Stim_Level_Config_Get(uint8_t level)
{
	if (TD_STIM_LEVEL_CFG_TABLE == NULL)
	{
		return;
	}

	if (level > TD_STIM_LEVEL_MAX)
	{
		level = 0;
	}

	TD_STIM_LEVEL_CFG_TABLE->puls_width = TD_STIM_LEVEL_CFG_PW_GET(level);
	TD_STIM_LEVEL_CFG_TABLE->output_target_voltage = TD_STIM_LEVEL_CFG_VOLT_GET(level);
	TD_STIM_LEVEL_CFG_TABLE->current_strength_step = TD_STIM_LEVEL_CFG_DAC_GET(level);
}

/*
 * LEVEL CONFIG FUNCTION :: Action : BT_LEVEL_SET_REQ
 * */
void td_Stim_Level_Config_Update(uint8_t level)
{
	/* Level 0 means to activate stimulation voltage output */
	if (level != 0 && (level > TD_STIM_LEVEL_MAX || TD_STIM_CUR_LEVEL == level))
	{
		TD_DEBUG_PRINT(("STIM LEVEL SET IS IGNORED: %d\r\n", level));
		return;
	}
	TD_RAW_STIM_LEVEL = level;
	ex_pulse_data.pulse_width = TD_STIM_LEVEL_CFG_PW_GET(level);

#ifdef STIM_LIB_EVKIT_CV
	ex_pulse_data.degree = TD_STIM_LEVEL_CFG_VOLT_GET(level);
#endif

#ifdef STIM_LIB_EVKIT_CC
	ex_pulse_data.degree = TD_STIM_LEVEL_CFG_DAC_GET(level);
#endif

	TD_RAW_PWM_CHANGE_F = 1;
	TD_RAW_MODE_SET_F = TD_STIM_OP_NORMAL;

	/* UPDATE LEVEL */
	TD_STIM_STATE_LEVEL_UPDATE(level);

	/*
	 * STIM LIB PARAMETER UPDATE SEQUENCE
	 * 1. stim Pause
	 * 2. session Stop
	 * 3. STIM Parameter Update
	 * 4. session ReStart
	 * 5. stim ReStart
	 * */
	stimLib_stimPause();
	stimLib_stimSessionStop();

	stimLib_stimSignalConfig(&ex_pulse_data);

	if (td_Get_Sys_FSM_State() == TD_SYS_STATE_RUN)
	{
		stimLib_stimSessionStart();
		stimLib_stimStart();
	}
}

/*
 * STIM DETECTION LEVEL CHECK START
 * */
void td_Stim_Detection_Check_Start(uint8_t level)
{
	TD_DEBUG_PRINT(("td_Stim_Detection_Check_Start: %d\r\n", level));

	/* Level 0 means stop electeric detect or reset parameters */
	if (level != 0 && (level > TD_STIM_ELDET_LEVEL_MAX || TD_STIM_CUR_DETECTOIN_LEVEL == level))
	{
		TD_DEBUG_PRINT(("STIM DETECTION IS IGNORED: %d\r\n", level));
		return;
	}

	if (level == 0)
	{
		td_Stim_Control(0);

		if (TD_STIM_PREV_ELDET_LEVEL != 0)
		{
			TD_STIM_DETECTION_LEVEL = td_Stim_Cur_Voltage_Get();
		}
	}
	else
	{
		TD_MANUAL_PULSE_FREQ = TD_STIM_ELDET_LEVEL_FREQ;
		TD_MANUAL_PULSE_WIDTH = TD_STIM_ELDET_LEVEL_PW;
		TD_MANUAL_TARGET_VOLTAGE = TD_STIM_ELDET_LEVEL_VOLT;
		TD_MANUAL_TARGET_DAC = TD_STIM_ELDET_LEVEL_DAC;
		TD_MANUAL_GP_OFF_TIME = 0;
		TD_MANUAL_GP_ON_TIME = 0;

		td_Stim_Manual_Mode_Start();

		TD_STIM_DETECTION_LEVEL = 0;
	}

	TD_STIM_STATE_ELDT_LEVEL_UPDATE(level);

	TD_DEBUG_PRINT(("STIM DETECTION LV: %d\r\n", TD_STIM_CUR_DETECTOIN_LEVEL));
}

/*
 * MANUAL MODE START
 * */
void td_Stim_Manual_Mode_Start(void)
{
	/* Change to new mode */
	TD_RAW_MODE_SET_F = TD_STIM_OP_NORMAL;
	TD_RAW_STIM_MODE = TD_MANUAL_MODE;

	/* Apply group pulse */
	if (TD_MANUAL_GP_OFF_TIME == 0 || TD_MANUAL_GP_ON_TIME == 0)
	{
		TD_RAW_GROUP_PULSE_F = 0;
		TD_RAW_GROUP_PULSE_DISABLE_TIME = 0;
		TD_RAW_GROUP_PULSE_ENABLE_TIME = 0;
	}
	else
	{
		TD_RAW_GROUP_PULSE_F = 1;
		TD_RAW_GROUP_PULSE_DISABLE_TIME = TD_MANUAL_GP_OFF_TIME;
		TD_RAW_GROUP_PULSE_ENABLE_TIME = TD_MANUAL_GP_ON_TIME;
	}

	/* APPLY PWM FREQ, PULSE WIDTH, STEP UP TARGET VOLTAGE, TARGET CURRENT */
	ex_pulse_data.freq = TD_MANUAL_PULSE_FREQ;
	ex_pulse_data.pulse_width = TD_MANUAL_PULSE_WIDTH;
#ifdef STIM_LIB_EVKIT_CV
	ex_pulse_data.degree = TD_MANUAL_TARGET_VOLTAGE;
#endif

#ifdef STIM_LIB_EVKIT_CC
	ex_pulse_data.degree = TD_MANUAL_TARGET_DAC;
#endif
	stimLib_stimSignalConfig(&ex_pulse_data);

	TD_RAW_PWM_CHANGE_F = 1;

	/* Update stimulation state */
	TD_STIM_ACTIVE_CHNAGE(TD_SYS_STATE_RUN);

	/* Reset current mode */
	TD_STIM_STATE_MODE_UPDATE(0);

	/* Reset current level */
	TD_STIM_STATE_LEVEL_UPDATE(0);
	TD_RAW_STIM_LEVEL = 0;

	/* STIM START :: SYSTEM FSM */
	td_Set_Sys_FSM_State_Start();
}

/*
 * Trigger MODE Setting
 * TODO:
 * Add in future
 * */
void td_Stim_Trigger_Config_Update(void)
{
	/*
	 * TODO:
	 * TEST PLS
	 * */
	/* MANUAL PULSE DATA SETTING */
	if (TD_MANUAL_PULSE_FREQ == 0 || TD_MANUAL_PULSE_WIDTH == 0 || TD_MANUAL_TARGET_VOLTAGE == 0)
	{
		ex_pulse_data.freq = 10;
		ex_pulse_data.pulse_width = 1000;

#ifdef STIM_LIB_EVKIT_CV
		ex_pulse_data.degree = 15;
#endif
#ifdef STIM_LIB_EVKIT_CC
		ex_pulse_data.degree = 1;
#endif
		stimLib_stimSignalConfig(&ex_pulse_data);
	}
	else
	{
		ex_pulse_data.freq = TD_MANUAL_PULSE_FREQ;
		ex_pulse_data.pulse_width = TD_MANUAL_PULSE_WIDTH;
#ifdef STIM_LIB_EVKIT_CV
		ex_pulse_data.degree = TD_MANUAL_TARGET_VOLTAGE;
#endif
#ifdef STIM_LIB_EVKIT_CC
			ex_pulse_data.degree = TD_MANUAL_TARGET_DAC;
#endif
	}
	stimLib_stimSignalConfig(&ex_pulse_data);

	/* TRIGGER SETTING */
	ex_trg_data.volt_prestart = TD_MANUAL_VOLT_PRESTART;
	ex_trg_data.trg_out_enable = TD_MANUAL_TRG_OUT_ENA;
	ex_trg_data.trg_out_active_pol = TD_MANUAL_TRG_OUT_ACT_POL;
	ex_trg_data.trg_in_enable = TD_MANUAL_TRG_IN_ENA;
	ex_trg_data.trg_in_active_pol = TD_MANUAL_TRG_IN_ACT_POL;
	ex_trg_data.trg_in_toggled = TD_MANUAL_TRG_IN_TOGGLED;

	stimLib_stimTriggerConfig(&ex_trg_data);
	td_Set_Sys_FSM_State_Start();
}

/*
 * BLE STIM PARAMETER CONTROL FUNCTION
 * */
void td_Stim_Control(uint8_t start)
{
	TD_DEBUG_PRINT(("td_Stim_Control: %d\r\n", start));

	/* IF STOPPING, ANYWAY STOP STIMUALTIONS */
	if (start != 0 && TD_STIM_ACTIVE == start)
	{
		TD_DEBUG_PRINT(("THE SAME STIM The same STIM state: %d\r\n", TD_STIM_ACTIVE));
		return;
	}

	if (start == 0)
	{
		/* STOP ALL ACTIVITIES */
		TD_DEBUG_PRINT(("STOP\r\n"));

		/* RESET ELECT DETECT */
		TD_STIM_DETECTION_LEVEL = 0; /* RESET CURRENT STIM LEVEL ALWAYS */

		if (td_Stim_Detection_LV_Check_Is_Active() == true)
		{
			TD_STIM_STATE_ELDT_LEVEL_UPDATE(0);
		}

		/* DEFAULT MODE SET IS 1 */
		TD_RAW_MODE_SET_F = TD_STIM_OP_NORMAL;

		/* RESET STIM LEVEL */
		TD_STIM_STATE_LEVEL_UPDATE(0);

		/* PARAMETER LOAD */
		ex_pulse_data.pulse_width = TD_STIM_LEVEL_CFG_PW_GET(0);
#ifdef STIM_LIB_EVKIT_CV
		ex_pulse_data.degree = TD_STIM_LEVEL_CFG_VOLT_GET(0);
#endif
#ifdef STIM_LIB_EVKIT_CC
		ex_pulse_data.degree = TD_STIM_LEVEL_CFG_DAC_GET(0);
#endif
		TD_RAW_STIM_LEVEL = 0;

		/* STIM STOP :: SYSTEM FSM */
		td_Set_Sys_FSM_State_Stop();
		stimLib_stimSignalConfig(&ex_pulse_data);

		/* GROUP PULSE SETTING */
		TD_RAW_GROUP_PULSE_F = 0;
		TD_RAW_GROUP_PULSE_DISABLE_TIME = 0;
		TD_RAW_GROUP_PULSE_ENABLE_TIME = 0;

		TD_RAW_STIM_MODE = 0;
		TD_STIM_STATE_MODE_UPDATE(0);
	}

	else if (start == 1)
	{
		TD_DEBUG_PRINT(("START\r\n"));

#if 0
		/*
		 * PARAMETER RESETTING
		 * MODE
		 * */
		ex_pulse_data.freq = TD_STIM_MODE_CFG_PULSE_FREQ_GET(TD_RAW_STIM_MODE);
		TD_RAW_GROUP_PULSE_F = TD_STIM_MODE_CFG_GP_ENABLE_GET(TD_RAW_STIM_MODE);
		TD_RAW_GROUP_PULSE_DISABLE_TIME = TD_STIM_MODE_CFG_GP_OFF_TIME_GET(TD_RAW_STIM_MODE);
		TD_RAW_GROUP_PULSE_ENABLE_TIME = TD_STIM_MODE_CFG_GP_ON_TIME_GET(TD_RAW_STIM_MODE);

		/*
		 * PARAMETER RESETTING
		 * LEVEL
		 * */
		ex_pulse_data.pulse_width = TD_STIM_LEVEL_CFG_PW_GET(TD_RAW_STIM_LEVEL);
#ifdef STIM_LIB_EVKIT_CV
		ex_pulse_data.degree = TD_STIM_LEVEL_CFG_VOLT_GET(TD_RAW_STIM_LEVEL);
#endif

#ifdef STIM_LIB_EVKIT_CC
		ex_pulse_data.degree = TD_STIM_LEVEL_CFG_DAC_GET(TD_RAW_STIM_LEVEL);
#endif

		stimLib_stimSignalConfig(&ex_pulse_data);
#endif
		/* STIM STOP :: SYSTEM FSM */
		td_Set_Sys_FSM_State_Start();
	}
	TD_STIM_ACTIVE_CHNAGE(start);
}

void td_Stim_FSM_Start(void)
{
	td_Set_Sys_FSM_State_Start();
}
void td_Stim_FSM_Stop(void)
{
	td_Set_Sys_FSM_State_Stop();
}

/*
 * PARAMETER CONTROL FUNCTION
 * */
void td_Stim_Param_Update_Handle(void)
{
	/* Send state change ind */
	if (TD_STIM_ACTIVE != td_Get_Sys_FSM_State() || TD_STIM_CUR_MODE != TD_RAW_STIM_MODE || TD_STIM_CUR_LEVEL != TD_RAW_STIM_LEVEL /*|| ElecDetEndFlag == 1*/)
	{
		TD_DEBUG_PRINT(("STIM STATE CHANGE\r\n"));

		TD_DEBUG_PRINT(("START: %d >> %d\r\n", TD_STIM_ACTIVE, td_Get_Sys_FSM_State()));
		TD_DEBUG_PRINT(("MODE: %d >> %d\r\n", TD_STIM_CUR_MODE, TD_RAW_STIM_MODE));
		TD_DEBUG_PRINT(("LEVLE: %d >> %d\r\n", TD_STIM_CUR_LEVEL, TD_RAW_STIM_LEVEL));
		/* TD_DEBUG_PRINT(("El Det: %d\n", ElecDetFlag)); */

		TD_STIM_ACTIVE_CHNAGE(td_Get_Sys_FSM_State());
		TD_STIM_STATE_MODE_UPDATE(TD_RAW_STIM_MODE);
		TD_STIM_STATE_LEVEL_UPDATE(TD_RAW_STIM_LEVEL);

		/*
		 if (ElecDetEndFlag == 1)
		 {
		 TD_STIM_STATE_ELDT_LEVEL_UPDATE(0);
		 }
		 */
		bt_state_ind();
	}
}

void td_Stim_Force_Stop(void)
{
	td_Stim_Control(0);
	bt_state_ind();
}

void td_Stim_Timeout_Ctrl(uint8_t start)
{
	if (TD_STIM_TIMEOUT_STARTED != start)
	{
		TD_STIM_TIMEOUT_CNT = 0;
		TD_STIM_TIMEOUT_STARTED = start;
		TD_DEBUG_PRINT(("STIM TIMEOUT: %d\r\n", TD_STIM_TIMEOUT_STARTED));
	}
}

void td_Stim_Timeout_Handle(void)
{
	if (TD_STIM_TIMEOUT_STARTED == true && TD_STIM_TIMEOUT_CNT <= TD_STIM_TIMEOUT_THRESHOLD_VALUE)
	{
		TD_STIM_TIMEOUT_CNT++;

		if (TD_STIM_TIMEOUT_CNT == TD_STIM_TIMEOUT_THRESHOLD_VALUE)
		{
			TD_DEBUG_PRINT(("STIM TIMEOUT: STOP\r\n"));
			td_Stim_Force_Stop();
		}
	}
}
