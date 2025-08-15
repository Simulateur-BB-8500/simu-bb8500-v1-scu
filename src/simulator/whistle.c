/*
 * whistle.c
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#include "whistle.h"

#include "gpio.h"
#include "mapping.h"
#include "scu.h"
#include "sgdu.h"
#include "sw3.h"
#include "stdint.h"

/*** WHISTLE local structures ***/

/*******************************************************************/
typedef struct {
	SW3_context_t sw3;
	SW3_state_t previous_state;
} WHISTLE_context_t;

/*** WHISTLE external global variables ***/

extern SCU_context_t scu_ctx;

/*** WHISTLE local global variables ***/

static WHISTLE_context_t whistle_ctx;

/*** WHISTLE functions ***/

/*******************************************************************/
void WHISTLE_init(void) {
	// Init GPIO.
	SW3_init(&whistle_ctx.sw3, &GPIO_WHISTLE, 100, (uint32_t*) &(scu_ctx.adc_data[ADC_DATA_INDEX_S]));
	whistle_ctx.previous_state = SW3_NEUTRAL;
}

/*******************************************************************/
void WHISTLE_process(void) {
	// Update current state.
	SW3_update_state(&whistle_ctx.sw3);
	// Perform actions according to state.
	switch (whistle_ctx.sw3.state) {
	case SW3_BACK:
		if (whistle_ctx.previous_state != SW3_BACK) {
			// Low tone.
			SGDU_write(SCU_OUT_WHISTLE_LOW_TONE);
		}
		break;
	case SW3_NEUTRAL:
		if (whistle_ctx.previous_state != SW3_NEUTRAL) {
			// Whistle off
			SGDU_write(SCU_OUT_WHISTLE_NEUTRAL);
		}
		break;
	case SW3_FRONT:
		if (whistle_ctx.previous_state != SW3_FRONT) {
			// High tone.
			SGDU_write(SCU_OUT_WHISTLE_HIGH_TONE);
		}
		break;
	default:
		// Unknown state.
		break;
	}
	// Update previous state.
	whistle_ctx.previous_state = whistle_ctx.sw3.state;
}
