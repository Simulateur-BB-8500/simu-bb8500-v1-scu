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
#include "mapping.h"
#include "sw3.h"

/*** FPB local structures ***/

typedef struct {
	SW3_Context fpb_sw3;
	SW3_State fpb_previous_state;
} FPB_Context;

/*** FPB external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** FPB local global variables ***/

static FPB_Context fpb_ctx;

/*** FPB functions ***/

/* INIT FPB MODULE.
 * @param:	None.
 * @return:	None.
 */
void FPB_Init(void) {
	// Init GPIO.
	SW3_Init(&fpb_ctx.fpb_sw3, &GPIO_FPB, 100);
	fpb_ctx.fpb_previous_state = SW3_NEUTRAL;
}

/* UPDATE THE VOLTAGE READ ON FPB SELECTOR (CALLED BY ADC ROUTINE).
 * @param new_voltage:	New voltage measured.
 * @return:				None.
 */
void FPB_SetVoltageMv(unsigned int fpb_voltage_mv) {
	SW3_SetVoltageMv(&fpb_ctx.fpb_sw3, fpb_voltage_mv);
}

/* MAIN ROUTINE OF FPB MODULE.
 * @param:	None.
 * @return:	None.
 */
void FPB_Task(void) {
	// Update current state.
	SW3_UpdateState(&fpb_ctx.fpb_sw3);
	// Check PBL2.
	if (lsmcu_ctx.lsmcu_pbl2_on != 0) {
		switch (fpb_ctx.fpb_sw3.sw3_state) {
		case SW3_BACK:
			if (fpb_ctx.fpb_previous_state != SW3_BACK) {
				// Backward.
				LSSGIU_Send(LSMCU_OUT_FPB_RELEASE);
			}
			break;
		case SW3_NEUTRAL:
			if (fpb_ctx.fpb_previous_state != SW3_NEUTRAL) {
				// Forward.
				LSSGIU_Send(LSMCU_OUT_FPB_NEUTRAL);
			}
			break;
		case SW3_FRONT:
			if (fpb_ctx.fpb_previous_state != SW3_FRONT) {
				// Forward.
				LSSGIU_Send(LSMCU_OUT_FPB_APPLY);
			}
			break;
		default:
			// Unknown state.
			break;
		}
	}
	// Update previous state.
	fpb_ctx.fpb_previous_state = fpb_ctx.fpb_sw3.sw3_state;

}

