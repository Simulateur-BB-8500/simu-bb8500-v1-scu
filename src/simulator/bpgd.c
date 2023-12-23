/*
 * bpgd.c
 *
 *  Created on: 26 dec. 2021
 *      Author: Ludo
 */

#include "bpgd.h"

#include "gpio.h"
#include "lsmcu.h"
#include "lsagiu.h"
#include "manometer.h"
#include "mapping.h"
#include "sw2.h"
#include "tim.h"
#include "stdint.h"

/*** BPGD local macros ***/

#define BPGD_CG_RE_PRESSURE_MBAR			5000
#define BPGD_CG_RE_SPEED_MBAR_PER_SECOND	1000

#define BPGD_CF_SPEED_MBAR_PER_SECOND		1000

#define BPGD_INHIBIT_DELAY_MS				5000

/*** BPGD local structures ***/

/*******************************************************************/
typedef struct {
	SW2_context_t sw2;
	uint8_t enable;
	uint32_t inhibit_start_ms;
} BPGD_context_t;

/*** BPGD external global variables ***/

extern LSMCU_context_t lsmcu_ctx;

/*** BPGD local global variables ***/

static BPGD_context_t bpgd_ctx;

/*** BPGD functions ***/

/*******************************************************************/
void BPGD_init(void) {
	// Init GPIO.
	SW2_init(&bpgd_ctx.sw2, &GPIO_BPGD, 0, 100); // BPGD active low.
	// Init global context.
	bpgd_ctx.inhibit_start_ms = 0;
	bpgd_ctx.enable = 1;
}

/*******************************************************************/
void BPGD_process(void) {
	// Update BPGD state.
	SW2_update_state(&bpgd_ctx.sw2);
	if (bpgd_ctx.sw2.state == SW2_ON) {
		// Send commands on change.
		if (bpgd_ctx.enable != 0) {
			// Update manometers.
			MANOMETER_set_pressure(lsmcu_ctx.manometer_cg, BPGD_CG_RE_PRESSURE_MBAR, BPGD_CG_RE_SPEED_MBAR_PER_SECOND);
			MANOMETER_set_pressure(lsmcu_ctx.manometer_re, BPGD_CG_RE_PRESSURE_MBAR, BPGD_CG_RE_SPEED_MBAR_PER_SECOND);
			MANOMETER_set_pressure(lsmcu_ctx.manometer_cf1, 0, BPGD_CF_SPEED_MBAR_PER_SECOND);
			MANOMETER_set_pressure(lsmcu_ctx.manometer_cf2, 0, BPGD_CF_SPEED_MBAR_PER_SECOND);
			// Send sound command.
			LSAGIU_write(LSMCU_OUT_BPGD);
			// Disable module and start delay.
			bpgd_ctx.enable = 0;
			bpgd_ctx.inhibit_start_ms = TIM2_get_milliseconds();
		}
	}
	// Check delay.
	if (TIM2_get_milliseconds() > (bpgd_ctx.inhibit_start_ms + BPGD_INHIBIT_DELAY_MS)) {
		bpgd_ctx.enable = 1;
	}
}
