/*
 * whistle.c
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#include "whistle.h"

#include "gpio.h"
#include "lssgiu.h"
#include "mapping.h"
#include "sw3.h"

/*** WHISTLE local structures ***/

typedef struct {
	SW3_Context whistle_sw3;
	SW3_State whistle_previouwhistle_state;
} WHISTLE_Context;

/*** WHISTLE local global variables ***/

static WHISTLE_Context whistle_ctx;

/*** WHISTLE functions ***/

/* INIT WHISTLE MODULE.
 * @param:	None.
 * @return:	None.
 */
void WHISTLE_Init(void) {
	// Init GPIO.
	SW3_Init(&whistle_ctx.whistle_sw3, &GPIO_WHISTLE, 100);
	whistle_ctx.whistle_previouwhistle_state = SW3_NEUTRAL;
}

/* UPDATE THE VOLTAGE READ ON WHISTLE SELECTOR (CALLED BY ADC ROUTINE).
 * @param whistle_voltage_mv:	New voltage measured.
 * @return:				None.
 */
void WHISTLE_SetVoltageMv(unsigned int whistle_voltage_mv) {
	SW3_SetVoltageMv(&whistle_ctx.whistle_sw3, whistle_voltage_mv);
}

/* MAIN ROUTINE OF WHISTLE MODULE.
 * @param:	None.
 * @return:	None.
 */
void WHISTLE_Task(void) {
	// Update current state.
	SW3_UpdateState(&whistle_ctx.whistle_sw3);
	// Perform actions according to state.
	switch (whistle_ctx.whistle_sw3.sw3_state) {
	case SW3_BACK:
		if (whistle_ctx.whistle_previouwhistle_state != SW3_BACK) {
			// Low tone.
			LSSGIU_Send(LSMCU_OUT_WHISTLE_LOW_TONE);
		}
		break;
	case SW3_NEUTRAL:
		if (whistle_ctx.whistle_previouwhistle_state != SW3_NEUTRAL) {
			// Whistle off
			LSSGIU_Send(LSMCU_OUT_WHISTLE_NEUTRAL);
		}
		break;
	case SW3_FRONT:
		if (whistle_ctx.whistle_previouwhistle_state != SW3_FRONT) {
			// High tone.
			LSSGIU_Send(LSMCU_OUT_WHISTLE_HIGH_TONE);
		}
		break;
	default:
		// Unknown state.
		break;
	}
	// Update previous state.
	whistle_ctx.whistle_previouwhistle_state = whistle_ctx.whistle_sw3.sw3_state;
}
