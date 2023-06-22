/*
 * td_btn.h
 *
 *  Created on: Oct 24, 2022
 *      Author: ECHO
 */
#include "main.h"

#ifndef INC_APP_TD_BTN_H_
#define INC_APP_TD_BTN_H_

typedef struct
{
	bool pressed;
	bool handle_enable;
	bool handled;
	uint32_t held_tick;
} td_btn_state_data_t;

#define TD_BTN_HELD_TIME								1000 /* 100ms */

/*
 * BTN PRESSED CHECK
 * */
#define TD_START_BTN_IS_PRESSED()						(HAL_GPIO_ReadPin(BTN_START_GPIO_Port, BTN_START_Pin) == GPIO_PIN_RESET)
#define TD_STIM_UP_BTN_IS_PRESSED()						(HAL_GPIO_ReadPin(BTN_STIM_UP_GPIO_Port, BTN_STIM_UP_Pin) == GPIO_PIN_RESET)
#define TD_STIM_DOWN_BTN_IS_PRESSED()					(HAL_GPIO_ReadPin(BTN_STIM_DOWN_GPIO_Port, BTN_STIM_DOWN_Pin) == GPIO_PIN_RESET)

/*
 * START BTN STATE
 * */
#define TD_START_BTN_STATE_PRESSED						td_start_btn_state.pressed
#define TD_START_BTN_STATE_HANDLE_ENABLE				td_start_btn_state.handle_enable
#define TD_START_BTN_STATE_HANDLED						td_start_btn_state.handled
#define TD_START_BTN_STATE_HELD_TICK					td_start_btn_state.held_tick

/*
 * STIM UP BTN STATE
 * */
#define TD_STIM_UP_BTN_STATE_PRESSED					td_stim_up_btn_state.pressed
#define TD_STIM_UP_BTN_STATE_HANDLE_ENABLE				td_stim_up_btn_state.handle_enable
#define TD_STIM_UP_BTN_STATE_HANDLED					td_stim_up_btn_state.handled
#define TD_STIM_UP_BTN_STATE_HELD_TICK					td_stim_up_btn_state.held_tick

/*
 * STIM DOWN BTN STATE
 * */
#define TD_STIM_DOWN_BTN_STATE_PRESSED					td_stim_down_btn_state.pressed
#define TD_STIM_DOWN_BTN_STATE_HANDLE_ENABLE			td_stim_down_btn_state.handle_enable
#define TD_STIM_DOWN_BTN_STATE_HANDLED					td_stim_down_btn_state.handled
#define TD_STIM_DOWN_BTN_STATE_HELD_TICK				td_stim_down_btn_state.held_tick

/*
 * START BTN FUNCTION
 * */
bool td_isStartButtonHandled(void);
void td_clearStartButtonHandled(void);
void td_enableStartButtonHandling(bool enable);

void td_handleStartButtonAction(void);
void td_handleStimUpButtonAction(void);
void td_handleStimDownButtonAction(void);

void td_handleButton(void);

#endif /* INC_APP_TD_BTN_H_ */
