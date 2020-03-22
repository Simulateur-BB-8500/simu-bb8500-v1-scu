/*
 * zpt.c
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludo
 */

#include "zpt.h"

#include "common.h"
#include "gpio.h"
#include "lssgkcu.h"
#include "mapping.h"
#include "sw4.h"

/*** ZPT local structures ***/

typedef struct {
	SW4_Context zpt_sw4;
	SW4_State zpt_previous_state;
	unsigned char zpt_enable;
} ZPT_Context;

/*** ZPT local global variables ***/

static ZPT_Context zpt_ctx;

/*** ZPT functions ***/

/* INIT ZPT MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZPT_Init(void) {

	/* Init context */
	SW4_Init(&zpt_ctx.zpt_sw4, &GPIO_ZPT, 500);
	zpt_ctx.zpt_previous_state = SW4_P0;
	zpt_ctx.zpt_enable = 0;
	GPIO_Configure(&GPIO_VLG, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Write(&GPIO_VLG, 1);
}

/* UPDATE THE VOLTAGE READ ON ZPT SELECTOR (CALLED BY ADC ROUTINE).
 * @param new_voltage:	New voltage measured.
 * @return:				None.
 */
void ZPT_SetVoltageMv(unsigned int zpt_voltage_mv) {
	SW4_SetVoltageMv(&zpt_ctx.zpt_sw4, zpt_voltage_mv);
}

/* MAIN ROUTINE OF ZPT MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZPT_Task(LSMCU_Context* lsmcu_ctx) {
	// Check BL.
	if (lsmcu_ctx -> lsmcu_bl_unlocked != 0) {
		zpt_ctx.zpt_enable = 1;
		// Update current state.
		SW4_UpdateState(&zpt_ctx.zpt_sw4);
		// Perform actions according to state.
		switch (zpt_ctx.zpt_sw4.sw4_state) {
		case SW4_P0:
			if (zpt_ctx.zpt_previous_state != SW4_P0) {
				LSSGKCU_Send(LSSGKCU_IN_ZPT_BACK_DOWN);
				LSSGKCU_Send(LSSGKCU_IN_ZPT_FRONT_DOWN);
				GPIO_Write(&GPIO_VLG, 1);
			}
			(lsmcu_ctx -> lsmcu_zpt_raised) = 0;
			break;
		case SW4_P1:
			if (zpt_ctx.zpt_previous_state != SW4_P1) {
				LSSGKCU_Send(LSSGKCU_IN_ZPT_BACK_UP);
				LSSGKCU_Send(LSSGKCU_IN_ZPT_FRONT_DOWN);
				GPIO_Write(&GPIO_VLG, 0);
			}
			(lsmcu_ctx -> lsmcu_zpt_raised) = 1;
			break;
		case SW4_P2:
			if (zpt_ctx.zpt_previous_state != SW4_P2) {
				LSSGKCU_Send(LSSGKCU_IN_ZPT_BACK_UP);
				LSSGKCU_Send(LSSGKCU_IN_ZPT_FRONT_UP);
				GPIO_Write(&GPIO_VLG, 0);
			}
			(lsmcu_ctx -> lsmcu_zpt_raised) = 1;
			break;
		case SW4_P3:
			if (zpt_ctx.zpt_previous_state != SW4_P3) {
				LSSGKCU_Send(LSSGKCU_IN_ZPT_BACK_DOWN);
				LSSGKCU_Send(LSSGKCU_IN_ZPT_FRONT_UP);
				GPIO_Write(&GPIO_VLG, 0);
			}
			(lsmcu_ctx -> lsmcu_zpt_raised) = 1;
			break;
		default:
			// Unknown state.
			break;
		}
		// Update previous state.
		zpt_ctx.zpt_previous_state = zpt_ctx.zpt_sw4.sw4_state;
	}
	else {
		// Disable ZPT.
		if (zpt_ctx.zpt_enable != 0) {
			LSSGKCU_Send(LSSGKCU_IN_ZPT_BACK_DOWN);
			LSSGKCU_Send(LSSGKCU_IN_ZPT_FRONT_DOWN);
			GPIO_Write(&GPIO_VLG, 1);
			zpt_ctx.zpt_previous_state = SW4_P0;
			(lsmcu_ctx -> lsmcu_zpt_raised) = 0;
		}
		zpt_ctx.zpt_enable = 0;
	}
}
