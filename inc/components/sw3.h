/*
 * sw3.h
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludovic
 */

#ifndef SW3_H
#define SW3_H

#include "gpio.h"

/*** 3-poles switch structures ***/

// Output state.
typedef enum {
	SW3_BACK = 0,
	SW3_NEUTRAL,
	SW3_FRONT
} SW3_State;

// Internal state machine.
typedef enum {
	SW3_STATE_NEUTRAL = 10,
	SW3_STATE_CONFIRM_NEUTRAL,
	SW3_STATE_BACK,
	SW3_STATE_CONFIRM_BACK,
	SW3_STATE_FRONT,
	SW3_STATE_CONFIRM_FRONT
} SW3_InternalState;

// SW3 structure.
typedef struct {
	unsigned int sw3_voltage; // Current voltage measured by ADC.
	SW3_InternalState sw3_internal_state; // Current state in SW3 internal state machine.
	SW3_State sw3_state; // State after anti-bouncing (used in higher levels).
	unsigned int sw3_debouncing_ms; // Delay before validating states (in ms).
	unsigned int sw3_confirm_start_time;
} SW3_Context;

/*** 3-poles switch functions ***/

void SW3_Init(SW3_Context* sw3, const GPIO* sw3_gpio, unsigned int sw3_debouncing_ms);
void SW3_SetVoltageMv(SW3_Context* sw3, unsigned int sw3_voltage_mv);
void SW3_UpdateState(SW3_Context* sw3);

#endif /* SW3_H */
