/*
 * td_stim_param_setting.h
 *
 *  Created on: May 22, 2023
 *      Author: eidos
 */

#ifndef INC_USER_APP_STIMULATION_TD_STIM_PARAM_SETTING_H_
#define INC_USER_APP_STIMULATION_TD_STIM_PARAM_SETTING_H_

#include "stim_lib.h"
#include "stim_lib_st_inc.h"
#include "stim_lib_type.h"

#include "td_types.h"
/*
 * APP CONTROL PULSE PARAMETER STRUCT
 * */
#define TD_RAW_STIM_MODE								exTd_stimControl_param.raw_stim_mode
#define TD_RAW_STIM_LEVEL								exTd_stimControl_param.raw_stim_level

#define TD_MODE_SIZE									exTd_stimControl_param.app_mode_size

#define TD_STIM_CUR_MODE								exTd_stimControl_param.app_cur_mode
#define TD_STIM_PREV_MODE								exTd_stimControl_param.app_prev_mode

#define TD_CUR_MODE_FREQ								exTd_stimControl_param.app_cur_mode_freq
#define TD_PREV_MODE_FREQ								exTd_stimControl_param.app_prev_mode_freq

#define TD_CUR_MODE_FREQ_HOLDING_TIME					exTd_stimControl_param.app_cur_mode_freq_holding_time
#define TD_PREV_MODE_FREQ_HOLDING_TIME					exTd_stimControl_param.app_prev_mode_freq_holding_time

#define TD_STIM_LEVEL_UPDATE_ENABLE						exTd_stimControl_param.app_level_update_ena
#define TD_STIM_CUR_LEVEL								exTd_stimControl_param.app_cur_level
#define TD_STIM_PREV_LEVEL								exTd_stimControl_param.app_prev_level

#define TD_STIM_CUR_DETECTOIN_LEVEL						exTd_stimControl_param.app_cur_detect_level
#define TD_STIM_PREV_ELDET_LEVEL						exTd_stimControl_param.app_prev_detect_level
#define TD_STIM_DETECTION_LEVEL							exTd_stimControl_param.app_detection_level

/*
 * UPDATE PARAMETER
 * */
/* GROUP PULSE MODE PARAMETER */
#define TD_MODE_FREQ_UPDATE(param)						TD_PREV_MODE_FREQ = TD_CUR_MODE_FREQ; TD_CUR_MODE_FREQ = param
#define TD_MODE_FREQ_KEEPING_TIME_UPDATE(param)			TD_PREV_MODE_FREQ_HOLDING_TIME = TD_CUR_MODE_FREQ_HOLDING_TIME; TD_CUR_MODE_FREQ_HOLDING_TIME = param

/* APPLICATION PARAMETER */
#define TD_STIM_STATE_MODE_UPDATE(param)				TD_STIM_PREV_MODE = TD_STIM_CUR_MODE; TD_STIM_CUR_MODE = param
#define TD_STIM_STATE_LEVEL_UPDATE(param)				TD_STIM_PREV_LEVEL = TD_STIM_CUR_LEVEL; TD_STIM_CUR_LEVEL = param
#define TD_STIM_STATE_ELDT_LEVEL_UPDATE(param)			TD_STIM_PREV_ELDET_LEVEL = TD_STIM_CUR_DETECTOIN_LEVEL; TD_STIM_CUR_DETECTOIN_LEVEL = param

/*
 * ELEC DETECTE STRUCT
 * */

/*
 * MANUAL MODE PULSE PARAMETER
 * */
#define TD_MANUAL_PULSE_FREQ							exTd_manual_param.freq
#define TD_MANUAL_PULSE_WIDTH							exTd_manual_param.pulse_width
#define TD_MANUAL_TARGET_VOLTAGE						exTd_manual_param.target_voltage
#define TD_MANUAL_TARGET_DAC							exTd_manual_param.target_dac
#define TD_MANUAL_GP_OFF_TIME							exTd_manual_param.gp_off_time
#define TD_MANUAL_GP_ON_TIME							exTd_manual_param.gp_on_time
#define TD_MANUAL_STIM_DECTION_F						exTd_manual_param.stim_detection_f

/*
 * MANUAL MODE TRIGGER PARAMETER
 * */
#define TD_MANUAL_VOLT_PRESTART							exTd_trg_param.volt_prestart
#define TD_MANUAL_TRG_OUT_ENA							exTd_trg_param.trg_out_enable
#define TD_MANUAL_TRG_OUT_ACT_POL						exTd_trg_param.trg_out_active_pol
#define TD_MANUAL_TRG_IN_ENA							exTd_trg_param.trg_in_enable
#define TD_MANUAL_TRG_IN_ACT_POL						exTd_trg_param.trg_in_active_pol
#define TD_MANUAL_TRG_IN_TOGGLED						exTd_trg_param.trg_in_toggled

