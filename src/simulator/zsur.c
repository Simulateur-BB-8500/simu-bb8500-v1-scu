/*
 * zsur.c
 *
 *  Created on: 27 dec. 2021
 *      Author: Ludo
 */

#include "zsur.h"

#include "gpio.h"
#include "lsmcu.h"
#include "manometer.h"
#include "mapping.h"
#include "sw2.h"

/*** ZSUR local macros ***/

#define LSUR_CG_PRESSURE_THRESHOLD_DECIBARS	50

/*** ZSUR local structures ***/

typedef struct {
	SW2_context_t sw2;
	SW2_state_t previous_state;
} ZSUR_Context;

/*** ZSUR external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** ZSUR local global variables ***/

static ZSUR_Context zsur_ctx;

/*** ZSUR functions ***/

/* INIT ZSUR MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZSUR_init(void) {
	// Init GPIOs.
	SW2_init(&zsur_ctx.sw2, &GPIO_ZSUR, 0, 100); // ZSUR active low.
	GPIO_configure(&GPIO_LSUR, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
}

/* MAIN TASK OF ZSUR MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZSUR_task(void) {
	// Update switch state.
	SW2_update_state(&zsur_ctx.sw2);
	// Check state.
	if (zsur_ctx.sw2.state == SW2_ON) {
		// Send command on change.
		if (zsur_ctx.previous_state != SW2_ON) {
			MANOMETER_set_pressure(lsmcu_ctx.manometer_cg, (lsmcu_ctx.manometer_cg) -> pressure_limit_decibars);
		}
	}
	// Update previous state.
	zsur_ctx.previous_state = zsur_ctx.sw2.state;
	// Manage LSUR indicator.
	if (MANOMETER_get_pressure(lsmcu_ctx.manometer_cg) > LSUR_CG_PRESSURE_THRESHOLD_DECIBARS) {
		GPIO_write(&GPIO_LSUR, 1);
	}
	else {
		GPIO_write(&GPIO_LSUR, 0);
	}
}


