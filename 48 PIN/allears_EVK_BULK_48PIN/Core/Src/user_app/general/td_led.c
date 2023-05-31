/*
 * td_led.c
 *
 *  Created on: 2023. 6. 9.
 *      Author: eidos
 */
#include "stim_lib.h"
#include "stim_lib_type.h"

#include "td_led.h"
#include "td_btn.h"
#include "td_debug.h"

#if 0
typedef struct
{
	uint8_t led_colors;
	uint16_t off_time;
	uint16_t on_time;
} td_led_state_ind_t;

typedef struct
{
	bool led_on;
	uint32_t led_tick;
	uint32_t led_timeout_tick;
	bool led_timed_out;
	td_led_sate_t led_state;
} td_led_state_data_t;
td_led_state_data_t td_led_state;
#endif
