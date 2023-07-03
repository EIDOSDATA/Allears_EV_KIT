/*
 * td_stim_param_table.c
 *
 *  Created on: Mar 9, 2023
 *      Author: eidos
 */
#include "td_stim_param_table.h"

/*
 * STIM MODE PARAMETER TABLE
 * */
/* MODE 0 :: NEEDLE MODE PULSE AND TIME */
const td_stim_freq_t td_stop_mode_freq_param[TD_STOP_MODE_SEQUENCE_SIZE] =
{
	/* FREQ, FREQ KEEPING TIME */
	{ 0, 0 }
};

/* MODE 1 :: NEEDLE MODE PULSE AND TIME */
const td_stim_freq_t td_needle_mode_freq_param[TD_NEEDLE_MODE_SEQUENCE_SIZE] =
{
	/* FREQ, FREQ KEEPING TIME */
	{ 10, 0 }
};

/* MODE 2 :: TAPPING L MODE PULSE AND TIME */
const td_stim_freq_t td_tapping_l_mode_freq_param[TD_TAPPING_L_MODE_SEQUENCE_SIZE] =
{
	/* FREQ, FREQ KEEPING TIME */
	/*
	 * 5Hz 3sec >> 0Hz 1sec
	 * 0Hz : OFF
	 * */
	{ 5, 3 }, /* ON TIME */
	{ 0, 1 }, /* OFF TIME */
};

/* MODE 3 :: TAPPING M MODE PULSE AND TIME */
const td_stim_freq_t td_tapping_m_mode_freq_param[TD_TAPPING_M_MODE_SEQUENCE_SIZE] =
{
	/* FREQ, FREQ KEEPING TIME */
	{ 5, 1 }, /* ON TIME */
	{ 0, 1 }, /* OFF TIME */

	{ 5, 2 },
	{ 0, 1 },

	{ 10, 1 },
	{ 0, 1 },

	{ 5, 2 },
	{ 0, 1 },

	{ 10, 2 },
	{ 0, 1 }
};

/* MODE 4 :: TAPPING H MODE PULSE AND TIME */
const td_stim_freq_t td_tapping_h_mode_freq_param[TD_TAPPING_H_MODE_SEQUENCE_SIZE] =
{
	/* FREQ, FREQ KEEPING TIME */
	{ 5, 4 }, /* ON TIME */
	{ 0, 1 }, /* OFF TIME */

	{ 5, 2 },
	{ 0, 1 },

	{ 10, 2 },
	{ 0, 1 },

	{ 20, 2 },
	{ 0, 1 },

	{ 10, 2 },
	{ 0, 1 }
};

/* MODE 5 :: MASSAGE L MODE PULSE AND TIME */
const td_stim_freq_t td_massage_l_mode_freq_param[TD_MASSAGE_L_MODE_SEQUENCE_SIZE] =
{
	/* FREQ, FREQ KEEPING TIME */
	{ 30, 3 },
	{ 5, 2 }
};

/* MODE 6 :: MASSAGE H MODE PULSE AND TIME */
const td_stim_freq_t td_massage_h_mode_freq_param[TD_MASSAGE_H_MODE_SEQUENCE_SIZE] =
{
	/* FREQ, FREQ KEEPING TIME */
	{ 30, 5 },
	{ 0, 2 }
};

const td_stim_freq_group_t td_pulse_param_table[TD_STIM_MODE_NUM_MAX] =
{
	{ TD_STOP_MODE_SEQUENCE_SIZE, td_stop_mode_freq_param },				/* mode 0 */
	{ TD_NEEDLE_MODE_SEQUENCE_SIZE, td_needle_mode_freq_param },			/* mode 1 */
	{ TD_TAPPING_L_MODE_SEQUENCE_SIZE, td_tapping_l_mode_freq_param },		/* mode 2 */
	{ TD_TAPPING_M_MODE_SEQUENCE_SIZE, td_tapping_m_mode_freq_param },		/* mode 3 */
	{ TD_TAPPING_H_MODE_SEQUENCE_SIZE, td_tapping_h_mode_freq_param },		/* mode 4 */
	{ TD_MASSAGE_L_MODE_SEQUENCE_SIZE, td_massage_l_mode_freq_param },		/* mode 5 */
	{ TD_MASSAGE_H_MODE_SEQUENCE_SIZE, td_massage_h_mode_freq_param }		/* mode 6 */
};

const td_stim_mode_t exTdConst_StimModeCfg_table[TD_STIM_MODE_NUM_MAX] =
{
	{ TD_STOP_MODE, td_pulse_param_table[TD_STOP_MODE] },
	{ TD_NEEDLE_MODE, td_pulse_param_table[TD_NEEDLE_MODE] },
	{ TD_TAPPING_L_MODE, td_pulse_param_table[TD_TAPPING_L_MODE] },
	{ TD_TAPPING_M_MODE, td_pulse_param_table[TD_TAPPING_M_MODE] },
	{ TD_TAPPING_H_MODE, td_pulse_param_table[TD_TAPPING_H_MODE] },
	{ TD_MASSAGE_L_MODE, td_pulse_param_table[TD_MASSAGE_L_MODE] },
	{ TD_MASSAGE_H_MODE, td_pulse_param_table[TD_MASSAGE_H_MODE] }
};

#ifdef STIM_LIB_EVKIT_CC
td_stim_level_cfg_t exTd_StimLevelCfg_table[TD_STIM_LEVEL_NUM_MAX] =
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
	{ 1000, 40, 15 }
};
#endif

#ifdef STIM_LIB_EVKIT_CV
td_stim_level_cfg_t exTd_StimLevelCfg_table[TD_STIM_LEVEL_NUM_MAX] =
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
	{ 700, 45, 0 }
};
#endif

