/*
 * sw3.c
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludovic
 */

#include "sw3.h"

#include "adc.h"
#include "gpio.h"
#include "tim.h"

/*** 3-poles switch local macros ***/

#define SW3_DELTA_HYSTERESIS_MV		100 // Set the voltage difference (in mV) between low and high thresholds.
#define SW3_BACK_THRESHOLD_LOW		((VCC_MV/4)-(SW3_DELTA_HYSTERESIS_MV/2))
#define SW3_BACK_THRESHOLD_HIGH 	((VCC_MV/4)+(SW3_DELTA_HYSTERESIS_MV/2))
#define SW3_FRONT_THRESHOLD_LOW 	(((3*VCC_MV)/4)-(SW3_DELTA_HYSTERESIS_MV/2))
#define SW3_FRONT_THRESHOLD_HIGH 	(((3*VCC_MV)/4)+(SW3_DELTA_HYSTERESIS_MV/2))

/*** 3-poles switch local functions ***/

/* CHECK IF A 3-POLES SWITCH IS IN NEUTRAL POSITION.
 * @param sw3:		The switch to analyse.
 * @return result:	'1' if switch voltage indicates a neutral position, '0' otherwise.
 */
unsigned char SW3_VoltageIsNeutral(SW3_Context* sw3) {
	unsigned char result = 0;
	if (((sw3 -> voltage) > SW3_BACK_THRESHOLD_HIGH) && ((sw3 -> voltage) < SW3_FRONT_THRESHOLD_LOW)) {
		result = 1;
	}
	return result;
}

/* CHECK IF A 3-POLES SWITCH IS IN BACK POSITION.
 * @param sw3:		The switch to analyse.
 * @return result:	'1' if switch voltage indicates a back position, '0' otherwise.
 */
unsigned char SW3_VoltageIsBack(SW3_Context* sw3) {
	unsigned char result = 0;
	if ((sw3 -> voltage) < SW3_BACK_THRESHOLD_LOW) {
		result = 1;
	}
	return result;
}

/* CHECK IF A 3-POLES SWITCH IS IN FRONT POSITION.
 * @param sw3:		The switch to analyse.
 * @return result:	'1' if switch voltage indicates a front position, '0' otherwise.
 */