typedef enum
{
	TD_STIM_OP_NONE = 0x00, TD_STIM_OP_NORMAL, TD_STIM_OP_ELECT_DETECT, TD_STIM_OP_MAX
} td_stim_op_t;

/*
 * STIM MODE ENUM
 * */
typedef enum
{
	TD_STOP_MODE = 0x00,
	TD_NEEDLE_MODE = 1,
	TD_TAPPING_L_MODE = 2,
	TD_TAPPING_M_MODE = 3,
	TD_TAPPING_H_MODE = 4,
	TD_MASSAGE_L_MODE = 5,
	TD_MASSAGE_H_MODE = 6,
	TD_STIM_MODE_MAX = 7,

	/*
	 * EXTENDED MODE
	 * */
	TD_MANUAL_MODE = 0x80,
	TD_TRIGGER_MODE = 0xFF
} td_stim_mode_state_t;

/*
 * APP CONTROL PULSE PARAMETER STRUCT
 * */
typedef struct
{
	/*
	 * CHECK PLS
	 * */
	uint8_t raw_stim_mode;
	uint8_t raw_stim_level;

	/* MODE */
	uint8_t app_mode_size;

	uint8_t app_cur_mode;
	uint8_t app_prev_mode;

	/* ABOUT MODE PARAMETER */
	uint8_t app_cur_mode_freq;
	uint8_t app_prev_mode_freq;

	uint16_t app_cur_mode_freq_holding_time;
	uint16_t app_prev_mode_freq_holding_time;

	/* LEVEL */
	uint8_t app_level_update_ena;
	uint8_t app_cur_level;
	uint8_t app_prev_level;

	/* STIM DETECTION LEVEL */
	uint8_t app_cur_detect_level;
	uint8_t app_prev_detect_level;
	uint8_t app_detection_level; // DETECTION LEVEL GET :: REQUEST
} td_stim_app_ctrl_param_t;

/*
 * ELEC DETECTE STRUCT
 * */
typedef struct
{
	uint8_t freq; /* Hz */
	uint16_t puls_width; /* us */
	uint8_t voltage; /* V */
} td_stim_eldet_cfg_t;

/*
 * MANUAL MODE PULSE PARAMETER
 * */
typedef struct
{
	uint8_t freq; /* Hz */
	uint16_t pulse_width; /* us */
	uint8_t target_voltage; /* V */
	uint8_t target_dac; /* DAC Step */
	uint8_t gp_off_time; /* s */
	uint8_t gp_on_time; /* s */

	uint8_t stim_detection_f;
} td_stim_manual_param_t;

/*
 * MANUAL MODE TRIGGER PARAMETER
 * */
typedef struct
{
	bool volt_prestart; /* Hz */
	bool trg_out_enable; /* us */
	uint8_t trg_out_active_pol; /* V */
	bool trg_in_enable; /* DAC Step */
	uint8_t trg_in_active_pol; /* s */
	bool trg_in_toggled; /* s */
} td_stim_trigger_param_t;

/*
 * EXTERN
 * */
extern td_stim_app_ctrl_param_t exTd_stimControl_param;
extern td_stim_manual_param_t exTd_manual_param;
extern td_stim_trigger_param_t exTd_trg_param;

extern stim_signal_cfg_t exTd_pulseCfg;
extern stim_trg_cfg_t exTd_triggerCfg;

/*
 * STIM CONTROL FUNCTION
 * */
/* STIM PARAMETER INIT */
void td_initStimulationControlParameters(void);

/* PARAMETER CHECK FUNCTION */
uint8_t td_isStimulationStarted(void);
uint8_t td_getCurrentStimulationMode(void);
uint8_t td_getCurrentStimulationLevel(void);
uint8_t td_getCurrentStimulationVoltage(void);
uint8_t td_isGroupPulseModeReady(void);

/* STIM DETECTION LEVEL CHECK :: PARAMETER */
uint8_t td_getCurrentStimDetectionLevel(void);
uint8_t td_getCurrentStimDetectionState(void);
uint8_t td_isStimDetectionLevelActive(void);
uint8_t td_getStimDetectionLevel(void);

/* MODE CONFIG FUNCTION :: Action : BT_MODE_SET_REQ */
void td_getStimModeConfiguration(uint8_t mode);
void td_configureStimulationMode(uint8_t mode);

/* LEVEL CONFIG FUNCTION :: Action : BT_LEVEL_SET_REQ */
void td_getStimLevelConfiguration(uint8_t level);
void td_configureStimLevels(uint8_t level);

/* STIM DETECTION LEVEL CHECK */
void td_startStimDetectionCheck(uint8_t level);

/* MANUAL MODE START */
void td_startManualStimMode(void);

/* Trigger Mode Setting */
void td_updateStimTriggerConfiguration(void);

/* BLE STIM PARAMETER CONTROL FUNCTION */
void td_controlStimulation(uint8_t start);

#endif /* INC_USER_APP_STIMULATION_TD_STIM_PARAM_SETTING_H_ */
