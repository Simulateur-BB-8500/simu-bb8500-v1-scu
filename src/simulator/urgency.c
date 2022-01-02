/*
 * urgency.c
 *
 *  Created on: 26 dec. 2021
 *      Author: Ludo
 */

#include "urgency.h"

#include "gpio.h"
#include "lsmcu.h"
#include "lssgiu.h"
#include "manometer.h"
#include "mapping.h"
#include "sw2.h"

/*** URGENCY local structures ***/

typedef struct {
	SW2_Context bpurg;
	unsigned char previous_state;
} URGENCY_Context;

/*** URGENCY external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** URGENCY local global variables ***/

static URGENCY_Context urgency_ctx;

/*** URGENCY functions ***/

/* INIT URGENCY MODULE.
 * @param:	None.
 * @return:	None.
 */
void URGENCY_Init(void) {
	// Init GPIO.
	SW2_Init(&urgency_ctx.bpurg, &GPIO_BPURG, 0, 100); // URGENCY active low.
	// Init context.
	urgency_ctx.previous_state = 0;
	lsmcu_ctx.urgency = 0;
}

/* MAIN TAS OF URGENCY MODULE.
 * @param:	None.
 * @return:	None.
 */
void URGENCY_Task(void) {
	// Update BPURG state.
	SW2_UpdateState(&urgency_ctx.bpurg);
	if (urgency_ctx.bpurg.state == SW2_ON) {
		// Update global context.
		lsmcu_ctx.urgency = 1;
	}
	// Check global flag.
	if ((lsmcu_ctx.urgency != 0) && (urgency_ctx.previous_state == 0)) {
		// Trigger urgency brake.
		MANOMETER_SetPressure(lsmcu_ctx.manometer_cg, 0);
		MANOMETER_SetPressure(lsmcu_ctx.manometer_re, 0);
		MANOMETER_SetPressure(lsmcu_ctx.manometer_cf1, (lsmcu_ctx.manometer_cf1) -> pressure_limit_decibars);
		MANOMETER_SetPressure(lsmcu_ctx.manometer_cf2, (lsmcu_ctx.manometer_cf2) -> pressure_limit_decibars);
		// Open DJ.
		lsmcu_ctx.dj_locked = 0;
		// Send sound command.
		LSSGIU_Send(LSMCU_OUT_URGENCY);
	}
	// Release urgency state only when train is stopped.
	if ((urgency_ctx.bpurg.state == SW2_OFF) && (lsmcu_ctx.speed_kmh == 0)) {
		lsmcu_ctx.urgency = 0;
	}
	// Update state.
	urgency_ctx.previous_state = lsmcu_ctx.urgency;
}
