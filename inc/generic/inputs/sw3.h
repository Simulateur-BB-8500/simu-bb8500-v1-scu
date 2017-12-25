/*
 * sw3.h
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludovic
 */

#ifndef GENERIC_INPUTS_SW3_H_
#define GENERIC_INPUTS_SW3_H_

#include "enum.h"

/*** 3-poles switch structure ***/

typedef struct {
	GPIO_Struct* gpio;
	GPIO_State activeState; // Depends on switch wiring.
	SW3_StateMachine currentState; // Current state in SW2 state machine.
	SW3_State state; // State after anti-bouncing (used in higher levels).
	unsigned int debouncingMs; // Delay before validating ON/ODD state (in ms).
	unsigned int confirmStartTime;
} SW3_Struct;

/*** 3-poles switch functions ***/

void SW3_Init(SW3_Struct* sw3, GPIO_Struct* pGpio, GPIO_State pActiveState, unsigned int pDebouncingMs);
void SW3_UpdateState(SW3_Struct* sw3);

#endif /* GENERIC_INPUTS_SW3_H_ */
