/*
 * td_stim_param.h
 *
 *  Created on: May 22, 2023
 *      Author: eidos
 */

#ifndef INC_USER_APP_STIMULATION_TD_STIM_PARAM_TABLE_H_
#define INC_USER_APP_STIMULATION_TD_STIM_PARAM_TABLE_H_

#include "td_types.h"

/*
 * MODE CONFIG STRUCT :: MODE FREQ
 * */
typedef struct
{
	uint8_t mode_pulse_freq;
} td_stim_mode_pulse_freq_t;
#define TD_STIM_MODE_CFG_PULSE_FREQ_GET(param)					ex_stim_mode_pulse_freq_table[param].mode_pulse_freq

/*
 * MODE CONFIG STRUCT :: GROUP PULSE
 * */
typedef struct
{
	uint8_t gp_mode_enable;
	uint8_t gp_off_time;
	uint8_t gp_on_time;
} td_stim_mode_cfg_group_pulse_t;
#define TD_STIM_MODE_CFG_TABLE							ex_stim_mode_cfg_gp_table
#define TD_STIM_MODE_CFG_GP_ENABLE_GET(param)			ex_stim_mode_cfg_gp_table[param].gp_mode_enable
#define TD_STIM_MODE_CFG_GP_OFF_TIME_GET(param)			ex_stim_mode_cfg_gp_table[param].gp_off_time
#define TD_STIM_MODE_CFG_GP_ON_TIME_GET(param)			ex_stim_mode_cfg_gp_table[param].gp_on_time

/*
 * CONST CURRENT STIMULATE LEVEL STRUCT
 * */
typedef struct
{
	uint16_t puls_width; /* us */
	uint8_t output_target_voltage; /* #setVOL */
	uint8_t current_strength_step; /* #setDAC Select */
} td_stim_level_cfg_t;
#define TD_STIM_LEVEL_CFG_TABLE							ex_cc_stim_levelcfg_table
#define TD_STIM_LEVEL_CFG_PW_GET(param)					ex_cc_stim_levelcfg_table[param].puls_width
#define TD_STIM_LEVEL_CFG_VOLT_GET(param)				ex_cc_stim_levelcfg_table[param].output_target_voltage
#define TD_STIM_LEVEL_CFG_DAC_GET(param)				ex_cc_stim_levelcfg_table[param].current_strength_step

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
 * */
extern int td_volt_ctrl_pulse;
#define TD_VOLTAGE_CTRL_PULSE				td_volt_ctrl_pulse

/*
 * PARAMETER TABLE
 * */
extern td_stim_mode_pulse_freq_t ex_stim_mode_pulse_freq_table[TD_STIM_MODE_NUM_MAX];
extern td_stim_mode_cfg_group_pulse_t ex_stim_mode_cfg_gp_table[TD_STIM_MODE_NUM_MAX];
extern td_stim_level_cfg_t ex_cc_stim_levelcfg_table[TD_STIM_LEVEL_NUM_MAX];

#endif /* INC_USER_APP_STIMULATION_TD_STIM_PARAM_TABLE_H_ */
