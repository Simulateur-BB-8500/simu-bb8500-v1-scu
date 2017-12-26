/*
 * sw3.c
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludovic
 */

#include "adc.h"
#include "gpio.h"
#include "sw3.h"
#include "tim.h"
#include "enum.h"

/*** 3-poles switch internal functions ***/

/* CHECK IF A 3-POLES SWITCH IS IN NEUTRAL POSITION.
 * @param sw3:		The switch to analyse.
 * @return result:	'true' if switch voltage indicates a neutral position, 'false' otherwise.
 */
boolean SW3_VoltageIsNeutral(SW3_Struct* sw3) {
	boolean result = false;
	if (((sw3 -> voltage) > SW3_BACK_THRESHOLD_HIGH) && ((sw3 -> voltage) < SW3_FRONT_THRESHOLD_LOW)) {
		result = true;
	}
	return result;
}

/* CHECK IF A 3-POLES SWITCH IS IN BACK POSITION.
 * @param sw3:		The switch to analyse.
 * @return result:	'true' if switch voltage indicates a back position, 'false' otherwise.
 */
boolean SW3_VoltageIsBack(SW3_Struct* sw3) {
	boolean result = false;
	if ((sw3 -> voltage) < SW3_BACK_THRESHOLD_LOW) {
		result = true;
	}
	return result;
}

/* CHECK IF A 3-POLES SWITCH IS IN FRONT POSITION.
 * @param sw3:		The switch to analyse.
 * @return result:	'true' if switch voltage indicates a front position, 'false' otherwise.
 */
boolean SW3_VoltageIsFront(SW3_Struct* sw3) {
	boolean result = false;
	if ((sw3 -> voltage) > SW3_FRONT_THRESHOLD_HIGH) {
		result = true;
	}
	return result;
}

/*** 3-poles switch functions ***/

/* INITIALISE AN SW3 STRUCTURE.
 * @param sw3:				Switch structure to initialise.
 * @param pGpio:			GPIO reading the switch.
 * @param pAdc:				ADC used for measuring the voltage.
 * @param pDebouncingMs:	Delay before validating ON/ODD state (in ms).
 * @return:					None;
 */
void SW3_Init(SW3_Struct* sw3, GPIO_Struct* pGpio, ADC_Struct* pAdc, unsigned int pDebouncingMs) {
	sw3 -> gpio = pGpio;
	sw3 -> adc = pAdc;
	sw3 -> voltage = (ADC_GetFullScale(pAdc -> adcAddress))/(2); // Neutral = Vcc/2.
	sw3 -> machineState = SW3_NEUTRAL;
	sw3 -> state = NEUTRAL;
	sw3 -> debouncingMs = pDebouncingMs;
	sw3 -> confirmStartTime = 0;
}

/* SET THE CURRENT VOLTAGE OF A 3-POLES SWITCH.
 * @param sw3:			The switch to set.
 * @param newVoltage:	New voltage measured by ADC.
 */
void SW3_SetVoltage(SW3_Struct* sw3, unsigned int newVoltage) {
	sw3 -> voltage = newVoltage;
}

/* UPDATE THE STATE OF AN SW3 STRUCTURE.
 * @param sw3:	The switch to analyse.
 * @return:		None.
 */
