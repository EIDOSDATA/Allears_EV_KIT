/*
 * stim_lib_current.c
 *
 *  Created on: 2023. 7. 10.
 *      Author: eidos
 */

#include <stdlib.h>

#include "stim_lib_current.h"
#include "stim_lib_volt.h"
#include "stim_lib_st_init.h"
#include "stim_lib_private.h"
#include "stim_lib_stim_cfg.h"

#include "td_debug.h"

/* DAC CONTROL :: VALUE and Counter */
int exStimLib_dacCtrl_value = 0;
int gStimLib_dacCtrl_cnt = 0;

/* CONTROL FLAG */
bool gStimLib_dacDataPrint_F = false;

void stimLib_dac_ctrlScheduler(void)
{
	STIM_LIB_DAC_CTRL_CNT++;
	/* DAC CONTROL */
	if (STIM_LIB_DAC_CTRL_CNT >= 1 && STIM_LIB_STEPUP_VOLTAGTE_STABILIZED_FLAG == true)
	{
		stimLib_dac_valControl();
		STIM_LIB_DAC_CTRL_CNT = 0;
		STIM_LIB_DAC_DATA_PRINT_FLAG = true;
	}
	/* STEP UP DATA PRINT */
#ifdef STIM_LIB_STEPUP_DATA_PRINT
	stimLib_dac_dataPrint();
#endif
}

void stimLib_dac_valControl(void)
{
	if (STIM_LIB_DAC_TARGET_VALUE > STIM_LIB_DAC_CTRL_VALUE)
	{
		STIM_LIB_DAC_CTRL_VALUE += 1;
		stimLib_dacctrl_Set();
	}
	else if (STIM_LIB_DAC_TARGET_VALUE < STIM_LIB_DAC_CTRL_VALUE)
	{
		STIM_LIB_DAC_CTRL_VALUE -= 1;
		stimLib_dacctrl_Set();
	}
	else
	{

	}
}

/*
 * STEP UP ADC DATA PRINT
 * */
void stimLib_dac_dataPrint(void)
{
	if (STIM_LIB_DAC_DATA_PRINT_FLAG == true)
	{
#ifdef STIM_LIB_EVKIT_CC
		TD_DEBUG_PRINT(("TARGET DAC : %d\n\n", STIM_LIB_DAC_TARGET_VALUE));
		TD_DEBUG_PRINT(("CURRENT DAC : %d\n\n", STIM_LIB_DAC_CTRL_VALUE));
#endif

	}
	STIM_LIB_DAC_DATA_PRINT_FLAG = false;

}

/*
 * DAC GPIO CONTROL
 * */
void stimLib_dacctrl_Set(void)
{
	stimLib_dacctrl_setRaw();
}

void stimLib_dacctrl_Off(void)
{
	stimLib_dacctrl_offRaw();
}

