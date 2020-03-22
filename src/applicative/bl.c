/*
 * bl.c
 *
 *  Created on: 8 apr. 2018
 *      Author: Ludo
 */

#include "bl.h"

#include "common.h"
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
	unsigned char bl_dj_locked;
	SW2_Context bl_zca;
	unsigned char bl_zca_on;
	SW2_Context bl_zcd;
	unsigned char bl_zcd_on;
	SW2_Context bl_zvm;
	unsigned char bl_zvm_on;
	SW2_Context bl_zfg;
	unsigned char bl_zfg_on;
	SW2_Context bl_zfd;
	unsigned char bl_zfd_on;
	SW2_Context bl_zpr;
	unsigned char bl_zpr_on;
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
	SW2_Init(&bl_ctx.bl_zdv, &GPIO_BL_ZDV, 0, 100); // ZDV active low.
	bl_ctx.bl_zdv_on = 0;
	SW2_Init(&bl_ctx.bl_zdj, &GPIO_BL_ZDJ, 1, 100); // ZDJ active high.
	bl_ctx.bl_zdj_on = 0;
	SW2_Init(&bl_ctx.bl_zen, &GPIO_BL_ZEN, 1, 100); // ZEN active high.
	bl_ctx.bl_zen_on = 0;
	bl_ctx.bl_dj_locked = 0;
	SW2_Init(&bl_ctx.bl_zca, &GPIO_BL_ZCA, 1, 100); // ZCA active high.
	bl_ctx.bl_zca_on = 0;
	SW2_Init(&bl_ctx.bl_zcd, &GPIO_BL_ZCD, 0, 100); // ZCD active low.
	bl_ctx.bl_zcd_on = 0;
	SW2_Init(&bl_ctx.bl_zvm, &GPIO_BL_ZVM, 0, 100); // ZVM active low.
	bl_ctx.bl_zvm_on = 0;
	SW2_Init(&bl_ctx.bl_zfg, &GPIO_BL_ZFG, 0, 100); // ZFG active low.
	bl_ctx.bl_zfg_on = 0;
	SW2_Init(&bl_ctx.bl_zfd, &GPIO_BL_ZFD, 0, 100); // ZFD active low.
	bl_ctx.bl_zfd_on = 0;
	SW2_Init(&bl_ctx.bl_zpr, &GPIO_BL_ZPR, 0, 100); // ZFD active low.
	bl_ctx.bl_zpr_on = 0;
}

/* MAIN ROUTINE OF BL MODULE.
 * @param lsmcu_ctx:	Pointer to simulator context.
 * @return:				None.
 */
void BL_Task(LSMCU_Context* lsmcu_ctx) {

	/* ZDV */
	SW2_UpdateState(&bl_ctx.bl_zdv);
	if (bl_ctx.bl_zdv.sw2_state == SW2_ON) {
		// Send command on change.
		if (bl_ctx.bl_zdv_on == 0) {
			LSSGKCU_Send(LSSGKCU_IN_ZDV_ON);
		}
		bl_ctx.bl_zdv_on = 1;
	}
	else {
		// Send command on change.
		if (bl_ctx.bl_zdv_on != 0) {
			LSSGKCU_Send(LSSGKCU_IN_ZDV_OFF);
		}
		bl_ctx.bl_zdv_on = 0;
	}
	(lsmcu_ctx -> lsmcu_bl_unlocked) = bl_ctx.bl_zdv_on;

	/* ZDJ */
	if ((lsmcu_ctx -> lsmcu_zpt_raised) != 0) {
		SW2_UpdateState(&bl_ctx.bl_zdj);
		if (bl_ctx.bl_zdj.sw2_state == SW2_ON) {
			bl_ctx.bl_zdj_on = 1;
		}
		else {
			// Send command on change.
			if (bl_ctx.bl_zdj_on != 0) {
				LSSGKCU_Send(LSSGKCU_IN_ZDJ_OFF);
				bl_ctx.bl_dj_locked = 0;
			}
			bl_ctx.bl_zdj_on = 0;
		}
	}
	else {
		if (bl_ctx.bl_zdj_on != 0) {
			LSSGKCU_Send(LSSGKCU_IN_ZDJ_OFF);
		}
		bl_ctx.bl_zdj_on = 0; // Virtual...
		bl_ctx.bl_dj_locked = 0;
	}
	(lsmcu_ctx -> lsmcu_dj_closed) = bl_ctx.bl_zdj_on;

	/* ZEN */
	if (((lsmcu_ctx -> lsmcu_zpt_raised) != 0) && (bl_ctx.bl_zdj_on != 0)) {
		SW2_UpdateState(&bl_ctx.bl_zen);
		if (bl_ctx.bl_zen.sw2_state == SW2_ON) {
			// Send command on change.
			if ((bl_ctx.bl_zen_on == 0) && (bl_ctx.bl_dj_locked == 0)) {
				LSSGKCU_Send(LSSGKCU_IN_ZEN_ON);
				bl_ctx.bl_dj_locked = 1;
			}
			bl_ctx.bl_zen_on = 1;
		}
		else {
			bl_ctx.bl_zen_on = 0;
		}
	}
	(lsmcu_ctx -> lsmcu_dj_locked) = bl_ctx.bl_dj_locked;

	/* ZCA */
	if (bl_ctx.bl_dj_locked != 0) {
		SW2_UpdateState(&bl_ctx.bl_zca);
		if (bl_ctx.bl_zca.sw2_state == SW2_ON) {
			// Send command on change.
			if (bl_ctx.bl_zca_on == 0) {
				LSSGKCU_Send(LSSGKCU_IN_ZCA_ON);
			}
			bl_ctx.bl_zca_on = 1;
		}
		else {
			// Send command on change.
			if (bl_ctx.bl_zca_on != 0) {
				LSSGKCU_Send(LSSGKCU_IN_ZCA_OFF);
			}
			bl_ctx.bl_zca_on = 0;
		}
	}

	/* ZCD */
	if (bl_ctx.bl_dj_locked != 0) {
		SW2_UpdateState(&bl_ctx.bl_zcd);
		if (bl_ctx.bl_zcd.sw2_state == SW2_ON) {
			// Send command on change.
			if (bl_ctx.bl_zcd_on == 0) {
				LSSGKCU_Send(LSSGKCU_IN_ZCD_ON);
			}
			bl_ctx.bl_zcd_on = 1;
		}
		else {
			// Send command on change.
			if (bl_ctx.bl_zcd_on != 0) {
				LSSGKCU_Send(LSSGKCU_IN_ZCD_OFF);
			}
			bl_ctx.bl_zcd_on = 0;
		}
	}

	/* ZVM */
	if (bl_ctx.bl_dj_locked != 0) {
		SW2_UpdateState(&bl_ctx.bl_zvm);
		if (bl_ctx.bl_zvm.sw2_state == SW2_ON) {
			// Send command on change.
			if (bl_ctx.bl_zvm_on == 0) {
				LSSGKCU_Send(LSSGKCU_IN_ZVM_ON);
			}
			bl_ctx.bl_zvm_on = 1;
		}
		else {
			// Send command on change.
			if (bl_ctx.bl_zvm_on != 0) {
				LSSGKCU_Send(LSSGKCU_IN_ZVM_OFF);
			}
			bl_ctx.bl_zvm_on = 0;
		}
	}
}
