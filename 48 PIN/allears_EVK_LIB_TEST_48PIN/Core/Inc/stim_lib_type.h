#ifndef	STIM_LIB_TYPE_H

#define STIM_LIB_TYPE_H

#include <stdint.h>
#include <stddef.h>

#include <stdbool.h>

/* Responses of requests to stimluation library */
typedef enum
{
	/* Request is handled */
	stim_lib_stim_rsp_ok = 0x00,

	/* Library requirement is failed */
	stim_lib_stim_rsp_failed,

	/* Library is not initialized */
	stim_lib_stim_rsp_uninitailized,

	/* Invalid status to handle request */
	stim_lib_stim_rsp_invalid_status,

	/* Request has invalid parameters */
	stim_lib_stim_rsp_invalid_parameter,

	/* Request is not supported */
	stim_lib_stim_rsp_unsupported,

	/* System is fault in currently */
	stim_lib_stim_rsp_system_fault,

	/* Max number of responses */
	stim_lib_stim_rsp_max
} stim_lib_rsp_t;

/* Stimulation type applied to library */
typedef enum
{
	/* Stimulation library type is unknown */
	stim_lib_stim_type_unknown = 0x00,

	/* Stimulation library type is constant current */
	stim_lib_stim_type_const_current,

	/* Stimulation library type is constant voltage */
	stim_lib_stim_type_const_voltage,

	/* Max number of Stimulation library types */
	stim_lib_stim_type_max
} stim_lib_stim_type_t;

/* States of stimation library */
typedef enum
{
	/* Library is not initialized */
	stim_lib_state_uninitialized = 0x00,

	/* Stimulation session is deactive */
	stim_lib_state_idle,

	/* Stimulation session is started and not stimating */
	stim_lib_state_session_idle,

	/* Stimulation starts */
	stim_lib_state_stimulating,

	/* Stimulation stopping */
	stim_lib_state_stim_stopping,

	/* Library is in error */
	stim_lib_state_error,

	/* Max number of stimation library states */
	stim_lib_state_max
} stim_lib_state_t;

/* Stimulation output signal configuration parameters */
typedef struct
{
	/* Stimualtion signal frequency */
	uint8_t freq;

	/* Stimualtion signal pulse width */
	uint16_t pulse_width;

	/*
	 - Constantl volrtage: stimulation voltage
	 - Constantl current: stimulation DAC ON value
	 */
	uint8_t degree;

	/*
	 - Constantl volrtage: ignored
	 - Constantl current: stimation peak detection callback function. If value is NULL, peak detection is disabled
	 - parameters: uint16_t* src_buf, uint8_t src_buf_len
	 */
	void (*peak_detect_callback)(uint16_t*, uint8_t);

} stim_signal_cfg_t;

/* Stimuation trigger siganl configuration parameters */
typedef struct
{

	/*
	 Decide if voltage control starts when session starts or when stimulation signal start at first
	 VALUE:
	 - false: start voltage control when stimulation signal start at first (default)
	 - true: start voltage control when session starts

	 */
	bool volt_prestart;

	/*
	 Trigger output signal is enabled or not
	 VALUE:
	 - false: disabled
	 - true: enabled

	 */
	bool trg_out_enable;

	/*
	 If trigger output signal is enabled, set signal polarity
	 VALUE:
	 - 0: low level
	 - 1: high level
	 */
	uint8_t trg_out_active_pol;

	/*
	 Stimul trigger input signal config
	 */
	/*
	 Trigger input signal is enabled or not
	 VALUE:
	 - false: disabled
	 - true: enabled
	 */
	bool trg_in_enable;

	/*
	 If trigger input signal is enabled, set signal polarity
	 VALUE:
	 - 0: low level
	 - 1: high level
	 */
	uint8_t trg_in_active_pol;

	/*
	 If trigger input signal is enabled, stimulation signal is output as to trigger input signal polarity or not
	 VALUE:
	 - false: toggle is disabled
	 - true: toggle is enabled
	 */
	bool trg_in_toggled;

} stim_trg_cfg_t;

/* Configuration parameters of stimation session */
typedef struct
{
	/* Stimulation output signal configuration parameters */
	stim_signal_cfg_t signal_cfg;

	/* Stimuation trigger siganl configuration parameters */
	stim_trg_cfg_t trg_cfg;
} stim_lib_ssession_cfg_t;

#endif	/* STIM_LIB_TYPE_H */

