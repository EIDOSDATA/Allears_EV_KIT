/*
 * td_stim_mask_param_setting.c
 *
 *  Created on: 2023. 3. 10.
 *      Author: eidos
 */
#include "main.h"

#include "stim_lib.h"
#include "stim_lib_st_inc.h"
#include "stim_lib_type.h"

#include "td_system_manager.h"
#include "td_stim_param_table.h"
#include "td_stim_param_setting.h"

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
td_stim_app_ctrl_param_t ex_stim_ctrl_param;
td_stim_manual_param_t ex_manual_param;
td_stim_trigger_param_t ex_man_trg_param;

/*
 * STIM PARAMETER INIT
 * */
void td_Stim_Ctrk_Param_Init(void)
{
	TD_RAW_STIM_MODE = TD_STOP_MODE;
	TD_RAW_STIM_LEVEL = 0;
	TD_MODE_SIZE = 0;
	TD_STIM_CUR_MODE = TD_STOP_MODE;
	TD_STIM_PREV_MODE = TD_STOP_MODE;
	TD_CUR_MODE_FREQ = 0;
	TD_PREV_MODE_FREQ = 0;
	TD_CUR_MODE_FREQ_HOLDING_TIME = 0;
	TD_PREV_MODE_FREQ_HOLDING_TIME = 0;
	TD_STIM_LEVEL_UPDATE_ENABLE = 0;
	TD_STIM_CUR_LEVEL = 0;
	TD_STIM_PREV_LEVEL = 0;

	/* CHECK THIS */
	TD_STIM_CUR_DETECTOIN_LEVEL = 0;
	TD_STIM_PREV_ELDET_LEVEL = 0;
	TD_STIM_DETECTION_LEVEL = 0;
}

/*
 * PARAMETER CHECK FUNCTION :: PARAMETER
 * */
uint8_t td_Stim_Is_Started(void)
{
	return td_Get_Sys_FSM_State();
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

	if (td_Get_Sys_FSM_State() == TD_SYS_STATE_RUN && TD_STIM_CUR_MODE != TD_NEEDLE_MODE && TD_STIM_CUR_MODE != TD_STOP_MODE)
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

	/*
	 TD_STIM_MODE_CFG_TABLE->gp_mode_enable = TD_STIM_MODE_CFG_GP_ENABLE_GET(mode);
	 TD_STIM_MODE_CFG_TABLE->gp_off_time = TD_STIM_MODE_CFG_GP_OFF_TIME_GET(mode);
	 TD_STIM_MODE_CFG_TABLE->gp_on_time = TD_STIM_MODE_CFG_GP_ON_TIME_GET(mode);
	 */
}

/*
 * MODE CONFIG FUNCTION :: Action : BT_MODE_SET_REQ
 * */
