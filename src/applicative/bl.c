/*
 * bl.c
 *
 *  Created on: 8 apr. 2018
 *      Author: Ludo
 */

#include "bl.h"

#include "gpio.h"
#include "kvb.h"
#include "lsmcu.h"
#include "lssgkcu.h"
#include "mapping.h"
#include "sw2.h"

/*** BL local structures ***/

typedef struct {
	SW2_Context bl_zdv;
	SW2_Context bl_zdj;
	SW2_Context bl_zen;
	unsigned char bl_zen_on;
	SW2_Context bl_zvm;
	unsigned char bl_zvm_on;
	SW2_Context bl_zfg;
	unsigned char bl_zfg_on;
	SW2_Context bl_zfd;
	unsigned char bl_zfd_on;
	SW2_Context bl_zpr;
	unsigned char bl_zpr_on;
} BL_Context;

/*** BL external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** BL local global variables ***/

static BL_Context bl_ctx;

/*** BL functions ***/

/* INIT BL MODULE.
 * @param:	None.
 * @return:	None.
 */
void BL_Init(void) {
	// Init GPIOs.
	SW2_Init(&bl_ctx.bl_zdv, &GPIO_BL_ZDV, 0, 100); // ZDV active low.
	SW2_Init(&bl_ctx.bl_zdj, &GPIO_BL_ZDJ, 1, 100); // ZDJ active high.
	SW2_Init(&bl_ctx.bl_zen, &GPIO_BL_ZEN, 1, 100); // ZEN active high.
	bl_ctx.bl_zen_on = 0;
	SW2_Init(&bl_ctx.bl_zvm, &GPIO_BL_ZVM, 0, 100); // ZVM active low.
	bl_ctx.bl_zvm_on = 0;
	SW2_Init(&bl_ctx.bl_zfg, &GPIO_BL_ZFG, 0, 100); // ZFG active low.
	bl_ctx.bl_zfg_on = 0;
	SW2_Init(&bl_ctx.bl_zfd, &GPIO_BL_ZFD, 0, 100); // ZFD active low.
	bl_ctx.bl_zfd_on = 0;
	SW2_Init(&bl_ctx.bl_zpr, &GPIO_BL_ZPR, 0, 100); // ZFD active low.
	bl_ctx.bl_zpr_on = 0;
	// Init global context.
	lsmcu_ctx.lsmcu_bl_unlocked = 0;
	lsmcu_ctx.lsmcu_dj_closed = 0;
	lsmcu_ctx.lsmcu_dj_locked = 0;
}

/* MAIN ROUTINE OF BL MODULE.
 * @param:	None.
 * @return:	None.
 */
