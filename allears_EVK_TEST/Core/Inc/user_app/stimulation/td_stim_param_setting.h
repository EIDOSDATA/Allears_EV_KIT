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

typedef enum
{
	td_stim_op_none = 0x00, td_stim_op_normal = 1, td_stim_op_elect_detect = 2,

	td_stim_op_max = 3
} td_stim_op_t;

/*
 * STIM MODE ENUM
 * */
typedef enum
{
	TD_STOP_MODE = 0,
	TD_NEEDLE_MODE = 1,
	TD_TAPPING_L_MODE = 2,
	TD_TAPPING_M_MODE = 3,
	TD_TAPPING_H_MODE = 4,
	TD_MASSAGE_L_MODE = 5,
	TD_MASSAGE_H_MODE = 6,
	/* BY SSI */
	TD_MANUAL_MODE = 0x80,

	TD_STIM_MODE_MAX = 7
} td_stim_mode_t;

/*
 * GROUP PULSE PARAMETER STRUCT :: RAW
 * */
typedef struct
{
	uint8_t raw_stim_start_f;

	uint8_t raw_mode_set_f;
	uint8_t raw_pwm_change_f;

	uint8_t raw_stim_mode;
	uint8_t raw_stim_level;

	uint8_t raw_group_pulse_f;
	uint16_t raw_gp_dis_time;
	uint16_t raw_gp_ena_time;
} td_stim_gpmode_ctrl_param_t;

extern td_stim_gpmode_ctrl_param_t ex_raw_gpmode_ctrl_param;

#define TD_RAW_STIM_START_F						ex_raw_gpmode_ctrl_param.raw_stim_start_f

#define TD_RAW_MODE_SET_F						ex_raw_gpmode_ctrl_param.raw_mode_set_f
#define TD_RAW_PWM_CHANGE_F						ex_raw_gpmode_ctrl_param.raw_pwm_change_f

#define TD_RAW_STIM_MODE						ex_raw_gpmode_ctrl_param.raw_stim_mode
#define TD_RAW_STIM_LEVEL						ex_raw_gpmode_ctrl_param.raw_stim_level

#define TD_RAW_GROUP_PULSE_F					ex_raw_gpmode_ctrl_param.raw_group_pulse_f
#define TD_RAW_GROUP_PULSE_DISABLE_TIME			ex_raw_gpmode_ctrl_param.raw_gp_dis_time
#define TD_RAW_GROUP_PULSE_ENABLE_TIME			ex_raw_gpmode_ctrl_param.raw_gp_ena_time

/*
 * --------------------------------------------------------------------
 * */

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
 * MANUAL MODE PARAMETER
 * */
typedef struct
{
	uint8_t freq; /* Hz */
	uint16_t pulse_width; /* us */
	uint8_t target_voltage; /* V */
	uint8_t target_dac; /* DAC Step */
	uint8_t gp_off_time; /* s */
	uint8_t gp_on_time; /* s */
} td_stim_manual_param_t;

extern td_stim_manual_param_t ex_manual_param;
#define TD_MANUAL_PULSE_FREQ							ex_manual_param.freq
#define TD_MANUAL_PULSE_WIDTH							ex_manual_param.pulse_width
#define TD_MANUAL_TARGET_VOLTAGE						ex_manual_param.target_voltage
#define TD_MANUAL_TARGET_DAC							ex_manual_param.target_dac
#define TD_MANUAL_GP_OFF_TIME							ex_manual_param.gp_off_time
#define TD_MANUAL_GP_ON_TIME							ex_manual_param.gp_on_time

/*
 * STIMULATE PWM PULSE PARAMETER :: FREQ, PULSE WIDTH, DEGREE(Voltage or Current*(DAC)), Trigger Setting
 * */
extern stim_signal_cfg_t ex_pulse_data;
extern stim_trg_cfg_t ex_trg_data;

/*
 * --------
 * */

/* STATE BUFF */
#define TD_STIM_CUR_MODE								td_stim_state.cur_mode
#define TD_STIM_PREV_MODE								td_stim_state.prev_mode
#define TD_STIM_CUR_LEVEL								td_stim_state.cur_level
#define TD_STIM_PREV_LEVEL								td_stim_state.prev_level
#define TD_STIM_CUR_DETECTOIN_LEVEL						td_stim_state.cur_detect_level
#define TD_STIM_PREV_ELDET_LEVEL						td_stim_state.prev_detect_level
#define TD_STIM_DETECTION_LEVEL							td_stim_state.detection_level // #setDAC VALUE SETTING
#define TD_STIM_ACTIVE									td_stim_state.active
#define TD_STIM_ACTIVE_CHNAGE(param)					{td_stim_state.active = param; td_Stim_Timeout_Ctrl(param);}

/* TIME OUT */
#define TD_STIM_TIMEOUT_STARTED							td_stim_timeout_state.timeout_started
#define TD_STIM_TIMEOUT_CNT								td_stim_timeout_state.timeout_cnt
#define TD_STIM_TIMEOUT_THRESHOLD_VALUE					(3600 * 100)		/* Sec */

/* UPDATE PARAMETER */
#define TD_STIM_STATE_MODE_UPDATE(param)				TD_STIM_PREV_MODE = TD_STIM_CUR_MODE; TD_STIM_CUR_MODE = param
#define TD_STIM_STATE_LEVEL_UPDATE(param)				TD_STIM_PREV_LEVEL = TD_STIM_CUR_LEVEL; TD_STIM_CUR_LEVEL = param
#define TD_STIM_STATE_ELDT_LEVEL_UPDATE(param)			TD_STIM_PREV_ELDET_LEVEL = TD_STIM_CUR_DETECTOIN_LEVEL; TD_STIM_CUR_DETECTOIN_LEVEL = param

/* INIT */
void td_Stim_Mask_Init(void);

/* PARAMETER CHECK FUNCTION */
uint8_t td_Stim_Is_Started(void);
uint8_t td_Stim_Cur_Mode_Get(void);
uint8_t td_Stim_Cur_Level_Get(void);
uint8_t td_Stim_Cur_Voltage_Get(void);
uint8_t td_GP_Mode_Is_Ready(void);

/* STIM DETECTION LEVEL CHECK :: PARAMETER */
uint8_t td_Stim_Cur_Detection_Level_Get(void);
uint8_t td_Stim_Cur_Detection_State_Get(void);
uint8_t td_Stim_Detection_LV_Check_Is_Active(void);
uint8_t td_Stim_Detection_Level(void);

/* MODE CONFIG FUNCTION :: Action : BT_MODE_SET_REQ */
void td_Stim_Mode_Config_Get(uint8_t mode);
void td_Stim_Mode_Config_Update(uint8_t mode);

/* LEVEL CONFIG FUNCTION :: Action : BT_LEVEL_SET_REQ */
void td_Stim_Level_Config_Get(uint8_t level);
void td_Stim_Level_Config_Update(uint8_t level);

/* STIM DETECTION LEVEL CHECK */
void td_Stim_Detection_Check_Start(uint8_t level);

/* BLE STIM PARAMETER CONTROL FUNCTION */
void td_Stim_Control(uint8_t start);

/* MANUAL MODE START */
void td_Stim_Manual_Mode_Start(void);

/* PARAMETER CONTROL FUNCTION :: SCHEDULER */
void td_Stim_Param_Update_Handle(void);
void td_Stim_Param_Haldle(void);
void td_Stim_Force_Stop(void);
void td_Stim_Timeout_Ctrl(uint8_t start);
void td_Stim_Timeout_Handle(void);

#endif /* INC_USER_APP_STIMULATION_TD_STIM_PARAM_SETTING_H_ */
