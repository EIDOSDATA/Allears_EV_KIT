/*
 * td_stim_param.h
 *
 *  Created on: May 22, 2023
 *      Author: eidos
 */

#ifndef INC_USER_APP_STIMULATION_TD_STIM_PARAM_TABLE_H_
#define INC_USER_APP_STIMULATION_TD_STIM_PARAM_TABLE_H_

#include "td_types.h"
#include "td_stim_param_setting.h"

/*
 * CONST CURRENT STIMULATE LEVEL STRUCT
 * */
#define TD_STIM_LEVEL_CFG_TABLE						ex_cc_stim_levelcfg_table
#define TD_STIM_LEVEL_PW_GET(param)					ex_cc_stim_levelcfg_table[param].puls_width
#define TD_STIM_LEVEL_VOLT_GET(param)				ex_cc_stim_levelcfg_table[param].output_target_voltage
#define TD_STIM_LEVEL_DAC_GET(param)				ex_cc_stim_levelcfg_table[param].current_strength_step

/*
 * MODE SETTING PARAMETER
 * */
#define TD_STOP_MODE_SEQUENCE_SIZE						1
#define TD_NEEDLE_MODE_SEQUENCE_SIZE					1
#define TD_TAPPING_L_MODE_SEQUENCE_SIZE					2
#define TD_TAPPING_M_MODE_SEQUENCE_SIZE					10
#define TD_TAPPING_H_MODE_SEQUENCE_SIZE					10
#define TD_MASSAGE_L_MODE_SEQUENCE_SIZE					2
#define TD_MASSAGE_H_MODE_SEQUENCE_SIZE					2

#define TD_STIM_MODE_CFG_TABLE							ex_mode_config_table
#define TD_STIM_MODE_GET_STATE(mode)					ex_mode_config_table[mode].cur_mode
#define TD_STIM_MODE_GET_PULSE_PARAM(mode)				ex_mode_config_table[mode].pulse_param

#define TD_STIM_MODE_GET_SIZE(mode)						ex_mode_config_table[mode].pulse_param.size
#define TD_STIM_MODE_GET_FREQ(mode,addr)				ex_mode_config_table[mode].pulse_param.freq_and_time[addr].freq
#define TD_STIM_MODE_GET_FREQ_HOLDING_TIME(mode,addr)	ex_mode_config_table[mode].pulse_param.freq_and_time[addr].freq_holding_time

/*
 * STIMULATE MODE MAX SIZE
 * */
#define TD_STIM_MODE_NUM_MAX							7
#define TD_STIM_MODE_LAST								(TD_STIM_MODE_NUM_MAX - 1)

#define TD_STIM_COMBO_MODE_NUM_MAX						10
#define TD_STIM_COMBO_MODE_LAST							(TD_STIM_COMBO_MODE_NUM_MAX - 1)

#define TD_STIM_LEVEL_NUM_MAX							24
#define TD_STIM_LEVEL_MAX								(TD_STIM_LEVEL_NUM_MAX - 1)

#define TD_STIM_ELDET_LEVEL_NUM_MAX						17
#define TD_STIM_ELDET_LEVEL_MAX							(TD_STIM_ELDET_LEVEL_NUM_MAX - 1)

#define TD_STIM_ELDET_LEVEL_FREQ						10
#define TD_STIM_ELDET_LEVEL_PW							800
#define TD_STIM_ELDET_LEVEL_VOLT						40
#define TD_STIM_ELDET_LEVEL_DAC							1
#define TD_STIM_ELDET_LEVEL_LOWER_THRESHOLD_VOLT		35

/*
 * STIM BT MODE MAX SIZE
 * */
#define TD_BT_MSG_MODE_MIN								0
#define TD_BT_MSG_MODE_MAX								6

#define TD_BT_MSG_LEVEL_MIN								0
#define TD_BT_MSG_LEVEL_MAX								23

#define TD_BT_MSG_STIM_STOP								0
#define TD_BT_MSG_STIM_START							1

#define TD_BT_MSG_STIM_DETECTION_LEVEL_MIN				0
#define TD_BT_MSG_STIM_DETECTION_LEVEL_MAX				16

/*
 * STEP UP Application Control Param
 * file : td_btn.c
 * */
extern int td_volt_ctrl_pulse;
#define TD_VOLTAGE_CTRL_PULSE							td_volt_ctrl_pulse

/*
 * TYPE DEFINITION
 * */

/*
 * CONST CURRENT STIMULATE LEVEL STRUCT
 * */
typedef struct
{
	uint16_t puls_width; /* us */
	uint8_t output_target_voltage; /* #setVOL */
	uint8_t current_strength_step; /* #setDAC Select */
} td_stim_level_cfg_t;

/*
 * STIM MODE STRUCT TABLE
 * */
typedef struct
{
	/* Stimualtion signal frequency */
	uint8_t freq;
	/* Stimualtion signal pulse on time */
	uint16_t freq_holding_time;

} td_pulse_freq_time_t;

typedef struct
{
	/* Stimualtion Sequence Size */
	uint8_t size;
	/* Stimualtion signal frequency and time */
	const td_pulse_freq_time_t *freq_and_time;

} td_pulse_param_t;

typedef struct
{
	/* STIM MODE */
	td_stim_mode_t cur_mode;
	/* Combo Pulse Parameter */
	td_pulse_param_t pulse_param;

} td_mode_set_t;

/*
 * EXTERN
 * */
extern td_stim_level_cfg_t ex_cc_stim_levelcfg_table[TD_STIM_LEVEL_NUM_MAX];
extern const td_mode_set_t ex_mode_config_table[TD_STIM_MODE_NUM_MAX];

#endif /* INC_USER_APP_STIMULATION_TD_STIM_PARAM_TABLE_H_ */
