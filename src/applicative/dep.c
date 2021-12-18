/*
 * dep.c
 *
 *  Created on: 22 mar. 2020
 *      Author: Ludo
 */

#include "dep.h"

#include "gpio.h"
#include "lsmcu.h"
#include "mapping.h"
#include "sw2.h"
#include "tim.h"

/*** DEP local macros ***/

#define DEP_RING_PULSE_DURATION_MS	250

/*** DEP local structures ***/

typedef enum {
	DEP_STATE_ENABLED,
	DEP_STATE_RING1,
	DEP_STATE_RELEASE1,
	DEP_STATE_RING2,
	DEP_STATE_DISABLED
} DEP_State;

typedef struct {
	// Input.
	SW2_Context dep_zlct;
	// State machine.
	DEP_State dep_state;
	unsigned int dep_switch_state_time; // In ms.
} DEP_Context;

/*** DEP external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** DEP local global variables ***/

static DEP_Context dep_ctx;

/* INIT DEP MODULE.
 * @param:	None.
 * @return:	None.
 */
void DEP_Init(void) {
	// Init GPIO.
	SW2_Init(&dep_ctx.dep_zlct, &GPIO_ZLCT, 0, 100); // ZLCT active low.
	GPIO_Configure(&GPIO_DEP, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init context.
	dep_ctx.dep_state = DEP_STATE_ENABLED;
	dep_ctx.dep_switch_state_time = 0;
}

void DEP_Task(void) {
	// Perform state machine.
	SW2_UpdateState(&dep_ctx.dep_zlct);
	switch (dep_ctx.dep_state) {
	case DEP_STATE_ENABLED:
		// Check speed and switch.
		if ((lsmcu_ctx.lsmcu_speed_kmh == 0) && (dep_ctx.dep_zlct.sw2_state == SW2_ON)) {
			// First ring.
			GPIO_Write(&GPIO_DEP, 1);
			dep_ctx.dep_switch_state_time = TIM2_GetMs();
			dep_ctx.dep_state = DEP_STATE_RING1;
		}
		break;
	case DEP_STATE_RING1:
		if (TIM2_GetMs() > (dep_ctx.dep_switch_state_time + DEP_RING_PULSE_DURATION_MS)) {
			// Release.
			GPIO_Write(&GPIO_DEP, 0);
			dep_ctx.dep_switch_state_time = TIM2_GetMs();
			dep_ctx.dep_state = DEP_STATE_RELEASE1;
		}
		break;
	case DEP_STATE_RELEASE1:
		if (TIM2_GetMs() > (dep_ctx.dep_switch_state_time + DEP_RING_PULSE_DURATION_MS)) {
			// Second ring.
			GPIO_Write(&GPIO_DEP, 1);
			dep_ctx.dep_switch_state_time = TIM2_GetMs();
			dep_ctx.dep_state = DEP_STATE_RING2;
		}
		break;
	case DEP_STATE_RING2:
		if (TIM2_GetMs() > (dep_ctx.dep_switch_state_time + DEP_RING_PULSE_DURATION_MS)) {
			// Release.
			GPIO_Write(&GPIO_DEP, 0);
			dep_ctx.dep_switch_state_time = TIM2_GetMs();
			dep_ctx.dep_state = DEP_STATE_DISABLED;
		}
		break;
	case DEP_STATE_DISABLED:
		// Enable bell on next stop.
		if (lsmcu_ctx.lsmcu_speed_kmh > 0) {
			dep_ctx.dep_state = DEP_STATE_ENABLED;
		}
		break;
	default:
		break;
	}
}