unsigned char SW3_VoltageIsFront(SW3_Context* sw3) {
	unsigned char result = 0;
	if ((sw3 -> voltage) > SW3_FRONT_THRESHOLD_HIGH) {
		result = 1;
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
void SW3_Init(SW3_Context* sw3, unsigned int sw3_debouncing_ms) {
	sw3 -> voltage = VCC_MV/2; // Neutral = Vcc/2.
	sw3 -> internal_state = SW3_STATE_NEUTRAL;
	sw3 -> state = SW3_NEUTRAL;
	sw3 -> debouncing_ms = sw3_debouncing_ms;
	sw3 -> confirm_start_time = 0;
}

/* SET THE CURRENT VOLTAGE OF A 3-POLES SWITCH.
 * @param sw3:			The switch to set.
 * @param newVoltage:	New voltage measured by ADC.
 */
void SW3_SetVoltageMv(SW3_Context* sw3, unsigned int sw3_voltage_mv) {
	sw3 -> voltage = sw3_voltage_mv;
}

/* UPDATE THE STATE OF AN SW3 STRUCTURE PERFORMING HYSTERESIS AND CONFIRMATION.
 * @param sw3:	The switch to analyse.
 * @return:		None.
 */
void SW3_UpdateState(SW3_Context* sw3) {
	switch(sw3 -> internal_state) {
	case SW3_STATE_CONFIRM_NEUTRAL:
		// Check previous state.
		switch (sw3 -> state) {
		case SW3_BACK:
			if (SW3_VoltageIsBack(sw3)) {
				// Come back to BACK without confirmation because it's the previous confirmed state.
				sw3 -> internal_state = SW3_STATE_BACK;
			}
			else {
				if (SW3_VoltageIsFront(sw3)) {
					// New state to confirm.
					sw3 -> internal_state = SW3_STATE_CONFIRM_FRONT;
					// Reset confirm start time.
					sw3 -> confirm_start_time = TIM2_GetMs();
				}
				else {
					if ((SW3_VoltageIsNeutral(sw3)) && (TIM2_GetMs() > (sw3 -> confirm_start_time) + (sw3 -> debouncing_ms))) {
						// NEUTRAL position confirmed.
						sw3 -> internal_state = SW3_STATE_NEUTRAL;
					}
				}
			}
			break;
		case SW3_FRONT:
			if (SW3_VoltageIsFront(sw3)) {
				// Come back to FRONT without confirmation because it's the previous confirmed state.
				sw3 -> internal_state = SW3_STATE_FRONT;
			}
			else {
				if (SW3_VoltageIsBack(sw3)) {
					// New state to confirm.
					sw3 -> internal_state = SW3_STATE_CONFIRM_BACK;
					// Reset confirm start time.
					sw3 -> confirm_start_time = TIM2_GetMs();
				}
				else {
					if ((SW3_VoltageIsNeutral(sw3)) && (TIM2_GetMs() > (sw3 -> confirm_start_time) + (sw3 -> debouncing_ms))) {
						// NEUTRAL position confirmed.
						sw3 -> internal_state = SW3_STATE_NEUTRAL;
					}
				}
			}
			break;
		default:
			// Impossible state.
			break;
		}
		break;
	case SW3_STATE_NEUTRAL:
		sw3 -> state = SW3_NEUTRAL; // Switch is in neutral position.
		if (SW3_VoltageIsBack(sw3)) {
			sw3 -> internal_state = SW3_STATE_CONFIRM_BACK;
			// Reset confirm start time.
			sw3 -> confirm_start_time = TIM2_GetMs();
		}
		else {
			if (SW3_VoltageIsFront(sw3)) {
				sw3 -> internal_state = SW3_STATE_CONFIRM_FRONT;
				// Reset confirm start time.
				sw3 -> confirm_start_time = TIM2_GetMs();
			}
		}
		break;
	case SW3_STATE_CONFIRM_BACK:
		// Check previous state.
		switch (sw3 -> state) {
		case SW3_NEUTRAL:
			if (SW3_VoltageIsNeutral(sw3)) {
				// Come back to NEUTRAL without confirmation because it's the previous confirmed state.
				sw3 -> internal_state = SW3_STATE_NEUTRAL;
			}
			else {
				if (SW3_VoltageIsFront(sw3)) {
					// New state to confirm.
					sw3 -> internal_state = SW3_STATE_CONFIRM_FRONT;
					// Reset confirm start time.
					sw3 -> confirm_start_time = TIM2_GetMs();
				}
				else {
					if ((SW3_VoltageIsBack(sw3)) && (TIM2_GetMs() > (sw3 -> confirm_start_time) + (sw3 -> debouncing_ms))) {
						// BACK position confirmed.
						sw3 -> internal_state = SW3_STATE_BACK;
					}
				}
			}
			break;
		case SW3_FRONT:
			if (SW3_VoltageIsFront(sw3)) {
				// Come back to FRONT without confirmation because it's the previous confirmed state.
				sw3 -> internal_state = SW3_STATE_FRONT;
			}
			else {
				if (SW3_VoltageIsNeutral(sw3)) {
					// New state to confirm.
					sw3 -> internal_state = SW3_STATE_CONFIRM_NEUTRAL;
					// Reset confirm start time.
					sw3 -> confirm_start_time = TIM2_GetMs();
				}
				else {
					if ((SW3_VoltageIsBack(sw3)) && (TIM2_GetMs() > (sw3 -> confirm_start_time) + (sw3 -> debouncing_ms))) {
						// BACK position confirmed.
						sw3 -> internal_state = SW3_STATE_BACK;
					}
				}
			}
			break;
		default:
			// Impossible state.
			break;
		}
		break;
	case SW3_STATE_BACK:
		sw3 -> state = SW3_BACK; // Switch is in back position.
		if (SW3_VoltageIsNeutral(sw3)) {
			sw3 -> internal_state = SW3_STATE_CONFIRM_NEUTRAL;
			// Reset confirm start time.
			sw3 -> confirm_start_time = TIM2_GetMs();
		}
		else {
			if (SW3_VoltageIsFront(sw3)) {
				sw3 -> internal_state = SW3_STATE_CONFIRM_FRONT;
				// Reset confirm start time.
				sw3 -> confirm_start_time = TIM2_GetMs();
			}
		}
		break;
	case SW3_STATE_CONFIRM_FRONT:
		// Check previous state.
		switch (sw3 -> state) {
		case SW3_BACK:
			if (SW3_VoltageIsBack(sw3)) {
				// Come back to BACK without confirmation because it's the previous confirmed state.
				sw3 -> internal_state = SW3_STATE_BACK;
			}
			else {
				if (SW3_VoltageIsNeutral(sw3)) {
					// New state to confirm.
					sw3 -> internal_state = SW3_STATE_CONFIRM_NEUTRAL;
					// Reset confirm start time.
					sw3 -> confirm_start_time = TIM2_GetMs();
				}
				else {
					if ((SW3_VoltageIsFront(sw3)) && (TIM2_GetMs() > (sw3 -> confirm_start_time) + (sw3 -> debouncing_ms))) {
						// FRONT position confirmed.
						sw3 -> internal_state = SW3_STATE_FRONT;
					}
				}
			}
			break;
		case SW3_NEUTRAL:
			if (SW3_VoltageIsNeutral(sw3)) {
				// Come back to NEUTRAL without confirmation because it's the previous confirmed state.
				sw3 -> internal_state = SW3_STATE_NEUTRAL;
			}
			else {
				if (SW3_VoltageIsBack(sw3)) {
					// New state to confirm.
					sw3 -> internal_state = SW3_STATE_CONFIRM_BACK;
					// Reset confirm start time.
					sw3 -> confirm_start_time = TIM2_GetMs();
				}
				else {
					if ((SW3_VoltageIsFront(sw3)) && (TIM2_GetMs() > (sw3 -> confirm_start_time) + (sw3 -> debouncing_ms))) {
						// FRONT position confirmed.
						sw3 -> internal_state = SW3_STATE_FRONT;
					}
				}
			}
			break;
		default:
			// Impossible state.
			break;
		}
		break;
	case SW3_STATE_FRONT:
		sw3 -> state = SW3_FRONT; // Switch is in front position.
		if (SW3_VoltageIsNeutral(sw3)) {
			sw3 -> internal_state = SW3_STATE_CONFIRM_NEUTRAL;
			// Reset confirm start time.
			sw3 -> confirm_start_time = TIM2_GetMs();
		}
		else {
			if (SW3_VoltageIsBack(sw3)) {
				sw3 -> internal_state = SW3_STATE_CONFIRM_BACK;
				// Reset confirm start time.
				sw3 -> confirm_start_time = TIM2_GetMs();
			}
		}
		break;
	default:
		// Unknown state;
		break;
	}
}
