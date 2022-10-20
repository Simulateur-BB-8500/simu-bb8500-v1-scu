/*
 * mpinv.c
 *
 *  Created on: 8 apr. 2018
 *      Author: Ludo
 */

#include "mpinv.h"

#include "gpio.h"
#include "lssgiu.h"
#include "mapping.h"
#include "sw3.h"

/*** MPINV local structures ***/

typedef struct {
	SW3_context_t sw3;
	SW3_state_t previous_state;
} MPINV_context_t;

/*** MPINV local global variables ***/

static MPINV_context_t mpinv_ctx;

/*** MPINV functions ***/

/* INIT MPINV MODULE.
 * @param:	None.
 * @return:	None.
 */
void MPINV_init(void) {
	// Init GPIO.
	SW3_init(&mpinv_ctx.sw3, &GPIO_MPINV, 100);
	mpinv_ctx.previous_state = SW3_NEUTRAL;
}

/* UPDATE THE VOLTAGE READ ON MPINV SELECTOR (CALLED BY ADC ROUTINE).
 * @param new_voltage:	New voltage measured.
 * @return:				None.
 */
void MPINV_set_voltage_mv(unsigned int mpinv_voltage_mv) {
	SW3_set_voltage_mv(&mpinv_ctx.sw3, mpinv_voltage_mv);
}

/* MAIN ROUTINE OF MPINV MODULE.
 * @param:	None.
 * @return:	None.
 */
void MPINV_task(void) {
	// Update current state.
	SW3_update_state(&mpinv_ctx.sw3);
	// Perform actions according to state.
	switch (mpinv_ctx.sw3.state) {
	case SW3_BACK:
		if (mpinv_ctx.previous_state != SW3_BACK) {
			// Backward.
			LSSGIU_Send(LSMCU_OUT_MPINV_BACKWARD);
		}
		break;
	case SW3_NEUTRAL:
		if (mpinv_ctx.previous_state != SW3_NEUTRAL) {
			// Forward.
			LSSGIU_Send(LSMCU_OUT_MPINV_NEUTRAL);
		}
		break;
	case SW3_FRONT:
		if (mpinv_ctx.previous_state != SW3_FRONT) {
			// Forward.
			LSSGIU_Send(LSMCU_OUT_MPINV_FORWARD);
		}
		break;
	default:
		// Unknown state.
		break;
	}
	// Update previous state.
	mpinv_ctx.previous_state = mpinv_ctx.sw3.state;
}
