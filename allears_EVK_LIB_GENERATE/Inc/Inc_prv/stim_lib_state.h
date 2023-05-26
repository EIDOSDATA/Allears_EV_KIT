#ifndef	STIM_LIB_STATE_H

#define STIM_LIB_STATE_H

#include "stim_lib_private.h"

/* Stimulation signal config */
#define STIM_LIB_STATE_SIG						exStimLib_state.signal_cfg
#define STIM_LIB_STATE_SIG_FREQ					exStimLib_state.signal_cfg.freq
#define STIM_LIB_STATE_SIG_PW					exStimLib_state.signal_cfg.pulse_width
#define STIM_LIB_STATE_SIG_DEGREE				exStimLib_state.signal_cfg.degree
#define STIM_LIB_STATE_SIG_PEAK_DET_CALLBACK	exStimLib_state.signal_cfg.peak_detect_callback

/* Stimulation trigger config */
#define STIM_LIB_STATE_TRG						exStimLib_state.trg_cfg
#define STIM_LIB_STATE_TRG_VOLT_PRESTART		exStimLib_state.trg_cfg.volt_prestart
#define STIM_LIB_STATE_TRG_OUT_ENABLE			exStimLib_state.trg_cfg.trg_out_enable
#define STIM_LIB_STATE_TRG_OUT_ACT_POL			exStimLib_state.trg_cfg.trg_out_active_pol
#define STIM_LIB_STATE_TRG_IN_ENABLE			exStimLib_state.trg_cfg.trg_in_enable
#define STIM_LIB_STATE_TRG_IN_ACT_POL			exStimLib_state.trg_cfg.trg_in_active_pol
#define STIM_LIB_STATE_TRG_IN_TOGGLED			exStimLib_state.trg_cfg.trg_in_toggled

/* SYSTEM MASTER CLOCK VALUE */
#define STIM_LIB_SYSTEM_CLK_FREQ					HAL_RCC_GetHCLKFreq()
#define STIM_LIB_SYSTEM_STEPUP_PSC					800	/* UNUSED >> TIMER 1 */
#define STIM_LIB_SYSTEM_STIM_PSC					80	/* TIMER 2 */
#define STIM_LIB_SYSTEM_FEEDBACK_PSC				800	/* TIMER 6 */
#define STIM_LIB_TIMER_SCALE						(int)(80000000 / STIM_LIB_SYSTEM_CLK_FREQ)

/*
 * STIMULATION TIMER CONFIG
 * TIM 2
 * */
/* STIM TIMER :: TIM2 :: SCALE SETTING */
#define STIM_LIB_SIGNAL_PSC							(int)(STIM_LIB_SYSTEM_STIM_PSC / STIM_LIB_TIMER_SCALE)
#define STIM_LIB_SIGNAL_ARR							(int)((STIM_LIB_SYSTEM_CLK_FREQ / STIM_LIB_SIGNAL_PSC) / STIM_LIB_STATE_SIG_FREQ)
#define STIM_LIB_SIGNAL_PSC_INPUT					STIM_LIB_SIGNAL_PSC - 1
#define STIM_LIB_SIGNAL_ARR_INPUT					STIM_LIB_SIGNAL_ARR - 1

/* STIM TIMER :: TIM2 :: DEFAULT PARAMETER ::GLICH and DEAD TIME */
#define STIM_LIB_SIGNAL_GLICH_TIME					5	/* 5us */
#define STIM_LIB_SIGNAL_DEAD_TIME					20	/* 20us */

/* TIM2 TOTAL PULSE SIZE */
#define STIM_LIB_TOTAL_PULSE_WIDTH					(int)(STIM_LIB_SIGNAL_GLICH_TIME * 2) + (STIM_LIB_STATE_SIG_PW * 2) + STIM_LIB_SIGNAL_DEAD_TIME

/* STIM TINER :: TIM2 CHANNEL 1 :: TRIGGER OUTPUT PULSE :: PWM OUTPUT */
#define STIM_LIB_TRG_OUTPUT_PULSE_TIME				(int)STIM_LIB_TOTAL_PULSE_WIDTH

