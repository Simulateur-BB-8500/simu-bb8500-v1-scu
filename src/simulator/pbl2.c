/*
 * pbl2.c
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#include "pbl2.h"

#include "gpio.h"
#include "lsmcu.h"
#include "lssgiu.h"
#include "mapping.h"
#include "sw4.h"

/*** PBL2 local macros ***/

#define PBL2_MIN_CP_PRESSURE_DECIBARS	50
#define PBL2_ON_CG_PRESSURE_DECIBARS	35
#define PBL2_ON_RE_PRESSURE_DECIBARS	35

/*** PBL2 external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** PBL2 local global variables ***/

static SW4_context_t pbl2_sw4;

/*** PBL2 functions ***/

/* INIT PBL2 MODULE.
 * @param:	None.
 * @return:	None.
 */
void PBL2_init(void) {
	// Init GPIO.
	SW4_init(&pbl2_sw4, &GPIO_PBL2, 500);
	// Init global context.
	lsmcu_ctx.pbl2_on = 0;
}

/* UPDATE THE VOLTAGE READ ON PBL2 SELECTOR (CALLED BY ADC ROUTINE).
 * @param new_voltage:	New voltage measured.
 * @return:				None.
 */
void PBL2_set_voltage_mv(unsigned int pbl2_voltage_mv) {
	SW4_set_voltage_mv(&pbl2_sw4, pbl2_voltage_mv);
}

/* MAIN ROUTINE OF PBL2 MODULE.
 * @param:	None.
 * @return:	None.
 */
void PBL2_task(void) {
	// Update current state.
	SW4_update_state(&pbl2_sw4);
	// Perform actions according to state.
	switch (pbl2_sw4.state) {
	case SW4_P0:
		// Retrait.
		if (lsmcu_ctx.pbl2_on != 0) {
			// Send command on change.
			LSSGIU_Send(LSMCU_OUT_FPB_OFF);
			// Empty CG and RE.
			MANOMETER_set_pressure(lsmcu_ctx.manometer_cg, 0);
			MANOMETER_needle_start(lsmcu_ctx.manometer_cg);
			MANOMETER_set_pressure(lsmcu_ctx.manometer_re, 0);
			MANOMETER_needle_start(lsmcu_ctx.manometer_re);
			// Update global context.
			lsmcu_ctx.pbl2_on = 0;
		}
		break;
	case SW4_P1:
		// Isolement.
		break;
	case SW4_P2:
		// Service.
		if ((lsmcu_ctx.pbl2_on == 0) && (MANOMETER_get_pressure(lsmcu_ctx.manometer_cp) > PBL2_MIN_CP_PRESSURE_DECIBARS)) {
			// Send command on change.
			LSSGIU_Send(LSMCU_OUT_FPB_ON);
			// Start CG and RE manometers.
			MANOMETER_set_pressure(lsmcu_ctx.manometer_cg, PBL2_ON_CG_PRESSURE_DECIBARS);
			MANOMETER_needle_start(lsmcu_ctx.manometer_cg);
			MANOMETER_set_pressure(lsmcu_ctx.manometer_re, PBL2_ON_CG_PRESSURE_DECIBARS);
			MANOMETER_needle_start(lsmcu_ctx.manometer_re);
			// Update global context.
			lsmcu_ctx.pbl2_on = 1;
		}
		break;
	case SW4_P3:
		// Neutre.
		break;
	default:
		// Unknown state.
		break;
	}
}

