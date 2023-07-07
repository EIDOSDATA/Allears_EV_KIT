/*
 * stim_lib_current.h
 *
 *  Created on: 2023. 7. 10.
 *      Author: eidos
 */

#ifndef INC_INC_PRV_STIM_LIB_CURRENT_H_
#define INC_INC_PRV_STIM_LIB_CURRENT_H_

#include "stm32l4xx_hal.h"
#include "stim_lib_type.h"
#include "stim_lib_state.h"

/* DAC TARGET VALUE */
#ifdef STIM_LIB_EVKIT_CC
#define STIM_LIB_DAC_TARGET_VALUE				STIM_LIB_STATE_SIG_DEGREE
#endif
#ifdef STIM_LIB_EVKIT_CV
#define STIM_LIB_DAC_TARGET_VALUE				0
#endif

/* DAC CONTROL :: VALUE and Counter */
#define STIM_LIB_DAC_CTRL_VALUE					exStimLib_dacCtrl_value
#define STIM_LIB_DAC_CTRL_CNT					gStimLib_dacCtrl_cnt
extern int exStimLib_dacCtrl_value;

/* DAC CONTROL FLAG */
#define STIM_LIB_DAC_DATA_PRINT_FLAG			gStimLib_dacDataPrint_F

/* DAC CONTROL SCHEDULER */
void stimLib_dac_ctrlScheduler(void);

/* DAC VALUE CTRL */
void stimLib_dac_valControl(void);

/* CURRENT DAC DATA PRINT */
void stimLib_dac_dataPrint(void);

/* DAC GPIO CONTROL */
void stimLib_dacctrl_Set(void);
void stimLib_dacctrl_Off(void);

/*
 * stim_lib_current_st.c
 * */
void stimLib_dacctrl_setRaw(void);
void stimLib_dacctrl_offRaw(void);

#endif /* INC_INC_PRV_STIM_LIB_CURRENT_H_ */
