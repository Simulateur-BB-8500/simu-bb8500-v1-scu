/*
 * mpinv.c
 *
 *  Created on: 8 avr. 2018
 *      Author: Ludovic
 */

#include "mpinv.h"

#include "gpio.h"
#include "mapping.h"
#include "sw3.h"

/*** MPINV local structures ***/

typedef struct {
	SW3_Context mpinv_sw3;
	SW3_State mpinv_previous_state;
} MPINV_Context;

/*** MPINV local global variables ***/

static MPINV_Context mpinv_ctx;

/*** MPINV functions ***/

/* INIT MPINV MODULE.
 * @param:	None.
 * @return:	None.
 */
void MPINV_Init(void) {

	/* Init context */
	SW3_Init(&mpinv_ctx.mpinv_sw3, &GPIO_MPINV, 2000);
	mpinv_ctx.mpinv_previous_state = SW3_NEUTRAL;
}

/* UPDATE THE VOLTAGE READ ON MPINV SELECTOR (CALLED BY ADC ROUTINE).
 * @param new_voltage:	New voltage measured.
 * @return:				None.
 */
void MPINV_SetVoltageMv(unsigned int mpinv_voltage_mv) {
	SW3_SetVoltageMv(&mpinv_ctx.mpinv_sw3, mpinv_voltage_mv);
}

/* MAIN ROUTINE OF MPINV MODULE.
 * @param:	None.
 * @return:	None.
 */
void MPINV_Task(void) {
	// Update current state.
	SW3_UpdateState(&mpinv_ctx.mpinv_sw3);
	// Perform actions according to state.
	switch (mpinv_ctx.mpinv_sw3.sw3_state) {
	case SW3_BACK:
		// TBD.
		break;
	case SW3_NEUTRAL:
		// TBD.
		break;
	case SW3_FRONT:
		// TBD.
		break;
	default:
		// Unknown state.
		break;
	}
	// Update previous state.
	mpinv_ctx.mpinv_previous_state = mpinv_ctx.mpinv_sw3.sw3_state;
}
