/*
 * fd.c
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#include "fd.h"

#include "gpio.h"
#include "lssgiu.h"
#include "mapping.h"
#include "sw3.h"

/*** FD local structures ***/

typedef struct {
	SW3_Context sw3;
	SW3_State previous_state;
} FD_Context;

/*** FD local global variables ***/

static FD_Context fd_ctx;

/*** FD functions ***/

/* INIT FD MODULE.
 * @param:	None.
 * @return:	None.
 */
void FD_Init(void) {
	// Init GPIO.
	SW3_Init(&fd_ctx.sw3, &GPIO_FD, 100);
	fd_ctx.previous_state = SW3_NEUTRAL;
}

/* UPDATE THE VOLTAGE READ ON FD SELECTOR (CALLED BY ADC ROUTINE).
 * @param new_voltage:	New voltage measured.
 * @return:				None.
 */
void FD_SetVoltageMv(unsigned int fd_voltage_mv) {
	SW3_SetVoltageMv(&fd_ctx.sw3, fd_voltage_mv);
}

/* MAIN ROUTINE OF FD MODULE.
 * @param:	None.
 * @return:	None.
 */
void FD_Task(void) {
	// Update current state.
	SW3_UpdateState(&fd_ctx.sw3);
	// Perform actions according to state.
	switch (fd_ctx.sw3.state) {
	case SW3_BACK:
		if (fd_ctx.previous_state != SW3_BACK) {
			// Backward.
			LSSGIU_Send(LSMCU_OUT_FD_RELEASE);
		}
		break;
	case SW3_NEUTRAL:
		if (fd_ctx.previous_state != SW3_NEUTRAL) {
			// Forward.
			LSSGIU_Send(LSMCU_OUT_FD_NEUTRAL);
		}
		break;
	case SW3_FRONT:
		if (fd_ctx.previous_state != SW3_FRONT) {
			// Forward.
			LSSGIU_Send(LSMCU_OUT_FD_APPLY);
		}
		break;
	default:
		// Unknown state.
		break;
	}
	// Update previous state.
	fd_ctx.previous_state = fd_ctx.sw3.state;
}

