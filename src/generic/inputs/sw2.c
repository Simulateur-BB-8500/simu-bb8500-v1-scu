/*
 * sw2.c
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludovic
 */

#include "gpio.h"
#include "sw2.h"
#include "tim.h"
#include "types.h"

/*** 2-poles switch functions ***/

void SW2_UpdateState(SW2_Struct* sw2) {
	switch(sw2 -> currentState) {
	case SW2_OFF:
		sw2 -> state = OFF; // Switch is off.
		if (GPIO_Read(sw2 -> gpio) == (sw2 -> activeState)) {
			sw2 -> currentState = SW2_CONFIRM_ON;
			// Reset confirm start time.
			sw2 -> confirmStartTime = TIM_GetMs();
		}
		break;
	case SW2_CONFIRM_ON:
		sw2 -> state = OFF; // On state is not confirmed yet.
		if (GPIO_Read(sw2 -> gpio) != (sw2 -> activeState)) {
			sw2 -> currentState = SW2_OFF;
		}
		else {
			if (TIM_GetMs() > (sw2 -> confirmStartTime) + (sw2 -> confirmDuration)) {
				sw2 -> currentState = SW2_ON;
			}
		}
		break;
	case SW2_ON:
		sw2 -> state = ON; // Switch is on.
		if (GPIO_Read(sw2 -> gpio) != (sw2 -> activeState)) {
			sw2 -> currentState = SW2_CONFIRM_OFF;
			// Reset confirm start time.
			sw2 -> confirmStartTime = TIM_GetMs();
		}
		break;
	case SW2_CONFIRM_OFF:
		sw2 -> state = ON; // Off state is not confirmed yet.
		if (GPIO_Read(sw2 -> gpio) == (sw2 -> activeState)) {
			sw2 -> currentState = SW2_ON;
		}
		else {
			if (TIM_GetMs() > (sw2 -> confirmStartTime) + (sw2 -> confirmDuration)) {
				sw2 -> currentState = SW2_OFF;
			}
		}
		break;
	default:
		// Unknown state.
		break;
	}
}
