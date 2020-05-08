/*
 * pbl2.c
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#include "pbl2.h"

#include "common.h"
#include "gpio.h"
#include "lssgkcu.h"
#include "mapping.h"
#include "sw4.h"

/*** PBL2 local structures ***/

typedef struct {
	SW4_Context pbl2_sw4;
	SW4_State pbl2_previous_state;
} PBL2_Context;

/*** PBL2 local global variables ***/

static PBL2_Context pbl2_ctx;

/*** PBL2 functions ***/

/* INIT PBL2 MODULE.
 * @param:	None.
 * @return:	None.
 */
void PBL2_Init(void) {
	// Init GPIO.
	SW4_Init(&pbl2_ctx.pbl2_sw4, &GPIO_PBL2, 500);
	pbl2_ctx.pbl2_previous_state = SW4_P0;
}

/* UPDATE THE VOLTAGE READ ON PBL2 SELECTOR (CALLED BY ADC ROUTINE).
 * @param new_voltage:	New voltage measured.
 * @return:				None.
 */
void PBL2_SetVoltageMv(unsigned int pbl2_voltage_mv) {
	SW4_SetVoltageMv(&pbl2_ctx.pbl2_sw4, pbl2_voltage_mv);
}

/* MAIN ROUTINE OF PBL2 MODULE.
 * @param:	None.
 * @return:	None.
 */
void PBL2_Task(void) {
	// Update current state.
	SW4_UpdateState(&pbl2_ctx.pbl2_sw4);
	// Perform actions according to state.
	switch (pbl2_ctx.pbl2_sw4.sw4_state) {
	case SW4_P0:
		// Retrait.
		if (pbl2_ctx.pbl2_previous_state != SW4_P0) {
			LSSGKCU_Send(LSMCU_OUT_FPB_OFF);
		}
		break;
	case SW4_P1:
		// Isolement.
		break;
	case SW4_P2:
		// Service.
		if (pbl2_ctx.pbl2_previous_state != SW4_P2) {
			LSSGKCU_Send(LSMCU_OUT_FPB_ON);
		}
		break;
	case SW4_P3:
		// Neutre.
		break;
	default:
		// Unknown state.
		break;
	}
	// Update previous state.
	pbl2_ctx.pbl2_previous_state = pbl2_ctx.pbl2_sw4.sw4_state;
}

