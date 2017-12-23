/*
 * switch.h
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludovic
 */

#ifndef GENERIC_INPUTS_SWITCH_H_
#define GENERIC_INPUTS_SWITCH_H_

#include "gpio.h"
#include "types.h"

/*** Switch state machine ***/

typedef enum {
	OFF,
	CONFIRM_ON,
	ON,
	CONFIRM_OFF
} SwitchState;

/*** Switch structure ***/

typedef struct {
	GPIO_Struct* gpio;
	GPIOState activeState;
	SwitchState state;
	unsigned int confirmStartTime;
} Switch_Struct;

/*** Switch functions ***/

static void Switch_UpdateState(Switch_Struct* s);

#endif /* GENERIC_INPUTS_SWITCH_H_ */