void SW3_UpdateState(SW3_Struct* sw3) {
	switch(sw3 -> machineState) {
	case SW3_CONFIRM_NEUTRAL:
		// Check previous state.
		switch (sw3 -> state) {
		case BACK:
			if (SW3_VoltageIsBack(sw3)) {
				// Come back to BACK without confirmation because it's the previous confirmed state.
				sw3 -> machineState = BACK;
			}
			else {
				if (SW3_VoltageIsFront(sw3)) {
					// New state to confirm.
					sw3 -> machineState = SW3_CONFIRM_FRONT;
					// Reset confirm start time.
					sw3 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if ((SW3_VoltageIsNeutral(sw3)) && (TIM_GetMs() > (sw3 -> confirmStartTime) + (sw3 -> debouncingMs))) {
						// NEUTRAL position confirmed.
						sw3 -> machineState = SW3_NEUTRAL;
					}
				}
			}
			break;
		case FRONT:
			if (SW3_VoltageIsFront(sw3)) {
				// Come back to FRONT without confirmation because it's the previous confirmed state.
				sw3 -> machineState = FRONT;
			}
			else {
				if (SW3_VoltageIsBack(sw3)) {
					// New state to confirm.
					sw3 -> machineState = SW3_CONFIRM_BACK;
					// Reset confirm start time.
					sw3 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if ((SW3_VoltageIsNeutral(sw3)) && (TIM_GetMs() > (sw3 -> confirmStartTime) + (sw3 -> debouncingMs))) {
						// NEUTRAL position confirmed.
						sw3 -> machineState = SW3_NEUTRAL;
					}
				}
			}
			break;
		default:
			// Impossible state.
			break;
		}
		break;
	case SW3_NEUTRAL:
		sw3 -> state = NEUTRAL; // Switch is in neutral position.
		if (SW3_VoltageIsBack(sw3)) {
			sw3 -> machineState = SW3_CONFIRM_BACK;
			// Reset confirm start time.
			sw3 -> confirmStartTime = TIM_GetMs();
		}
		else {
			if (SW3_VoltageIsFront(sw3)) {
				sw3 -> machineState = SW3_CONFIRM_FRONT;
				// Reset confirm start time.
				sw3 -> confirmStartTime = TIM_GetMs();
			}
		}
		break;
	case SW3_CONFIRM_BACK:
		// Check previous state.
		switch (sw3 -> state) {
		case NEUTRAL:
			if (SW3_VoltageIsNeutral(sw3)) {
				// Come back to NEUTRAL without confirmation because it's the previous confirmed state.
				sw3 -> machineState = NEUTRAL;
			}
			else {
				if (SW3_VoltageIsFront(sw3)) {
					// New state to confirm.
					sw3 -> machineState = SW3_CONFIRM_FRONT;
					// Reset confirm start time.
					sw3 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if ((SW3_VoltageIsBack(sw3)) && (TIM_GetMs() > (sw3 -> confirmStartTime) + (sw3 -> debouncingMs))) {
						// BACK position confirmed.
						sw3 -> machineState = SW3_BACK;
					}
				}
			}
			break;
		case FRONT:
			if (SW3_VoltageIsFront(sw3)) {
				// Come back to FRONT without confirmation because it's the previous confirmed state.
				sw3 -> machineState = FRONT;
			}
			else {
				if (SW3_VoltageIsNeutral(sw3)) {
					// New state to confirm.
					sw3 -> machineState = SW3_CONFIRM_NEUTRAL;
					// Reset confirm start time.
					sw3 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if ((SW3_VoltageIsBack(sw3)) && (TIM_GetMs() > (sw3 -> confirmStartTime) + (sw3 -> debouncingMs))) {
						// BACK position confirmed.
						sw3 -> machineState = SW3_BACK;
					}
				}
			}
			break;
		default:
			// Impossible state.
			break;
		}
		break;
	case SW3_BACK:
		sw3 -> state = BACK; // Switch is in back position.
		if (SW3_VoltageIsNeutral(sw3)) {
			sw3 -> machineState = SW3_CONFIRM_NEUTRAL;
			// Reset confirm start time.
			sw3 -> confirmStartTime = TIM_GetMs();
		}
		else {
			if (SW3_VoltageIsFront(sw3)) {
				sw3 -> machineState = SW3_CONFIRM_FRONT;
				// Reset confirm start time.
				sw3 -> confirmStartTime = TIM_GetMs();
			}
		}
		break;
	case SW3_CONFIRM_FRONT:
		// Check previous state.
		switch (sw3 -> state) {
		case BACK:
			if (SW3_VoltageIsBack(sw3)) {
				// Come back to BACK without confirmation because it's the previous confirmed state.
				sw3 -> machineState = BACK;
			}
			else {
				if (SW3_VoltageIsNeutral(sw3)) {
					// New state to confirm.
					sw3 -> machineState = SW3_CONFIRM_NEUTRAL;
					// Reset confirm start time.
					sw3 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if ((SW3_VoltageIsFront(sw3)) && (TIM_GetMs() > (sw3 -> confirmStartTime) + (sw3 -> debouncingMs))) {
						// FRONT position confirmed.
						sw3 -> machineState = SW3_FRONT;
					}
				}
			}
			break;
		case NEUTRAL:
			if (SW3_VoltageIsNeutral(sw3)) {
				// Come back to NEUTRAL without confirmation because it's the previous confirmed state.
				sw3 -> machineState = NEUTRAL;
			}
			else {
				if (SW3_VoltageIsBack(sw3)) {
					// New state to confirm.
					sw3 -> machineState = SW3_CONFIRM_BACK;
					// Reset confirm start time.
					sw3 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if ((SW3_VoltageIsFront(sw3)) && (TIM_GetMs() > (sw3 -> confirmStartTime) + (sw3 -> debouncingMs))) {
						// FRONT position confirmed.
						sw3 -> machineState = SW3_FRONT;
					}
				}
			}
			break;
		default:
			// Impossible state.
			break;
		}
		break;
	case SW3_FRONT:
		sw3 -> state = FRONT; // Switch is in front position.
		if (SW3_VoltageIsNeutral(sw3)) {
			sw3 -> machineState = SW3_CONFIRM_NEUTRAL;
			// Reset confirm start time.
			sw3 -> confirmStartTime = TIM_GetMs();
		}
		else {
			if (SW3_VoltageIsBack(sw3)) {
				sw3 -> machineState = SW3_CONFIRM_BACK;
				// Reset confirm start time.
				sw3 -> confirmStartTime = TIM_GetMs();
			}
		}
		break;
	default:
		// Unknown state;
		break;
	}
}
