/*
 * bpsa.c
 *
 *  Created on: 02 jan. 2024
 *      Author: Ludo
 */

#include "bpsa.h"

#include "gpio.h"
#include "lsagiu.h"
#include "mapping.h"
#include "sw2.h"
#include "stdint.h"

/*** BPSA local structures ***/

/*******************************************************************/
typedef enum {
	BPSA_STATE_OFF = 0,
	BPSA_STATE_ON,
	BPSA_STATE_LAST
} BPSA_state_t;

/*******************************************************************/
typedef struct {
	BPSA_state_t state;
	SW2_context_t button_on;
	SW2_context_t button_off;
} BPSA_context_t;

/*** BPSA local global variables ***/

static BPSA_context_t bpsa_ctx;

/*** BPSA functions ***/

/*******************************************************************/
void BPSA_init(void) {
	// Init context.
	bpsa_ctx.state = BPSA_STATE_OFF;
	// Init GPIO.
	SW2_init(&bpsa_ctx.button_on, &GPIO_BPSA, 0, 500); // BPSA active low.
	SW2_init(&bpsa_ctx.button_off, &GPIO_BPASA, 0, 500); // BP(A)SA active low.
}

/*******************************************************************/
void BPSA_process(void) {
	// Update buttons state.
	SW2_update_state(&bpsa_ctx.button_on);
	SW2_update_state(&bpsa_ctx.button_off);
	// Check buttons.
	if ((bpsa_ctx.button_on.state == SW2_ON) && (bpsa_ctx.button_off.state == SW2_OFF) && (bpsa_ctx.state != BPSA_STATE_ON)) {
		// Send command and update state.
		LSAGIU_write(LSMCU_OUT_BPSA_ON);
		bpsa_ctx.state = BPSA_STATE_ON;
	}
	if ((bpsa_ctx.button_off.state == SW2_ON) && (bpsa_ctx.button_on.state == SW2_OFF) && (bpsa_ctx.state != BPSA_STATE_OFF)) {
		// Send command and update state.
		LSAGIU_write(LSMCU_OUT_BPSA_OFF);
		bpsa_ctx.state = BPSA_STATE_OFF;
	}
}
