/*
 * bell.c
 *
 *  Created on: 22 mar. 2020
 *      Author: Ludo
 */

#include "bell.h"

#include "gpio.h"
#include "lsmcu.h"
#include "mapping.h"
#include "sw2.h"
#include "tim.h"
#include "types.h"

/*** BELL local macros ***/

#define BELL_RING_PULSE_DURATION_MS	250

/*** BELL local structures ***/

typedef enum {
	BELL_STATE_ENABLED,
	BELL_STATE_RING1,
	BELL_STATE_RELEASE1,
	BELL_STATE_RING2,
	BELL_STATE_DISABLED
} BELL_State;

typedef struct {
	// Input.
	SW2_context_t zlct;
	// State machine.
	BELL_State state;
	uint32_t switch_state_time; // In ms.
} BELL_Context;

/*** BELL external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** BELL local global variables ***/

static BELL_Context bell_ctx;

/* INIT BELL MODULE.
 * @param:	None.
 * @return:	None.
 */
void BELL_init(void) {
	// Init GPIO.
	SW2_init(&bell_ctx.zlct, &GPIO_ZLCT, 0, 100); // ZLCT active low.
	GPIO_configure(&GPIO_BELL, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init context.
	bell_ctx.state = BELL_STATE_ENABLED;
	bell_ctx.switch_state_time = 0;
}

void BELL_task(void) {
	// Perform state machine.
	SW2_update_state(&bell_ctx.zlct);
	switch (bell_ctx.state) {
	case BELL_STATE_ENABLED:
		// Check speed and switch.
		if ((lsmcu_ctx.speed_kmh == 0) && (bell_ctx.zlct.state == SW2_ON)) {
			// First ring.
			GPIO_write(&GPIO_BELL, 1);
			bell_ctx.switch_state_time = TIM2_get_milliseconds();
			bell_ctx.state = BELL_STATE_RING1;
		}
		break;
	case BELL_STATE_RING1:
		if (TIM2_get_milliseconds() > (bell_ctx.switch_state_time + BELL_RING_PULSE_DURATION_MS)) {
			// Release.
			GPIO_write(&GPIO_BELL, 0);
			bell_ctx.switch_state_time = TIM2_get_milliseconds();
			bell_ctx.state = BELL_STATE_RELEASE1;
		}
		break;
	case BELL_STATE_RELEASE1:
		if (TIM2_get_milliseconds() > (bell_ctx.switch_state_time + BELL_RING_PULSE_DURATION_MS)) {
			// Second ring.
			GPIO_write(&GPIO_BELL, 1);
			bell_ctx.switch_state_time = TIM2_get_milliseconds();
			bell_ctx.state = BELL_STATE_RING2;
		}
		break;
	case BELL_STATE_RING2:
		if (TIM2_get_milliseconds() > (bell_ctx.switch_state_time + BELL_RING_PULSE_DURATION_MS)) {
			// Release.
			GPIO_write(&GPIO_BELL, 0);
			bell_ctx.switch_state_time = TIM2_get_milliseconds();
			bell_ctx.state = BELL_STATE_DISABLED;
		}
		break;
	case BELL_STATE_DISABLED:
		// Enable bell on next stop.
		if (lsmcu_ctx.speed_kmh > 0) {
			bell_ctx.state = BELL_STATE_ENABLED;
		}
		break;
	default:
		break;
	}
}
