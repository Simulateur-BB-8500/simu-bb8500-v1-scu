/*
 * sw2.c
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludo
 */

#include "sw2.h"

#include "gpio.h"
#include "tim.h"
#include "stdint.h"

/*** SW2 functions ***/

/* INITIALISE AN SW2 STRUCTURE.
 * @param sw2:				Switch structure to initialize.
 * @param gpio:				GPIO attached to the switch.
 * @param active_state:		GPIO state ('0' or '1') for which the switch is considered on.
 * @param debouncing_ms:	Delay before validating ON/OFF state (in ms).
 * @return:					None.
 */
void SW2_init(SW2_context_t* sw2, const GPIO* gpio, uint8_t active_state, uint32_t debouncing_ms) {
	// Init GPIO.
	GPIO_configure(gpio, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_UP);
	// Init context.
	(sw2 -> gpio) = gpio;
	(sw2 -> active_state) = active_state;
	(sw2 -> internal_state) = SW2_STATE_OFF;
	(sw2 -> state) = SW2_OFF;
	(sw2 -> debouncing_ms) = debouncing_ms;
	(sw2 -> confirm_start_time) = 0;
}

/* UPDATE THE STATE OF AN SW2 STRUCTURE.
 * @param sw2:	The switch to analyse.
 * @return:		None.
 */
void SW2_update_state(SW2_context_t* sw2) {
	switch((sw2 -> internal_state)) {
	case SW2_STATE_OFF:
		((sw2 -> state)) = SW2_OFF; // Switch is off.
		if (GPIO_read(sw2 -> gpio) == (sw2 -> active_state)) {
			((sw2 -> internal_state)) = SW2_STATE_CONFIRM_ON;
			// Reset confirm start time.
			sw2 -> confirm_start_time = TIM2_get_milliseconds();
		}
		break;
	case SW2_STATE_CONFIRM_ON:
		if (GPIO_read(sw2 -> gpio) != (sw2 -> active_state)) {
			((sw2 -> internal_state)) = SW2_STATE_OFF;
		}
		else {
			if (TIM2_get_milliseconds() > ((sw2 -> confirm_start_time) + (sw2 -> debouncing_ms))) {
				(sw2 -> internal_state) = SW2_STATE_ON;
			}
		}
		break;
	case SW2_STATE_ON:
		(sw2 -> state) = SW2_ON; // Switch is on.
		if (GPIO_read(sw2 -> gpio) != (sw2 -> active_state)) {
			(sw2 -> internal_state) = SW2_STATE_CONFIRM_OFF;
			// Reset confirm start time.
			sw2 -> confirm_start_time = TIM2_get_milliseconds();
		}
		break;
	case SW2_STATE_CONFIRM_OFF:
		if (GPIO_read(sw2 -> gpio) == (sw2 -> active_state)) {
			(sw2 -> internal_state) = SW2_STATE_ON;
		}
		else {
			if (TIM2_get_milliseconds() > ((sw2 -> confirm_start_time) + (sw2 -> debouncing_ms))) {
				(sw2 -> internal_state) = SW2_STATE_OFF;
			}
		}
		break;
	default:
		// Unknown state.
		break;
	}
}
