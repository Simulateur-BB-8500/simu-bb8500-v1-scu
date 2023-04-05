/*
 * fpb.c
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#include "fpb.h"

#include "gpio.h"
#include "lsmcu.h"
#include "lssgiu.h"
#include "manometer.h"
#include "mapping.h"
#include "sw3.h"
#include "stdint.h"

/*** FPB local macros ***/

#define FPB_CG_PRESSURE_MAX_DECIBARS	50

/*** FPB local structures ***/

typedef struct {
	SW3_context_t sw3;
	SW3_state_t previous_state;
} FPB_context_t;

/*** FPB external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** FPB local global variables ***/

static FPB_context_t fpb_ctx;

/*** FPB functions ***/

/* INIT FPB MODULE.
 * @param:	None.
 * @return:	None.
 */
void FPB_init(void) {
	// Init GPIO.
	SW3_init(&fpb_ctx.sw3, &GPIO_FPB, 100);
	fpb_ctx.previous_state = SW3_NEUTRAL;
}

/* UPDATE THE VOLTAGE READ ON FPB SELECTOR (CALLED BY ADC ROUTINE).
 * @param new_voltage:	New voltage measured.
 * @return:				None.
 */
void FPB_set_voltage_mv(uint32_t fpb_voltage_mv) {
	SW3_set_voltage_mv(&fpb_ctx.sw3, fpb_voltage_mv);
}

/* MAIN ROUTINE OF FPB MODULE.
 * @param:	None.
 * @return:	None.
 */
void FPB_task(void) {
	// Update current state.
	SW3_update_state(&fpb_ctx.sw3);
	// Check PBL2.
	if (lsmcu_ctx.pbl2_on != 0) {
		switch (fpb_ctx.sw3.state) {
		case SW3_BACK:
			if (fpb_ctx.previous_state != SW3_BACK) {
				// Backward.
				LSSGIU_Send(LSMCU_OUT_FPB_RELEASE);
				// Start CG and RE manometers.
				MANOMETER_set_pressure(lsmcu_ctx.manometer_cg, FPB_CG_PRESSURE_MAX_DECIBARS);
				MANOMETER_needle_start(lsmcu_ctx.manometer_cg);
				MANOMETER_set_pressure(lsmcu_ctx.manometer_re, (lsmcu_ctx.manometer_re) -> pressure_limit_decibars);
				MANOMETER_needle_start(lsmcu_ctx.manometer_re);
			}
			break;
		case SW3_NEUTRAL:
			if (fpb_ctx.previous_state != SW3_NEUTRAL) {
				// Forward.
				LSSGIU_Send(LSMCU_OUT_FPB_NEUTRAL);
				// Stop manometers.
				MANOMETER_needle_stop(lsmcu_ctx.manometer_cg);
				MANOMETER_needle_stop(lsmcu_ctx.manometer_re);
			}
			break;
		case SW3_FRONT:
			if (fpb_ctx.previous_state != SW3_FRONT) {
				// Forward.
				LSSGIU_Send(LSMCU_OUT_FPB_APPLY);
				// Start CG and RE manometers.
				MANOMETER_set_pressure(lsmcu_ctx.manometer_cg, 0);
				MANOMETER_needle_start(lsmcu_ctx.manometer_cg);
				MANOMETER_set_pressure(lsmcu_ctx.manometer_re, 0);
				MANOMETER_needle_start(lsmcu_ctx.manometer_re);
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