/* STIM TIMER :: TIM2 CHANNEL 2 :: ANODE PULSE :: PWM OUTPUT*/
#define STIM_LIB_ANODE_PULSE_TIME					(int)STIM_LIB_STATE_SIG_PW + (STIM_LIB_SIGNAL_GLICH_TIME * 2)

/* STIM TIMER :: TIM2 CHANNEL 3 :: CATHODE PULSE :: OUTPUT COMPAIR DMA */
#define STIM_LIB_CATHODE_PULSE_TIME0				(int)(STIM_LIB_SIGNAL_GLICH_TIME * 2) + (STIM_LIB_STATE_SIG_PW * 2) + STIM_LIB_SIGNAL_DEAD_TIME
#define STIM_LIB_CATHODE_PULSE_TIME1				(int)STIM_LIB_STATE_SIG_PW + STIM_LIB_SIGNAL_DEAD_TIME

/* STIM TIMER :: TIM2 CHANNEL 4 :: DAC ON CONTROL :: OUTPUT COMPAIR DMA >> CONST CURRENT PULSE */
#define STIM_LIB_DAC_CTRL_TIME0						(int)STIM_LIB_STATE_SIG_PW + STIM_LIB_SIGNAL_GLICH_TIME
#define STIM_LIB_DAC_CTRL_TIME1						(int)STIM_LIB_STATE_SIG_PW + STIM_LIB_SIGNAL_DEAD_TIME + STIM_LIB_SIGNAL_GLICH_TIME
#define STIM_LIB_DAC_CTRL_TIME2						(int)(STIM_LIB_STATE_SIG_PW * 2) + STIM_LIB_SIGNAL_DEAD_TIME + STIM_LIB_SIGNAL_GLICH_TIME // 2PW + D + S
#define STIM_LIB_DAC_CTRL_TIME3						(int)STIM_LIB_SIGNAL_GLICH_TIME

/* STIM TIMER :: TIM2 CHANNEL 4 :: DISCHARGE CONTROL PULSE :: OUTPUT COMPAIR DMA >> CONST VOLTAGE PULSE */
#define STIM_LIB_DISCHARGE_PULSE_WIDTH				5000 /* 5000us >> 5ms */
#define STIM_LIB_DISCHARGE_PULSE_TIME0				(int)STIM_LIB_TOTAL_PULSE_WIDTH + STIM_LIB_SIGNAL_DEAD_TIME + STIM_LIB_DISCHARGE_PULSE_WIDTH
#define STIM_LIB_DISCHARGE_PULSE_TIME1				(int)STIM_LIB_TOTAL_PULSE_WIDTH + STIM_LIB_SIGNAL_DEAD_TIME

/*
 * STEP UP FEEDBACK TIMER CONFIG
 * TIM 6
 * */
#define STIM_LIB_STEPUP_FEEDBACK_FREQ				10 /* 10Hz */
#define STIM_LIB_STEPUP_PSC							(int)(STIM_LIB_SYSTEM_FEEDBACK_PSC / STIM_LIB_TIMER_SCALE)
#define STIM_LIB_STEPUP_ARR							(int)((STIM_LIB_SYSTEM_CLK_FREQ / STIM_LIB_STEPUP_PSC) / STIM_LIB_STEPUP_FEEDBACK_FREQ)
#define STIM_LIB_STEPUP_PSC_INPUT					STIM_LIB_STEPUP_PSC - 1
#define STIM_LIB_STEPUP_ARR_INPUT					STIM_LIB_STEPUP_ARR - 1

typedef struct
{
	stim_lib_state_t cur_state;

	stim_signal_cfg_t signal_cfg;

	stim_trg_cfg_t trg_cfg;

} stim_lib_state_data_t;

extern stim_lib_state_data_t exStimLib_state;

stim_lib_state_t stimLib_stateGet(void);

bool stimLib_stateSet(stim_lib_state_t set_state);

void stimLib_stateSigParamSet(stim_signal_cfg_t *cfg);

void stimLib_stateTrgParamSet(stim_trg_cfg_t *cfg);

bool stimLib_stateIsInSession(void);

bool stimLib_stateSigParamCheck(void);

void stimLib_stateParamClear(void);

#endif	/* STIM_LIB_STATE_H */

