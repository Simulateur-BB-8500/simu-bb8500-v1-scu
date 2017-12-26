/*
 * sw2.c
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludovic
 */

#include "gpio.h"
#include "sw2.h"
#include "tim.h"
#include "enum.h"

/*** 2-poles switch functions ***/

/* INITIALISE AN SW2 STRUCTURE.
 * @param sw2:				Switch structure to initialise.
 * @param pGpio:			GPIO reading the switch.
 * @param pActiveState:		GPIO state ('LOW' or 'HIGH') for which the switch is considered on.
 * @param pDebouncingMs:	Delay before validating ON/ODD state (in ms).
 * @return:					None;
 */
void SW2_Init(SW2_Struct* sw2, GPIO_Struct* pGpio, GPIO_State pActiveState, unsigned int pDebouncingMs) {
	sw2 -> gpio = pGpio;
	sw2 -> activeState = pActiveState;
	sw2 -> machineState = SW2_OFF;
	sw2 -> state = OFF;
	sw2 -> debouncingMs = pDebouncingMs;
	sw2 -> confirmStartTime = 0;
}

/* UPDATE THE STATE OF AN SW2 STRUCTURE.
 * @param sw2:	The switch to analyse.
 * @return:		None.
 */
void SW2_UpdateState(SW2_Struct* sw2) {
	switch(sw2 -> machineState) {
	case SW2_OFF:
		sw2 -> state = OFF; // Switch is off.
		if (GPIO_Read(sw2 -> gpio) == (sw2 -> activeState)) {
			sw2 -> machineState = SW2_CONFIRM_ON;
			// Reset confirm start time.
			sw2 -> confirmStartTime = TIM_GetMs();
		}
		break;
	case SW2_CONFIRM_ON:
		if (GPIO_Read(sw2 -> gpio) != (sw2 -> activeState)) {
			sw2 -> machineState = SW2_OFF;
		}
		else {
			if (TIM_GetMs() > (sw2 -> confirmStartTime) + (sw2 -> debouncingMs)) {
				sw2 -> machineState = SW2_ON;
			}
		}
		break;
	case SW2_ON:
		sw2 -> state = ON; // Switch is on.
		if (GPIO_Read(sw2 -> gpio) != (sw2 -> activeState)) {
			sw2 -> machineState = SW2_CONFIRM_OFF;
			// Reset confirm start time.
			sw2 -> confirmStartTime = TIM_GetMs();
		}
		break;
	case SW2_CONFIRM_OFF:
		if (GPIO_Read(sw2 -> gpio) == (sw2 -> activeState)) {
			sw2 -> machineState = SW2_ON;
		}
		else {
			if (TIM_GetMs() > (sw2 -> confirmStartTime) + (sw2 -> debouncingMs)) {
				sw2 -> machineState = SW2_OFF;
			}
		}
		break;
	default:
		// Unknown state.
		break;
	}
}