void td_Stim_Mode_Config_Update(uint8_t mode)
{
	int addr = 0;

	if (mode > TD_STIM_MODE_LAST || TD_STIM_CUR_MODE == mode)
	{
		TD_DEBUG_PRINT(("STIM MODE SET IS IGNORED: %d\r\n", mode));
		return;
	}
	/* CHANGE TO NEW MODE */
	TD_RAW_STIM_MODE = mode;
	/*
	 * TODO:
	 * 1. APPLY MODE STATE
	 * 2. APPLY MODE SIZE
	 * 3. UPDATE MODE FREQ and FREQ HOLDING TIME
	 * 4. STIM LEVEL INIT
	 * */
	TD_STIM_CUR_MODE = TD_STIM_MODE_GET_STATE(mode);
	TD_MODE_SIZE = TD_STIM_MODE_GET_SIZE(mode);

	/*
	 * UPDATE MODE FREQ and FREQ HOLDING TIME
	 * */
	TD_MODE_FREQ_UPDATE(TD_STIM_MODE_GET_FREQ(mode, addr));
	TD_MODE_FREQ_HOLDING_TIME_UPDATE(TD_STIM_MODE_GET_FREQ_HOLDING_TIME(mode, addr));
	ex_pulse_data.freq = TD_CUR_MODE_FREQ;

	if (TD_MODE_SIZE > TD_NEEDLE_MODE_SEQUENCE_SIZE)
	{
		TD_DEBUG_PRINT(("GPMODE ENABL\r\n"));
	}

	/*
	 * RESET STIM LEVEL
	 * */
	TD_STIM_STATE_LEVEL_UPDATE(0);
	ex_pulse_data.pulse_width = TD_STIM_LEVEL_PW_GET(0);

#ifdef STIM_LIB_EVKIT_CV
	ex_pulse_data.degree = TD_STIM_LEVEL_VOLT_GET(0);
#endif
#ifdef STIM_LIB_EVKIT_CC
	ex_pulse_data.degree = TD_STIM_LEVEL_DAC_GET(0);
#endif

	TD_RAW_STIM_LEVEL = 0;

	/* UPDATE MODE */
	TD_STIM_STATE_MODE_UPDATE(mode);

	/* STIM STOP :: SYSTEM FSM */
	TD_SYS_STATE_ACTIVE_CHNAGE(TD_SYS_STATE_IDLE);
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

	TD_STIM_LEVEL_CFG_TABLE->puls_width = TD_STIM_LEVEL_PW_GET(level);
	TD_STIM_LEVEL_CFG_TABLE->output_target_voltage = TD_STIM_LEVEL_VOLT_GET(level);
	TD_STIM_LEVEL_CFG_TABLE->current_strength_step = TD_STIM_LEVEL_DAC_GET(level);
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

	if (TD_STIM_LEVEL_UPDATE_ENABLE == 1 || td_Get_Sys_FSM_State() == TD_SYS_STATE_IDLE)
	{
		TD_RAW_STIM_LEVEL = level;

		ex_pulse_data.pulse_width = TD_STIM_LEVEL_PW_GET(level);

#ifdef STIM_LIB_EVKIT_CV
		ex_pulse_data.degree = TD_STIM_LEVEL_VOLT_GET(level);
#endif

#ifdef STIM_LIB_EVKIT_CC
	ex_pulse_data.degree = TD_STIM_LEVEL_DAC_GET(level);
#endif

		/* UPDATE LEVEL */
		TD_STIM_STATE_LEVEL_UPDATE(level);

		/* Pulse Parameter Setting and Change */
		stimLib_stimIntensiveChange(&ex_pulse_data);

		TD_STIM_LEVEL_UPDATE_ENABLE = 0;
	}
	else
	{
		TD_DEBUG_PRINT(("LEVEL UPDATE DENIED: %d\r\n", TD_STIM_LEVEL_UPDATE_ENABLE));
		return;
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
	/* CHANGE TO NEW MODE :: SET DEFAULT */
	TD_STIM_CUR_MODE = TD_STOP_MODE;

	/* APPLY GROUP PULSE */
	if (TD_MANUAL_GP_OFF_TIME == 0 || TD_MANUAL_GP_ON_TIME == 0)
	{
		TD_STIM_CUR_MODE = TD_NEEDLE_MODE;
		TD_MODE_SIZE = 1;
	}
	else
	{
		/* MANUAL MODE >> GROUP PULSE MODE */
		TD_STIM_CUR_MODE = TD_MANUAL_MODE;
		TD_MODE_SIZE = 2;
		TD_CUR_MODE_FREQ_HOLDING_TIME = TD_MANUAL_GP_ON_TIME;
	}

	/* APPLY PWM FREQ, PULSE WIDTH, STEP UP TARGET VOLTAGE, TARGET CURRENT */
	TD_MODE_FREQ_UPDATE(TD_MANUAL_PULSE_FREQ); /* TD_CUR_MODE_FREQ = TD_MANUAL_PULSE_FREQ */
	TD_MODE_FREQ_HOLDING_TIME_UPDATE(TD_MANUAL_GP_ON_TIME); /* TD_CUR_MODE_FREQ_HOLDING_TIME = TD_MANUAL_GP_ON_TIME */

	ex_pulse_data.freq = TD_MANUAL_PULSE_FREQ;
	ex_pulse_data.pulse_width = TD_MANUAL_PULSE_WIDTH;
#ifdef STIM_LIB_EVKIT_CV
	ex_pulse_data.degree = TD_MANUAL_TARGET_VOLTAGE;
#endif

#ifdef STIM_LIB_EVKIT_CC
	ex_pulse_data.degree = TD_MANUAL_TARGET_DAC;
#endif
	stimLib_stimSignalConfig(&ex_pulse_data);

	/* SET MODE */
	TD_STIM_STATE_MODE_UPDATE(TD_STIM_CUR_MODE);

	/* RESET LEVEL */
	TD_STIM_STATE_LEVEL_UPDATE(0);
	TD_RAW_STIM_LEVEL = 0;

	/* STIM START :: SYSTEM FSM */
	TD_SYS_STATE_ACTIVE_CHNAGE(TD_SYS_STATE_RUN);
}

/*
 * Trigger MODE Setting
 * */
void td_Stim_Trigger_Config_Update(void)
{
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
	TD_SYS_STATE_ACTIVE_CHNAGE(TD_SYS_STATE_RUN);
}

/*
 * BLE STIM PARAMETER CONTROL FUNCTION
 * */
void td_Stim_Control(uint8_t start)
{
	TD_DEBUG_PRINT(("td_Stim_Control: %d\r\n", start));

	/* IF STOPPING, ANYWAY STOP STIMUALTIONS */
	if (start != 0 && TD_CUR_SYS_STATE == start)
	{
		TD_DEBUG_PRINT(("THE SAME STIM The same STIM state: %d\r\n", TD_CUR_SYS_STATE));
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

		/* RESET STIM LEVEL */
		TD_STIM_STATE_LEVEL_UPDATE(0);

		/* PARAMETER LOAD */
		ex_pulse_data.pulse_width = TD_STIM_LEVEL_PW_GET(0);
#ifdef STIM_LIB_EVKIT_CV
		ex_pulse_data.degree = TD_STIM_LEVEL_VOLT_GET(0);
#endif
#ifdef STIM_LIB_EVKIT_CC
		ex_pulse_data.degree = TD_STIM_LEVEL_DAC_GET(0);
#endif
		TD_RAW_STIM_LEVEL = 0;

		/* STIM STOP :: SYSTEM FSM */
		TD_SYS_STATE_ACTIVE_CHNAGE(TD_SYS_STATE_IDLE);
		stimLib_stimSignalConfig(&ex_pulse_data);

		/* MODE SETTING */
		TD_STIM_CUR_MODE = TD_STOP_MODE;
		TD_STIM_STATE_MODE_UPDATE(TD_STIM_CUR_MODE);
	}

	else if (start == 1)
	{
		TD_DEBUG_PRINT(("START\r\n"));

		/* STIM START :: SYSTEM FSM */
		TD_SYS_STATE_ACTIVE_CHNAGE(TD_SYS_STATE_RUN);
	}
}

