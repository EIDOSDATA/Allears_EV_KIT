/*
 * td_stim_param_table.c
 *
 *  Created on: Mar 9, 2023
 *      Author: eidos
 */
#include "td_stim_param_table.h"

/*
 * MODE CONFIG STRUCT :: GROUP PULSE
 * */
td_stim_mode_pulse_freq_t ex_stim_mode_pulse_freq_table[TD_STIM_MODE_NUM_MAX] =
{
#if 1
		/* Mode 0 ~ 6 */
		{ 0 }, // MODE 0 >> TD_STOP_MODE
		{ 30 }, // MODE 1 >> TD_NEEDLE_MODE
		{ 10 }, // MODE2 >> TD_NEEDLE_MODE
		{ 3 }, // MODE 3 >> TD_TAPPING_M_MODE
		{ 4 }, // MODE 4 >> TD_TAPPING_H_MODE
		{ 5 }, // MODE 5 >> TD_MASSAGE_L_MODE
		{ 6 } // MODE6 >> TD_MASSAGE_H_MODE
#endif
};

td_stim_mode_cfg_group_pulse_t ex_stim_mode_cfg_gp_table[TD_STIM_MODE_NUM_MAX] =
{
/* Mode 0 ~ 6 */
#if 1
		/*{GP ENABLE, GP OFF TIME, GP ON TIME}*/
		{ 0, 0, 0 }, // MODE 0 >> TD_STOP_MODE
		{ 0, 0, 0 }, // MODE 1 >> TD_NEEDLE_MODE
		{ 1, 1, 3 }, // MODE 2 >> TD_TAPPING_L_MODE
		{ 0, 0, 0 }, // MODE 3 >> TD_TAPPING_M_MODE
		{ 0, 0, 0 }, // MODE 4 >> TD_TAPPING_H_MODE
		{ 0, 0, 0 }, // MODE 5 >> TD_MASSAGE_L_MODE
		{ 1, 2, 5 } // MODE6 >> TD_MASSAGE_H_MODE
#else
		{ 0, 0, 0 }, // MODE 0 >> TD_STOP_MODE
		{ 0, 0, 0 }, // MODE 1 >> TD_NEEDLE_MODE
		{ 0, 0, 0 }, // MODE 2 >> TD_TAPPING_L_MODE
		{ 0, 0, 0 }, // MODE 3 >> TD_TAPPING_M_MODE
		{ 0, 0, 0 }, // MODE 4 >> TD_TAPPING_H_MODE
		{ 0, 0, 0 }, // MODE 5 >> TD_MASSAGE_L_MODE
		{ 1, 2, 5 } // MODE6 >> TD_MASSAGE_H_MODE
#endif
};

#ifdef STIM_LIB_EVKIT_CC
td_stim_level_cfg_t ex_cc_stim_levelcfg_table[TD_STIM_LEVEL_NUM_MAX] =
{
/* {PULSE WIDTH, VOLTAGE, DAC}*/
/*
 * The voltage is fixed in the library, but is put in the application for explicit purposes.
 * */
/* Level 0 ~ 5 */
{ 100, 0, 0 },
{ 100, 40, 1 },
{ 100, 40, 2 },
{ 100, 40, 3 },
{ 150, 40, 4 },
{ 200, 40, 5 },

/* Level 6 ~ 8 */
{ 250, 40, 6 },
{ 300, 40, 7 },
{ 350, 40, 8 },

/* Level 9 ~ 11 */
{ 400, 40, 9 },
{ 450, 40, 10 },
{ 500, 40, 11 },

/* Level 12 ~ 14 */
{ 550, 40, 12 },
{ 600, 40, 13 },
{ 650, 40, 14 },

/* Level 15 ~ 17 */
{ 700, 40, 15 },
{ 750, 40, 15 },
{ 800, 40, 15 },

/* Level 18 ~ 20 */
{ 850, 40, 15 },
{ 900, 40, 15 },
{ 950, 40, 15 },

/* Level 21 ~ 23 */
{ 1000, 40, 15 },
{ 1000, 40, 15 },
{ 1000, 40, 15 } };
#endif

#ifdef STIM_LIB_EVKIT_CV
td_stim_level_cfg_t ex_cc_stim_levelcfg_table[TD_STIM_LEVEL_NUM_MAX] =
{
/* {PULSE WIDTH, VOLTAGE, DAC}*/
/*
 * The DAC Value is unused in the library(CV), but is put in the application for explicit purposes.
 * */
/* Level 0 ~ 5 */
{ 100, 0, 0 },
{ 100, 15, 0 },
{ 300, 15, 0 },
{ 500, 15, 0 },
{ 700, 15, 0 },
{ 100, 20, 0 },

/* Level 6 ~ 8 */
{ 300, 20, 0 },
{ 500, 20, 0 },
{ 700, 20, 0 },

/* Level 9 ~ 11 */
{ 300, 25, 0 },
{ 500, 25, 0 },
{ 700, 25, 0 },

/* Level 12 ~ 14 */
{ 300, 30, 0 },
{ 500, 30, 0 },
{ 700, 30, 0 },

/* Level 15 ~ 17 */
{ 300, 35, 0 },
{ 500, 35, 0 },
{ 700, 35, 0 },

/* Level 18 ~ 20 */
{ 300, 40, 0 },
{ 500, 40, 0 },
{ 700, 40, 0 },

/* Level 21 ~ 23 */
{ 300, 45, 0 },
{ 500, 45, 0 },
{ 700, 45, 0 } };
#endif
