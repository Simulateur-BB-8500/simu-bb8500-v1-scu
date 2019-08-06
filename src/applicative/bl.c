/*
 * bl.c
 *
 *  Created on: 8 avr. 2018
 *      Author: Ludovic
 */

#include "bl.h"

#include "gpio.h"
#include "mano.h"
#include "mapping.h"
#include "sw2.h"

/*** BL local structures ***/

typedef struct {
	SW2_Context zdv;
	unsigned char bl_unlocked;
	SW2_Context zdj;
	unsigned char bl_dj_on;

} BL_Context;

/*** BL local global variables ***/

static BL_Context bl_ctx;

/*** BL functions ***/

/* INIT BL MODULE.
 * @param:	None.
 * @return:	None.
 */
void BL_Init(void) {

	/* Init context */
	SW2_Init(&bl_ctx.zdv, &GPIO_BL_ZDV, 0, 100); // ZDV active low = 0.
	bl_ctx.bl_unlocked = 0;
	SW2_Init(&bl_ctx.zdj, &GPIO_BL_ZDJ, 0, 100);
	bl_ctx.bl_dj_on = 0;
}

/* MAIN ROUTINE OF BL MODULE.
 * @param:	None.
 * @return:	None.
 */
void BL_Task(unsigned char* bl_unlocked) {

	// ZDV.
	SW2_UpdateState(&bl_ctx.zdv);
	if (bl_ctx.zdv.sw2_state == SW2_ON) {
		if (bl_ctx.bl_unlocked == 0) {
			MANO_SetTarget(&mano_cp, 50);
			MANO_StartNeedle(&mano_cp);
		}
		bl_ctx.bl_unlocked = 1;
	}
	else {
		if (bl_ctx.bl_unlocked != 0) {
			MANO_StopNeedle(&mano_cp);
		}
		bl_ctx.bl_unlocked = 0;
	}
	(*bl_unlocked) = bl_ctx.bl_unlocked;

	// ZDJ.
	SW2_UpdateState(&bl_ctx.zdj);
	if (bl_ctx.zdj.sw2_state == SW2_ON) {
		if (bl_ctx.bl_dj_on == 0) {
			MANO_SetTarget(&mano_cp, 0);
			MANO_StartNeedle(&mano_cp);
		}
		bl_ctx.bl_dj_on = 1;
	}
	else {
		if (bl_ctx.bl_dj_on != 0) {
			MANO_StopNeedle(&mano_cp);
		}
		bl_ctx.bl_dj_on = 0;
	}
}
