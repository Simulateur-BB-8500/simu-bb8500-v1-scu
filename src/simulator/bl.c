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

/*** BL global variables ***/

static SW2_Context zdv;

/*** BL functions ***/

/* INIT BL MODULE.
 * @param:	None.
 * @return:	None.
 */
void BL_Init(void) {
	GPIO_Configure(BL_ZDV_GPIO, Input, OpenDrain, LowSpeed, PullUp);
	SW2_Init(&zdv, BL_ZDV_GPIO, 0, 2000); // ZDV active low = 0.
}

/* MAIN ROUTINE OF BL MODULE.
 * @param:	None.
 * @return:	None.
 */
void BL_Routine(void) {
	SW2_UpdateState(&zdv);
	if (zdv.state == SW2_ON) {
		GPIO_Write(LED1_GPIO, 1);
	}
	else {
		GPIO_Write(LED1_GPIO, 0);
	}
}
