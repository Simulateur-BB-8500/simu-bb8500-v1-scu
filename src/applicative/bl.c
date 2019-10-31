/*
 * bl.c
 *
 *  Created on: 8 avr. 2018
 *      Author: Ludovic
 */

#include "bl.h"

#include "gpio.h"
#include "lssgkcu.h"
#include "mapping.h"
#include "sw2.h"

/*** BL local structures ***/

typedef struct {
	SW2_Context bl_zdv;
	unsigned char bl_zdv_on;
	SW2_Context bl_zdj;
	unsigned char bl_zdj_on;
	SW2_Context bl_zen;
	unsigned char bl_zen_on;
	SW2_Context bl_zvm;
	unsigned char bl_zvm_on;
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
	SW2_Init(&bl_ctx.bl_zdv, &GPIO_BL_ZDV, 0, 100); // ZDV active low = 0.
	bl_ctx.bl_zdv_on = 0;
	SW2_Init(&bl_ctx.bl_zdj, &GPIO_BL_ZDJ, 0, 100);
	bl_ctx.bl_zdj_on = 0;
	SW2_Init(&bl_ctx.bl_zen, &GPIO_BL_ZEN, 0, 100);
	bl_ctx.bl_zen_on = 0;
	SW2_Init(&bl_ctx.bl_zvm, &GPIO_BL_ZVM, 0, 100);
	bl_ctx.bl_zvm_on = 0;
}

/* MAIN ROUTINE OF BL MODULE.
 * @param:	None.
 * @return:	None.
 */
void BL_Task(unsigned char* bl_unlocked) {

	/* ZDV */
	SW2_UpdateState(&bl_ctx.bl_zdv);
	if (bl_ctx.bl_zdv.sw2_state == SW2_ON) {
		if (bl_ctx.bl_zdv_on == 0) LSSGKCU_Send(LSSGKCU_IN_ZDV_ON);
		bl_ctx.bl_zdv_on = 1;
	}
	else {
		if (bl_ctx.bl_zdv_on != 0) LSSGKCU_Send(LSSGKCU_IN_ZDV_OFF);
		bl_ctx.bl_zdv_on = 0;
	}
	(*bl_unlocked) = bl_ctx.bl_zdv_on;

	/* ZDJ */
	SW2_UpdateState(&bl_ctx.bl_zdj);
	if (bl_ctx.bl_zdj.sw2_state == SW2_ON) {
		bl_ctx.bl_zdj_on = 1;
	}
	else {
		if (bl_ctx.bl_zdj_on != 0) LSSGKCU_Send(LSSGKCU_IN_ZDJ_OFF);
		bl_ctx.bl_zdj_on = 0;
	}

	/* ZEN */
	SW2_UpdateState(&bl_ctx.bl_zen);
	if (bl_ctx.bl_zen.sw2_state == SW2_ON) {
		if (bl_ctx.bl_zen_on == 0) LSSGKCU_Send(LSSGKCU_IN_ZEN_ON);
		bl_ctx.bl_zen_on = 1;
	}
	else {
		bl_ctx.bl_zen_on = 0;
	}

	/* ZVM */
	SW2_UpdateState(&bl_ctx.bl_zvm);
	if (bl_ctx.bl_zvm.sw2_state == SW2_ON) {
		if (bl_ctx.bl_zvm_on == 0) LSSGKCU_Send(LSSGKCU_IN_ZVM_ON);
		bl_ctx.bl_zvm_on = 1;
	}
	else {
		if (bl_ctx.bl_zvm_on != 0) LSSGKCU_Send(LSSGKCU_IN_ZVM_OFF);
		bl_ctx.bl_zvm_on = 0;
	}
}
