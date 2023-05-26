/*
 * stim_lib_stim_cfg.h
 *
 *  Created on: 2023. 4. 24.
 *      Author: eidos
 */
#ifndef	SRC_STIM_LIB_STIM_STIM_CFG_H
#define	SRC_STIM_LIB_STIM_STIM_CFG_H

#include "stim_lib_type.h"
#include "stim_lib_state.h"

#define STIM_LIB_TRG_INPUT_IS_ENABLED()						(STIM_LIB_STATE_TRG_IN_ENABLE == stim_lib_trg_input_enable)
#define STIM_LIB_TRG_OUTPUT_IS_ENABLED()					(STIM_LIB_STATE_TRG_OUT_ENABLE == stim_lib_trg_output_enable)

#define STIM_LIB_TRG_INPUT_TOGGLE_IS_ACT_LOW()				(STIM_LIB_STATE_TRG_IN_ACT_POL == stim_lib_trg_input_active_low && STIM_LIB_STATE_TRG_IN_TOGGLED == stim_lib_trg_input_toggle_enable)
#define STIM_LIB_TRG_INPUT_TOGGLE_IS_ACT_HIGH()				(STIM_LIB_STATE_TRG_IN_ACT_POL == stim_lib_trg_input_active_high && STIM_LIB_STATE_TRG_IN_TOGGLED == stim_lib_trg_input_toggle_enable)
#define STIM_LIB_TRG_INPUT_IS_FALLING_EDGE()				(STIM_LIB_STATE_TRG_IN_ACT_POL == stim_lib_trg_input_active_low && STIM_LIB_STATE_TRG_IN_TOGGLED == stim_lib_trg_input_toggle_disable)
#define STIM_LIB_TRG_INPUT_IS_RISING_EDGE()					(STIM_LIB_STATE_TRG_IN_ACT_POL == stim_lib_trg_input_active_high && STIM_LIB_STATE_TRG_IN_TOGGLED == stim_lib_trg_input_toggle_disable)

/* TRIGGER OUTPUT ENABLE TYPE */
typedef enum
{
	stim_lib_trg_output_disable = 0x00,
	stim_lib_trg_output_enable,
	stim_lib_trg_output_enable_max
} stim_lib_trigger_output_enable_type_t;

/* TRIGGER OUTPUT ACTIVE TYPE */
typedef enum
{
	stim_lib_trg_output_active_low = 0x00,
	stim_lib_trg_output_active_high,
	stim_lib_trg_output_active_max
} stim_lib_trigger_output_active_type_t;

/* TRIGGER INPUT ENABLE TYPE */
typedef enum
{
	stim_lib_trg_input_disable = 0x00,
	stim_lib_trg_input_enable,
	stim_lib_trg_input_enable_max
} stim_lib_trigger_input_enable_type_t;

/* TRIGGER INPUT ACTIVE TYPE */
typedef enum
{
	stim_lib_trg_input_active_low = 0x00,
	stim_lib_trg_input_active_high,
	stim_lib_trg_input_active_max
} stim_lib_trigger_intput_active_type_t;

/* TRIGGER INPUT TOGGLE ENABLE TYPE */
typedef enum
{
	stim_lib_trg_input_toggle_disable = 0x00,
	stim_lib_trg_input_toggle_enable,
	stim_lib_trg_input_toggle_max
} stim_lib_trigger_input_edge_type_t;

/*
 * FUNCTION AREA
 * INPUT PARAMETER CHECK FUNCTION
 * */
bool stimLib_signalParamCheck(stim_signal_cfg_t *cfg);
bool stimLib_triggerParamCheck(stim_trg_cfg_t *cfg);

bool stimLib_paramSetting(void);
void stimLib_paramClear(void);

bool stimLib_stimPulseSetiing(void);
void stimLib_stimPulseStart(void);
void stimLib_stimPulseStop(void);

/*
 * stim_lib_stim_cfg_st.c
 * */
bool stimLib_paramPulseSettingRaw(void);
void stimLib_paramTrgSettingRaw(void);
void stimLib_paramTrgResettingRaw(void);

bool stimLib_pulseConfigRaw(void);
bool stimLib_stimStartRaw(void);
bool stimLib_stimStopRaw(void);

/*
 * stim_lib_stim_inc_st.c
 * */
void stimLib_stimStopDelayRaw(void);

#endif /* SRC_STIM_LIB_STIM_LIB_STIM_CFG_H */

