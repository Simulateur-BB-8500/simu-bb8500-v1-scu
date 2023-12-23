/*
 * bl.c
 *
 *  Created on: 8 apr. 2018
 *      Author: Ludo
 */

#include "bl.h"

#include "gpio.h"
#include "lsmcu.h"
#include "lsagiu.h"
#include "mapping.h"
#include "sw2.h"
#include "stdint.h"

/*** BL local structures ***/

/*******************************************************************/
typedef struct {
	SW2_context_t zdv;
	SW2_context_t zdj;
	SW2_context_t zen;
	uint8_t zen_on;
	SW2_context_t zvm;
	uint8_t zvm_on;
	SW2_context_t zfg;
	uint8_t zfg_on;
	SW2_context_t zfd;
	uint8_t zfd_on;
	SW2_context_t zpr;
	uint8_t zpr_on;
} BL_context_t;

/*** BL external global variables ***/

extern LSMCU_context_t lsmcu_ctx;

/*** BL local global variables ***/

static BL_context_t bl_ctx;

/*** BL functions ***/

/*******************************************************************/
void BL_init(void) {
	// Init GPIOs.
	SW2_init(&bl_ctx.zdv, &GPIO_BL_ZDV, 0, 100); // ZDV active low.
	SW2_init(&bl_ctx.zdj, &GPIO_BL_ZDJ, 1, 100); // ZDJ active high.
	SW2_init(&bl_ctx.zen, &GPIO_BL_ZEN, 1, 100); // ZEN active high.
	bl_ctx.zen_on = 0;
	SW2_init(&bl_ctx.zvm, &GPIO_BL_ZVM, 0, 100); // ZVM active low.
	bl_ctx.zvm_on = 0;
	SW2_init(&bl_ctx.zfg, &GPIO_BL_ZFG, 0, 100); // ZFG active low.
	bl_ctx.zfg_on = 0;
	SW2_init(&bl_ctx.zfd, &GPIO_BL_ZFD, 0, 100); // ZFD active low.
	bl_ctx.zfd_on = 0;
	SW2_init(&bl_ctx.zpr, &GPIO_BL_ZPR, 0, 100); // ZFD active low.
	bl_ctx.zpr_on = 0;
	// Init global context.
	lsmcu_ctx.bl_unlocked = 0;
	lsmcu_ctx.dj_closed = 0;
	lsmcu_ctx.dj_locked = 0;
}

/*******************************************************************/
void BL_process(void) {
	// ZDV.
	SW2_update_state(&bl_ctx.zdv);
	if ((bl_ctx.zdv.state == SW2_ON) && (lsmcu_ctx.zba_closed != 0)) {
		// Send command on change.
		if (lsmcu_ctx.bl_unlocked == 0) {
			LSAGIU_write(LSMCU_OUT_ZDV_ON);
		}
		lsmcu_ctx.bl_unlocked = 1;
	}
	else {
		// Send command on change.
		if (lsmcu_ctx.bl_unlocked != 0) {
			LSAGIU_write(LSMCU_OUT_ZDV_OFF);
		}
		lsmcu_ctx.bl_unlocked = 0;
	}
	// ZDJ.
	SW2_update_state(&bl_ctx.zdj);
	if (lsmcu_ctx.zpt_raised != 0) {
		if (bl_ctx.zdj.state == SW2_ON) {
			lsmcu_ctx.dj_closed = 1;
		}
		else {
			// Send command on change (only if DJ is locked).
			if (lsmcu_ctx.dj_closed != 0) {
				if (lsmcu_ctx.dj_locked != 0) {
					LSAGIU_write(LSMCU_OUT_ZDJ_OFF);
				}
				lsmcu_ctx.dj_locked = 0;
			}
			lsmcu_ctx.dj_closed = 0;
		}
	}
	else {
		if (lsmcu_ctx.dj_closed != 0) {
			LSAGIU_write(LSMCU_OUT_ZDJ_OFF);
		}
		lsmcu_ctx.dj_closed = 0; // Hack.
		lsmcu_ctx.dj_locked = 0;
	}
	// ZEN.
	SW2_update_state(&bl_ctx.zen);
	if ((lsmcu_ctx.zpt_raised != 0) && (lsmcu_ctx.dj_closed != 0)) {
		if (bl_ctx.zen.state == SW2_ON) {
			// Send command on change.
			if ((bl_ctx.zen_on == 0) && (lsmcu_ctx.dj_locked == 0)) {
				LSAGIU_write(LSMCU_OUT_ZEN_ON);
				lsmcu_ctx.dj_locked = 1;
			}
			bl_ctx.zen_on = 1;
		}
		else {
			bl_ctx.zen_on = 0;
		}
	}
	// ZVM.
	SW2_update_state(&bl_ctx.zvm);
	if ((lsmcu_ctx.dj_locked != 0) && (bl_ctx.zvm.state == SW2_ON)) {
		// Send command on change.
		if (bl_ctx.zvm_on == 0) {
			LSAGIU_write(LSMCU_OUT_ZVM_ON);
		}
		bl_ctx.zvm_on = 1;
	}
	else {
		// Send command on change.
		if (bl_ctx.zvm_on != 0) {
			LSAGIU_write(LSMCU_OUT_ZVM_OFF);
		}
		bl_ctx.zvm_on = 0;
	}
	// ZFG.
	SW2_update_state(&bl_ctx.zfg);
	if ((bl_ctx.zfg.state == SW2_ON) && (lsmcu_ctx.zba_closed != 0)) {
		// Send command on change.
		if (bl_ctx.zfg_on == 0) {
			LSAGIU_write(LSMCU_OUT_ZFG_ON);
		}
		bl_ctx.zfg_on = 1;
	}
	else {
		// Send command on change.
		if (bl_ctx.zfg_on != 0) {
			LSAGIU_write(LSMCU_OUT_ZFG_OFF);
		}
		bl_ctx.zfg_on = 0;
	}
	// ZFD.
	SW2_update_state(&bl_ctx.zfd);
	if ((bl_ctx.zfd.state == SW2_ON) && (lsmcu_ctx.zba_closed != 0)) {
		// Send command on change.
		if (bl_ctx.zfd_on == 0) {
			LSAGIU_write(LSMCU_OUT_ZFD_ON);
		}
		bl_ctx.zfd_on = 1;
	}
	else {
		// Send command on change.
		if (bl_ctx.zfd_on != 0) {
			LSAGIU_write(LSMCU_OUT_ZFD_OFF);
		}
		bl_ctx.zfd_on = 0;
	}
	// ZPR.
	SW2_update_state(&bl_ctx.zpr);
	if ((bl_ctx.zpr.state == SW2_ON) && (lsmcu_ctx.zba_closed != 0)) {
		// Send command on change.
		if (bl_ctx.zpr_on == 0) {
			LSAGIU_write(LSMCU_OUT_ZPR_ON);
		}
		bl_ctx.zpr_on = 1;
	}
	else {
		// Send command on change.
		if (bl_ctx.zpr_on != 0) {
			LSAGIU_write(LSMCU_OUT_ZPR_OFF);
		}
		bl_ctx.zpr_on = 0;
	}
}
