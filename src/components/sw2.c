/*
 * sw2.c
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludovic
 */

#include "sw2.h"

#include "gpio.h"
#include "tim.h"

/*** 2-poles switch functions ***/

/* INITIALISE AN SW2 STRUCTURE.
 * @param sw2:				Switch structure to initialise.
 * @param pGpio:			GPIO reading the switch.
 * @param pActiveState:		GPIO state ('LOW' or 'HIGH') for which the switch is considered on.
 * @param pDebouncingMs:	Delay before validating ON/ODD state (in ms).
 * @return:					None.
 */
void SW2_Init(SW2_Context* sw2, const GPIO* sw2_gpio, unsigned char sw2_active_state, unsigned int sw2_debouncing_ms) {
	sw2 -> gpio = sw2_gpio;
	sw2 -> active_state = sw2_active_state;
	sw2 -> internal_state = SW2_STATE_OFF;
	sw2 -> state = SW2_OFF;
	sw2 -> debouncing_ms = sw2_debouncing_ms;
	sw2 -> confirm_start_time = 0;
}

/* UPDATE THE STATE OF AN SW2 STRUCTURE.
 * @param sw2:	The switch to analyse.
 * @return:		None.
 */
void SW2_UpdateState(SW2_Context* sw2) {
	switch(sw2 -> internal_state) {
	case SW2_STATE_OFF:
		sw2 -> state = SW2_OFF; // Switch is off.
		if (GPIO_Read(sw2 -> gpio) == (sw2 -> active_state)) {
			sw2 -> internal_state = SW2_STATE_CONFIRM_ON;
			// Reset confirm start time.
			sw2 -> confirm_start_time = TIM2_GetMs();
		}
		break;
	case SW2_STATE_CONFIRM_ON:
		if (GPIO_Read(sw2 -> gpio) != (sw2 -> active_state)) {
			sw2 -> internal_state = SW2_STATE_OFF;
		}
		else {
			if (TIM2_GetMs() > (sw2 -> confirm_start_time) + (sw2 -> debouncing_ms)) {
				sw2 -> internal_state = SW2_STATE_ON;
			}
		}
		break;
	case SW2_STATE_ON:
		sw2 -> state = SW2_ON; // Switch is on.
		if (GPIO_Read(sw2 -> gpio) != (sw2 -> active_state)) {
			sw2 -> internal_state = SW2_STATE_CONFIRM_OFF;
			// Reset confirm start time.
			sw2 -> confirm_start_time = TIM2_GetMs();
		}
		break;
	case SW2_STATE_CONFIRM_OFF:
		if (GPIO_Read(sw2 -> gpio) == (sw2 -> active_state)) {
			sw2 -> internal_state = SW2_STATE_ON;
		}
		else {
			if (TIM2_GetMs() > (sw2 -> confirm_start_time) + (sw2 -> debouncing_ms)) {
				sw2 -> internal_state = SW2_STATE_OFF;
			}
		}
		break;
	default:
		// Unknown state.
		break;
	}
}
