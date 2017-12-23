/*
 * switch.c
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludovic
 */

#include "switch.h"
#include "tim.h"

/*** Switch #define ***/

// Confirm duration before validate ON/OFF state (in ms).
#define CONFIRM_DURATION 50

/*** Switch functions ***/

static void Switch_UpdateState(Switch_Struct* s) {
	switch(s -> state) {
	case OFF:
		if (GPIO_Read(s -> gpio) == (s -> activeState)) {
			s -> state = CONFIRM_ON;
			// Update confirm start time.
			s -> confirmStartTime = TIM_GetMs();
		}
		break;
	case CONFIRM_ON:
		if (GPIO_Read(s -> gpio) != (s -> activeState)) {
			s -> state = OFF;
		}
		else {
			if (TIM_GetMs() > (s -> confirmStartTime) + CONFIRM_DURATION) {
				s -> state = ON;
			}
		}
		break;
	case ON:
		if (GPIO_Read(s -> gpio) != (s -> activeState)) {
			s -> state = CONFIRM_OFF;
			// Update confirm start time.
			s -> confirmStartTime = TIM_GetMs();
		}
		break;
	case CONFIRM_OFF:
		if (GPIO_Read(s -> gpio) == (s -> activeState)) {
			s -> state = ON;
		}
		else {
			if (TIM_GetMs() > (s -> confirmStartTime) + CONFIRM_DURATION) {
				s -> state = OFF;
			}
		}
		break;
	default:
		break;
	}
}
