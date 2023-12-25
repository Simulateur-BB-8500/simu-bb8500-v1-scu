/*
 * fpb.c
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#include "fpb.h"

#include "adc.h"
#include "gpio.h"
#include "lsmcu.h"
#include "lsagiu.h"
#include "manometer.h"
#include "mapping.h"
#include "sw3.h"
#include "stdint.h"

/*** FPB local macros ***/

#define FPB_CG_RE_PRESSURE_MAX_MBAR		5000
#define FPB_CG_RE_SPEED_MBAR_PER_SECOND	500

/*** FPB local structures ***/

/*******************************************************************/
typedef struct {
	SW3_context_t sw3;
	SW3_state_t previous_state;
} FPB_context_t;

/*** FPB external global variables ***/

extern LSMCU_context_t lsmcu_ctx;

/*** FPB local global variables ***/

static FPB_context_t fpb_ctx;

/*** FPB functions ***/

/*******************************************************************/
void FPB_init(void) {
	// Init GPIO.
	SW3_init(&fpb_ctx.sw3, &GPIO_FPB, 100, (uint32_t*) &(lsmcu_ctx.adc_data[ADC_DATA_INDEX_FPB]));
	fpb_ctx.previous_state = SW3_NEUTRAL;
}

/*******************************************************************/
void FPB_process(void) {
	// Update current state.
	SW3_update_state(&fpb_ctx.sw3);
	// Check PBL2.
	if (lsmcu_ctx.pbl2_on != 0) {
		switch (fpb_ctx.sw3.state) {
		case SW3_BACK:
			if (fpb_ctx.previous_state != SW3_BACK) {
				// Backward.
				LSAGIU_write(LSMCU_OUT_FPB_RELEASE);
				// Start CG and RE manometers.
				MANOMETER_set_pressure(lsmcu_ctx.manometer_cg, FPB_CG_RE_PRESSURE_MAX_MBAR, FPB_CG_RE_SPEED_MBAR_PER_SECOND);
				MANOMETER_set_pressure(lsmcu_ctx.manometer_re, FPB_CG_RE_PRESSURE_MAX_MBAR, FPB_CG_RE_SPEED_MBAR_PER_SECOND);
			}
			break;
		case SW3_NEUTRAL:
			if (fpb_ctx.previous_state != SW3_NEUTRAL) {
				// Forward.
				LSAGIU_write(LSMCU_OUT_FPB_NEUTRAL);
				// Stop manometers.
				MANOMETER_needle_stop(lsmcu_ctx.manometer_cg);
				MANOMETER_needle_stop(lsmcu_ctx.manometer_re);
			}
			break;
		case SW3_FRONT:
			if (fpb_ctx.previous_state != SW3_FRONT) {
				// Forward.
				LSAGIU_write(LSMCU_OUT_FPB_APPLY);
				// Start CG and RE manometers.
				MANOMETER_set_pressure(lsmcu_ctx.manometer_cg, 0, FPB_CG_RE_SPEED_MBAR_PER_SECOND);
				MANOMETER_set_pressure(lsmcu_ctx.manometer_re, 0, FPB_CG_RE_SPEED_MBAR_PER_SECOND);
			}
			break;
		default:
			// Unknown state.
			break;
		}
	}
	// Update previous state.
	fpb_ctx.previous_state = fpb_ctx.sw3.state;
}
