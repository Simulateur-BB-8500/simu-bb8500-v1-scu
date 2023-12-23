/*
 * whistle.c
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#include "whistle.h"

#include "gpio.h"
#include "lsmcu.h"
#include "lsagiu.h"
#include "mapping.h"
#include "sw3.h"
#include "stdint.h"

/*** WHISTLE local structures ***/

typedef struct {
	SW3_context_t sw3;
	SW3_state_t previous_state;
} WHISTLE_Context;

/*** WHISTLE external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** WHISTLE local global variables ***/

static WHISTLE_Context whistle_ctx;

/*** WHISTLE functions ***/

/* INIT WHISTLE MODULE.
 * @param:	None.
 * @return:	None.
 */
void WHISTLE_init(void) {
	// Init GPIO.
	SW3_init(&whistle_ctx.sw3, &GPIO_WHISTLE, 100, (uint32_t*) &(lsmcu_ctx.adc_data[ADC_DATA_INDEX_S]));
	whistle_ctx.previous_state = SW3_NEUTRAL;
}

/* MAIN ROUTINE OF WHISTLE MODULE.
 * @param:	None.
 * @return:	None.
 */
void WHISTLE_task(void) {
	// Update current state.
	SW3_update_state(&whistle_ctx.sw3);
	// Perform actions according to state.
	switch (whistle_ctx.sw3.state) {
	case SW3_BACK:
		if (whistle_ctx.previous_state != SW3_BACK) {
			// Low tone.
			LSAGIU_write(LSMCU_OUT_WHISTLE_LOW_TONE);
		}
		break;
	case SW3_NEUTRAL:
		if (whistle_ctx.previous_state != SW3_NEUTRAL) {
			// Whistle off
			LSAGIU_write(LSMCU_OUT_WHISTLE_NEUTRAL);
		}
		break;
	case SW3_FRONT:
		if (whistle_ctx.previous_state != SW3_FRONT) {
			// High tone.
			LSAGIU_write(LSMCU_OUT_WHISTLE_HIGH_TONE);
		}
		break;
	default:
		// Unknown state.
		break;
	}
	// Update previous state.
	whistle_ctx.previous_state = whistle_ctx.sw3.state;
}
