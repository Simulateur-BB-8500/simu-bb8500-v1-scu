/*
 * fd.c
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#include "fd.h"

#include "adc.h"
#include "gpio.h"
#include "lsmcu.h"
#include "lsagiu.h"
#include "manometer.h"
#include "mapping.h"
#include "sw3.h"
#include "stdint.h"

/*** FD local macros ***/

#define FD_CF1_SPEED_MBAR_PER_SECOND	800
#define FD_CF2_SPEED_MBAR_PER_SECOND	1000

/*** FD local structures ***/

typedef struct {
	SW3_context_t sw3;
	SW3_state_t previous_state;
} FD_context_t;

/*** FD external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** FD local global variables ***/

static FD_context_t fd_ctx;

/*** FD functions ***/

/* INIT FD MODULE.
 * @param:	None.
 * @return:	None.
 */
void FD_init(void) {
	// Init GPIO.
	SW3_init(&fd_ctx.sw3, &GPIO_FD, 100, (uint32_t*) &(lsmcu_ctx.adc_data[ADC_DATA_INDEX_FD]));
	fd_ctx.previous_state = SW3_NEUTRAL;
}

/* MAIN ROUTINE OF FD MODULE.
 * @param:	None.
 * @return:	None.
 */
void FD_task(void) {
	// Update current state.
	SW3_update_state(&fd_ctx.sw3);
	// Perform actions according to state.
	switch (fd_ctx.sw3.state) {
	case SW3_BACK:
		if (fd_ctx.previous_state != SW3_BACK) {
			// Update CF1/CF2 manometers.
			MANOMETER_set_pressure(lsmcu_ctx.manometer_cf1, 0, FD_CF1_SPEED_MBAR_PER_SECOND);
			MANOMETER_set_pressure(lsmcu_ctx.manometer_cf2, 0, FD_CF2_SPEED_MBAR_PER_SECOND);
			// Send command.
			LSAGIU_Send(LSMCU_OUT_FD_RELEASE);
		}
		break;
	case SW3_NEUTRAL:
		if (fd_ctx.previous_state != SW3_NEUTRAL) {
			// Stop CF1/CF2 manometers.
			MANOMETER_needle_stop(lsmcu_ctx.manometer_cf1);
			MANOMETER_needle_stop(lsmcu_ctx.manometer_cf2);
			// Neutral.
			LSAGIU_Send(LSMCU_OUT_FD_NEUTRAL);
		}
		break;
	case SW3_FRONT:
		if (fd_ctx.previous_state != SW3_FRONT) {
			// Update CF1/CF2 manometers.
			MANOMETER_set_pressure(lsmcu_ctx.manometer_cf1, ((lsmcu_ctx.manometer_cf1) -> pressure_limit_mbar), FD_CF1_SPEED_MBAR_PER_SECOND);
			MANOMETER_set_pressure(lsmcu_ctx.manometer_cf2, ((lsmcu_ctx.manometer_cf2) -> pressure_limit_mbar), FD_CF2_SPEED_MBAR_PER_SECOND);
			// Forward.
			LSAGIU_Send(LSMCU_OUT_FD_APPLY);
		}
		break;
	default:
		// Unknown state.
		break;
	}
	// Update previous state.
	fd_ctx.previous_state = fd_ctx.sw3.state;
}