void BL_Task(void) {
	// ZDV.
	SW2_UpdateState(&bl_ctx.bl_zdv);
	if ((bl_ctx.bl_zdv.sw2_state == SW2_ON) && (lsmcu_ctx.lsmcu_zba_closed != 0)) {
		// Send command on change.
		if (lsmcu_ctx.lsmcu_bl_unlocked == 0) {
			LSSGKCU_Send(LSMCU_OUT_ZDV_ON);
			KVB_StartSweepTimer();
		}
		lsmcu_ctx.lsmcu_bl_unlocked = 1;
	}
	else {
		// Send command on change.
		if (lsmcu_ctx.lsmcu_bl_unlocked != 0) {
			LSSGKCU_Send(LSMCU_OUT_ZDV_OFF);
			KVB_StopSweepTimer();
		}
		lsmcu_ctx.lsmcu_bl_unlocked = 0;
	}
	// ZDJ.
	SW2_UpdateState(&bl_ctx.bl_zdj);
	if (lsmcu_ctx.lsmcu_zpt_raised != 0) {
		if (bl_ctx.bl_zdj.sw2_state == SW2_ON) {
			lsmcu_ctx.lsmcu_dj_closed = 1;
		}
		else {
			// Send command on change (only if DJ is locked).
			if (lsmcu_ctx.lsmcu_dj_closed != 0) {
				if (lsmcu_ctx.lsmcu_dj_locked != 0) {
					LSSGKCU_Send(LSMCU_OUT_ZDJ_OFF);
				}
				lsmcu_ctx.lsmcu_dj_locked = 0;
			}
			lsmcu_ctx.lsmcu_dj_closed = 0;
		}
	}
	else {
		if (lsmcu_ctx.lsmcu_dj_closed != 0) {
			LSSGKCU_Send(LSMCU_OUT_ZDJ_OFF);
		}
		lsmcu_ctx.lsmcu_dj_closed = 0; // Hack.
		lsmcu_ctx.lsmcu_dj_locked = 0;
	}
	// ZEN.
	SW2_UpdateState(&bl_ctx.bl_zen);
	if ((lsmcu_ctx.lsmcu_zpt_raised != 0) && (lsmcu_ctx.lsmcu_dj_closed != 0)) {
		if (bl_ctx.bl_zen.sw2_state == SW2_ON) {
			// Send command on change.
			if ((bl_ctx.bl_zen_on == 0) && (lsmcu_ctx.lsmcu_dj_locked == 0)) {
				LSSGKCU_Send(LSMCU_OUT_ZEN_ON);
				lsmcu_ctx.lsmcu_dj_locked = 1;
			}
			bl_ctx.bl_zen_on = 1;
		}
		else {
			bl_ctx.bl_zen_on = 0;
		}
	}
	// ZVM.
	SW2_UpdateState(&bl_ctx.bl_zvm);
	if ((lsmcu_ctx.lsmcu_dj_locked != 0) && (bl_ctx.bl_zvm.sw2_state == SW2_ON)) {
		// Send command on change.
		if (bl_ctx.bl_zvm_on == 0) {
			LSSGKCU_Send(LSMCU_OUT_ZVM_ON);
		}
		bl_ctx.bl_zvm_on = 1;
	}
	else {
		// Send command on change.
		if (bl_ctx.bl_zvm_on != 0) {
			LSSGKCU_Send(LSMCU_OUT_ZVM_OFF);
		}
		bl_ctx.bl_zvm_on = 0;
	}
	// ZFG.
	SW2_UpdateState(&bl_ctx.bl_zfg);
	if ((bl_ctx.bl_zfg.sw2_state == SW2_ON) && (lsmcu_ctx.lsmcu_zba_closed != 0)) {
		// Send command on change.
		if (bl_ctx.bl_zfg_on == 0) {
			LSSGKCU_Send(LSMCU_OUT_ZFG_ON);
		}
		bl_ctx.bl_zfg_on = 1;
	}
	else {
		// Send command on change.
		if (bl_ctx.bl_zfg_on != 0) {
			LSSGKCU_Send(LSMCU_OUT_ZFG_OFF);
		}
		bl_ctx.bl_zfg_on = 0;
	}
	// ZFD.
	SW2_UpdateState(&bl_ctx.bl_zfd);
	if ((bl_ctx.bl_zfd.sw2_state == SW2_ON) && (lsmcu_ctx.lsmcu_zba_closed != 0)) {
		// Send command on change.
		if (bl_ctx.bl_zfd_on == 0) {
			LSSGKCU_Send(LSMCU_OUT_ZFD_ON);
		}
		bl_ctx.bl_zfd_on = 1;
	}
	else {
		// Send command on change.
		if (bl_ctx.bl_zfd_on != 0) {
			LSSGKCU_Send(LSMCU_OUT_ZFD_OFF);
		}
		bl_ctx.bl_zfd_on = 0;
	}
	// ZPR.
	SW2_UpdateState(&bl_ctx.bl_zpr);
	if ((bl_ctx.bl_zpr.sw2_state == SW2_ON) && (lsmcu_ctx.lsmcu_zba_closed != 0)) {
		// Send command on change.
		if (bl_ctx.bl_zpr_on == 0) {
			LSSGKCU_Send(LSMCU_OUT_ZPR_ON);
		}
		bl_ctx.bl_zpr_on = 1;
	}
	else {
		// Send command on change.
		if (bl_ctx.bl_zpr_on != 0) {
			LSSGKCU_Send(LSMCU_OUT_ZPR_OFF);
		}
		bl_ctx.bl_zpr_on = 0;
	}
}
