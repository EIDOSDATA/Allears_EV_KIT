#ifndef	STIMUL_LIB_H

#define STIMUL_LIB_H


#include "stim_lib_type.h"
#include "stim_lib_st_inc.h"


/*
	BRIEF
		- Get the version of stimulation library
	PARAM:
		- buff: buffer to copy stimulation library version
		- len: length of buffer
	RETRUN:
		- length of stimulation library version copied to buffer
*/
uint8_t stimLib_verGet(uint8_t* buff, uint8_t len);


/*
	BRIEF
		- Get stimulation type applied to library
	PARAM:
		- void
	RETRUN:
		- Stimulation type applied to library
*/
stim_lib_stim_type_t stimLib_stimTypeGet(void);


/*
	BRIEF
		- Initalize stimulation library. Preempt system resources and peripherals.
	PARAM:
		- void
	RETRUN:
		- Response to initialisation of stimulation library
*/
stim_lib_rsp_t stimLib_stimInit(void);


/*
	BRIEF
		- Deinitalize stimulation library. Release system resources and peripherals.
	PARAM:
		- void
	RETRUN:
		- Response to deinitialisation of stimulation library
*/
stim_lib_rsp_t stimLib_stimDeinit(void);


/*
	BRIEF
		- Get current state of stimulation library.
	PARAM:
		- void
	RETRUN:
		- Current state of stimulation library
*/
stim_lib_state_t stimLib_stimCurStateGet(void);


/*
	BRIEF
		- Setup signal pusle of stimulation configuration.
	PARAM:
		- signal_cfg: signal pusle of stimulation configuration parameters
	RETRUN:
		- Response to config signal pusle
*/
stim_lib_rsp_t stimLib_stimSignalConfig(stim_signal_cfg_t* signal_cfg);


/*
	BRIEF
		- Setup trigger of stimulation configuration.
	PARAM:
		- signal_cfg: trigger of stimulation configuration parameters
	RETRUN:
		- Response to config trigger
*/
stim_lib_rsp_t stimLib_stimTriggerConfig(stim_trg_cfg_t* trg_cfg);


/*
	BRIEF
		- Start stimulation session.
	PARAM:
		- void
	RETRUN:
		- Result of starting stimulation session
*/
stim_lib_rsp_t stimLib_stimSessionStart(void);


/*
	BRIEF
		- Stop stimulation session.
	PARAM:
		- void
	RETRUN:
		- Result of stopping stimulation session
*/
stim_lib_rsp_t stimLib_stimSessionStop(void);


/*
	BRIEF
		- Start stimulation signal output in a session.
	PARAM:
		- void
	RETRUN:
		- Result of starting stimulation signal output in a session
*/
stim_lib_rsp_t stimLib_stimStart(void);


/*
	BRIEF
		- Pause stimulation signal output in a session.
	PARAM:
		- void
	RETRUN:
		- Result of Pausing stimulation signal output in a session
*/
stim_lib_rsp_t stimLib_stimPause(void);

/*
	BRIEF
		- Pause stimulation signal output in a session.
	PARAM:
		- void
	RETRUN:
		- Result of Pausing stimulation signal output in a session
*/
stim_lib_rsp_t stimLib_stimIntensiveChange(stim_signal_cfg_t *signal_cfg);


/*
	Definition in application area
*/
/*
BRIEF
	- Handle stim library errors.
PARAM:
	- void
RETURN:
	- none
*/
void sitmLib_appErrorHandle_callback(void);

/*
BRIEF
	- stim library emergency Stop.
PARAM:
	- void
RETURN:
	- none
*/
void sitmLib_emergencyStop(void);

#endif	/* STIMUL_LIB_H */

