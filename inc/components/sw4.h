/*
 * sw4.h
 *
 *  Created on: 26 dec. 2017
 *      Author: Ludo
 */

#ifndef __SW4_H__
#define __SW4_H__

#include "gpio.h"
#include "stdint.h"

/*** SW4 structures ***/

// Output state.
typedef enum {
	SW4_P0 = 0,
	SW4_P1,
	SW4_P2,
	SW4_P3
} SW4_state_t;

// Internal state machine.
typedef enum {
	SW4_STATE_P0 = 10,
	SW4_STATE_CONFIRM_P0,
	SW4_STATE_P1,
	SW4_STATE_CONFIRM_P1,
	SW4_STATE_P2,
	SW4_STATE_CONFIRM_P2,
	SW4_STATE_P3,
	SW4_STATE_CONFIRM_P3
} SW4_internal_state_t;

typedef struct {
	uint32_t voltage; // Current voltage measured by ADC.
	SW4_internal_state_t internal_state; // Current state in SW4 state machine.
	SW4_state_t state; // State after anti-bouncing (used in higher levels).
	uint32_t debouncing_ms; // Delay before validating states (in ms).
	uint32_t confirm_start_time;
} SW4_context_t;

/*** SW4 functions ***/

void SW4_init(SW4_context_t* sw4, const GPIO* gpio, uint32_t debouncing_ms);
void SW4_set_voltage_mv(SW4_context_t* sw4, uint32_t voltage_mv);
void SW4_update_state(SW4_context_t* sw4);

#endif /* __SW4_H__ */
