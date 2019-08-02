/*
 * bl.c
 *
 *  Created on: 8 avr. 2018
 *      Author: Ludovic
 */

#include "bl.h"

#include "gpio.h"
#include "mapping.h"
#include "sw2.h"

/*** BL local structures ***/

typedef struct {
	SW2_Context zdv;
	unsigned char bl_unlocked;
} BL_Context;

/*** BL local global variables ***/

static BL_Context bl_ctx;

/*** BL functions ***/

/* INIT BL MODULE.
 * @param:	None.
 * @return:	None.
 */
void BL_Init(void) {

	/* Init GPIOs */
	GPIO_Configure(&GPIO_BL_ZDV, GPIO_MODE_INPUT, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_UP);

	/* Init context */
	SW2_Init(&bl_ctx.zdv, &GPIO_BL_ZDV, 0, 2000); // ZDV active low = 0.
	bl_ctx.bl_unlocked = 0;
}

/* MAIN ROUTINE OF BL MODULE.
 * @param:	None.
 * @return:	None.
 */
void BL_Task(unsigned char* bl_unlocked) {

	// Unlock switch.
	SW2_UpdateState(&bl_ctx.zdv);
	if (bl_ctx.zdv.state == SW2_ON) {
		bl_ctx.bl_unlocked = 1;
	}
	else {
		bl_ctx.bl_unlocked = 0;
	}
	(*bl_unlocked) = bl_ctx.bl_unlocked;
}
