/*
 * s.c
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludov
 */

#include "s.h"

#include "gpio.h"
#include "lssgkcu.h"
#include "mapping.h"
#include "sw3.h"

/*** S local structures ***/

typedef struct {
	SW3_Context s_sw3;
	SW3_State s_previous_state;
} S_Context;

/*** S local global variables ***/

static S_Context s_ctx;

/*** S functions ***/

/* INIT S MODULE.
 * @param:	None.
 * @return:	None.
 */
void S_Init(void) {

	/* Init context */
	SW3_Init(&s_ctx.s_sw3, &GPIO_S, 100);
	s_ctx.s_previous_state = SW3_NEUTRAL;
}

/* UPDATE THE VOLTAGE READ ON S SELECTOR (CALLED BY ADC ROUTINE).
 * @param s_voltage_mv:	New voltage measured.
 * @return:				None.
 */
void S_SetVoltageMv(unsigned int s_voltage_mv) {
	SW3_SetVoltageMv(&s_ctx.s_sw3, s_voltage_mv);
}

/* MAIN ROUTINE OF S MODULE.
 * @param:	None.
 * @return:	None.
 */
void S_Task(void) {
	// Update current state.
	SW3_UpdateState(&s_ctx.s_sw3);
	// Perform actions according to state.
	switch (s_ctx.s_sw3.sw3_state) {
	case SW3_BACK:
		if (s_ctx.s_previous_state != SW3_BACK) {
			// Low tone.
			LSSGKCU_Send(LSSGKCU_IN_S_LOW_TONE);
		}
		break;
	case SW3_NEUTRAL:
		if (s_ctx.s_previous_state != SW3_NEUTRAL) {
			// Whistle off
			LSSGKCU_Send(LSSGKCU_IN_S_NEUTRAL);
		}
		break;
	case SW3_FRONT:
		if (s_ctx.s_previous_state != SW3_FRONT) {
			// High tone.
			LSSGKCU_Send(LSSGKCU_IN_S_HIGH_TONE);
		}
		break;
	default:
		// Unknown state.
		break;
	}
	// Update previous state.
	s_ctx.s_previous_state = s_ctx.s_sw3.sw3_state;
}
