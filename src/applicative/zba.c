/*
 * zba.c
 *
 *  Created on: 6 feb. 2020
 *      Author: Ludo
 */

#include "zba.h"

#include "common.h"
#include "lssgkcu.h"
#include "mapping.h"
#include "sw2.h"

/*** ZBA local structures ***/

typedef struct {
	SW2_Context zba;
	unsigned char zba_closed;
} ZBA_Context;

/*** ZBA local global variables ***/

static ZBA_Context zba_ctx;

/*** ZBA functions ***/

/* INIT ZDA MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZBA_Init(void) {

	/* Init context */
	SW2_Init(&zba_ctx.zba, &GPIO_ZBA, 1, 100); // ZBA active high (+3.3V supply present).
	zba_ctx.zba_closed = 0;
}

/* MAIN TASK OF ZBA MODULE.
 * @param lsmcu_ctx:	Pointer to simulator context.
 * @return:				None.
 */
void ZBA_Task(LSMCU_Context* lsmcu_ctx) {

	/* Update ZBA state */
	SW2_UpdateState(&zba_ctx.zba);
	if (zba_ctx.zba.sw2_state == SW2_ON) {
		// Send command on change.
		if (zba_ctx.zba_closed == 0) {
			LSSGKCU_Send(LSSGKCU_IN_ZBA_ON);
		}
		zba_ctx.zba_closed = 1;
	}
	else {
		// Send command on change.
		if (zba_ctx.zba_closed != 0) {
			LSSGKCU_Send(LSSGKCU_IN_ZBA_OFF);
		}
		zba_ctx.zba_closed = 0;
	}
	(lsmcu_ctx -> lsmcu_zba_closed) = zba_ctx.zba_closed;
}
