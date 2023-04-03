/*
 * sw3.h
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludo
 */

#ifndef __SW3_H__
#define __SW3_H__

#include "gpio.h"
#include "types.h"

/*** 3-poles switch structures ***/

// Output state.
typedef enum {
	SW3_BACK = 0,
	SW3_NEUTRAL,
	SW3_FRONT
} SW3_state_t;

// Internal state machine.
typedef enum {
	SW3_STATE_NEUTRAL = 10,
	SW3_STATE_CONFIRM_NEUTRAL,
	SW3_STATE_BACK,
	SW3_STATE_CONFIRM_BACK,
	SW3_STATE_FRONT,
	SW3_STATE_CONFIRM_FRONT
} SW3_internal_state_t;

// SW3 structure.
typedef struct {
	uint32_t voltage; // Current voltage measured by ADC.
	SW3_internal_state_t internal_state; // Current state in SW3 internal state machine.
	SW3_state_t state; // State after anti-bouncing (used in higher levels).
	uint32_t debouncing_ms; // Delay before validating states (in ms).
	uint32_t confirm_start_time;
} SW3_context_t;

/*** 3-poles switch functions ***/

void SW3_init(SW3_context_t* sw3, const GPIO* gpio, uint32_t debouncing_ms);
void SW3_set_voltage_mv(SW3_context_t* sw3, uint32_t voltage_mv);
void SW3_update_state(SW3_context_t* sw3);

#endif /* __SW3_H__ */
