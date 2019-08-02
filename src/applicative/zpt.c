/*
 * zpt.c
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludovic
 */

#include "zpt.h"

#include "gpio.h"
#include "mapping.h"
#include "sw4.h"

/*** ZPT local structures ***/

typedef struct {
	SW4_Context zpt_sw4;
	SW4_State zpt_previous_state;
} ZPT_Context;

/*** ZPT local global variables ***/

static ZPT_Context zpt_ctx;

/*** ZPT functions ***/

/* INIT ZPT MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZPT_Init(void) {

	/* Configure analog GPIOs */
	GPIO_Configure(&GPIO_ZPT, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_NONE);

	/* Init context */
	SW4_Init(&zpt_ctx.zpt_sw4, 2000);
	zpt_ctx.zpt_previous_state = SW4_P0;
}

/* UPDATE THE VOLTAGE READ ON ZPT SELECTOR (CALLED BY ADC ROUTINE).
 * @param new_voltage:	New voltage measured.
 * @return:				None.
 */
void ZPT_SetVoltageMv(unsigned int zpt_voltage_mv) {
	SW4_SetVoltageMv(&zpt_ctx.zpt_sw4, zpt_voltage_mv);
}

/* MAIN ROUTINE OF ZPT MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZPT_Task(void) {
	// Update current state.
	SW4_UpdateState(&zpt_ctx.zpt_sw4);
	// Perform actions according to state.
	switch (zpt_ctx.zpt_sw4.state) {
	case SW4_P0:
		// TBD.
		break;
	case SW4_P1:
		// TBD.
		break;
	case SW4_P2:
		// TBD.
		break;
	case SW4_P3:
		// TBD.
		break;
	default:
		// Unknown state.
		break;
	}
	// Update previous state.
	zpt_ctx.zpt_previous_state = zpt_ctx.zpt_sw4.state;
}
