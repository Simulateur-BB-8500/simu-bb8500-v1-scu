/*
 * sw2.h
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludovic
 */

#ifndef GENERIC_INPUTS_SW2_H_
#define GENERIC_INPUTS_SW2_H_

#include "gpio.h"
#include "types.h"

/*** Switch state machine ***/

typedef enum {
	SW2_OFF,
	SW2_CONFIRM_ON,
	SW2_ON,
	SW2_CONFIRM_OFF
} SW2_StateMachine;

/*** 2-poles switch structure ***/

typedef struct {
	GPIO_Struct* gpio;
	GPIO_State activeState; // Depends on switch wiring.
	SW2_StateMachine currentState; // Current state in SW2 state machine.
	SW2_State state; // State after anti-bouncing (used in higher levels).
	unsigned int confirmDuration; // Delay before validating ON/ODD state (in ms).
	unsigned int confirmStartTime;
} SW2_Struct;

/*** 2-poles switch functions ***/

void SW2_UpdateState(SW2_Struct* s);

#endif /* GENERIC_INPUTS_SW2_H_ */
