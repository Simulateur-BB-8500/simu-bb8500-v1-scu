/*
 * sw3.c
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludo
 */

#include "sw3.h"

#include "adc.h"
#include "gpio.h"
#include "lsmcu.h"
#include "tim.h"
#include "stdint.h"

/*** SW3 local macros ***/

#define SW3_DEFAULT_VOLTAGE_MV		(ADC_VCC_DEFAULT_MV / 2); // Neutral = Vcc/2.
#define SW3_DELTA_HYSTERESIS_MV		100 // Set the voltage difference (in mV) between low and high thresholds.
#define SW3_BACK_THRESHOLD_LOW		((ADC_VCC_DEFAULT_MV / 4) - (SW3_DELTA_HYSTERESIS_MV / 2))
#define SW3_BACK_THRESHOLD_HIGH 	((ADC_VCC_DEFAULT_MV / 4) + (SW3_DELTA_HYSTERESIS_MV / 2))
#define SW3_FRONT_THRESHOLD_LOW 	(((3 * ADC_VCC_DEFAULT_MV) / 4) - (SW3_DELTA_HYSTERESIS_MV / 2))
#define SW3_FRONT_THRESHOLD_HIGH 	(((3 * ADC_VCC_DEFAULT_MV) / 4) + (SW3_DELTA_HYSTERESIS_MV / 2))

/*** SW3 external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** SW3 local functions ***/

/* CHECK IF A SW3 IS IN NEUTRAL POSITION.
 * @param sw3:		The switch to analyze.
 * @return result:	'1' if switch voltage indicates a neutral position, '0' otherwise.
 */
static uint8_t _SW3_voltage_is_neutral(SW3_context_t* sw3) {
	uint8_t result = 0;
	if (((sw3 -> voltage_mv) > SW3_BACK_THRESHOLD_HIGH) && ((sw3 -> voltage_mv) < SW3_FRONT_THRESHOLD_LOW)) {
		result = 1;
	}
	return result;
}

/* CHECK IF A SW3 IS IN BACK POSITION.
 * @param sw3:		The switch to analyze.
 * @return result:	'1' if switch voltage indicates a back position, '0' otherwise.
 */
static uint8_t _SW3_voltage_is_back(SW3_context_t* sw3) {
	uint8_t result = 0;
	if ((sw3 -> voltage_mv) < SW3_BACK_THRESHOLD_LOW) {
		result = 1;
	}
	return result;
}

/* CHECK IF A SW3 IS IN FRONT POSITION.
 * @param sw3:		The switch to analyze.
 * @return result:	'1' if switch voltage indicates a front position, '0' otherwise.
 */
static uint8_t _SW3_voltage_is_front(SW3_context_t* sw3) {
	uint8_t result = 0;
	if ((sw3 -> voltage_mv) > SW3_FRONT_THRESHOLD_HIGH) {
		result = 1;
	}
	return result;
}

/*** SW3 functions ***/

/* INITIALISE AN SW3 STRUCTURE.
 * @param sw3:				Switch structure to initialize.
 * @param gpio:				GPIO attached to the switch.
 * @param debouncing_ms:	Delay before validating ON/OFF state (in ms).
 * @param adc_data_ptr:		Pointer to the 12-bits ADC data.
 * @return:					None.
 */
void SW3_init(SW3_context_t* sw3, const GPIO* gpio, uint32_t debouncing_ms, uint32_t* adc_data_ptr) {
	// Init GPIO.
	GPIO_configure(gpio, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init context.
	(sw3 -> adc_data_ptr) = adc_data_ptr;
	(sw3 -> voltage_mv) = SW3_DEFAULT_VOLTAGE_MV;
	(sw3 -> internal_state) = SW3_STATE_NEUTRAL;
	(sw3 -> state) = SW3_NEUTRAL;
	(sw3 -> debouncing_ms) = debouncing_ms;
	(sw3 -> confirm_start_time) = 0;
}

/* UPDATE THE STATE OF AN SW3 STRUCTURE PERFORMING HYSTERESIS AND CONFIRMATION.
 * @param sw3:	The switch to analyze.
 * @return:		None.
 */
void SW3_update_state(SW3_context_t* sw3) {
	// Update voltage (only if ZBA is closed).
	(sw3 -> voltage_mv) = (lsmcu_ctx.zba_closed != 0) ? ADC1_convert_to_mv(*(sw3 -> adc_data_ptr)) : SW3_DEFAULT_VOLTAGE_MV;
	// Perform debouncing state machine.
	switch((sw3 -> internal_state)) {
	case SW3_STATE_CONFIRM_NEUTRAL:
		// Check previous state.
		switch ((sw3 -> state)) {
		case SW3_BACK:
			if (_SW3_voltage_is_back(sw3)) {
				// Come back to BACK without confirmation because it's the previous confirmed state.
				(sw3 -> internal_state) = SW3_STATE_BACK;
			}
			else {
				if (_SW3_voltage_is_front(sw3)) {
					// New state to confirm.
					(sw3 -> internal_state) = SW3_STATE_CONFIRM_FRONT;
					// Reset confirm start time.
					(sw3 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if ((_SW3_voltage_is_neutral(sw3)) && (TIM2_get_milliseconds() > ((sw3 -> confirm_start_time)) + (sw3 -> debouncing_ms))) {
						// NEUTRAL position confirmed.
						(sw3 -> internal_state) = SW3_STATE_NEUTRAL;
					}
				}
			}
			break;
		case SW3_FRONT:
			if (_SW3_voltage_is_front(sw3)) {
				// Come back to FRONT without confirmation because it's the previous confirmed state.
				(sw3 -> internal_state) = SW3_STATE_FRONT;
			}
			else {
				if (_SW3_voltage_is_back(sw3)) {
					// New state to confirm.
					(sw3 -> internal_state) = SW3_STATE_CONFIRM_BACK;
					// Reset confirm start time.
					(sw3 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if ((_SW3_voltage_is_neutral(sw3)) && (TIM2_get_milliseconds() > ((sw3 -> confirm_start_time)) + (sw3 -> debouncing_ms))) {
						// NEUTRAL position confirmed.
						(sw3 -> internal_state) = SW3_STATE_NEUTRAL;
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
		(sw3 -> state) = SW3_NEUTRAL; // Switch is in neutral position.
		if (_SW3_voltage_is_back(sw3)) {
			(sw3 -> internal_state) = SW3_STATE_CONFIRM_BACK;
			// Reset confirm start time.
			(sw3 -> confirm_start_time) = TIM2_get_milliseconds();
		}
		else {
			if (_SW3_voltage_is_front(sw3)) {
				(sw3 -> internal_state) = SW3_STATE_CONFIRM_FRONT;
				// Reset confirm start time.
				(sw3 -> confirm_start_time) = TIM2_get_milliseconds();
			}
		}
		break;
	case SW3_STATE_CONFIRM_BACK:
		// Check previous state.
		switch ((sw3 -> state)) {
		case SW3_NEUTRAL:
			if (_SW3_voltage_is_neutral(sw3)) {
				// Come back to NEUTRAL without confirmation because it's the previous confirmed state.
				(sw3 -> internal_state) = SW3_STATE_NEUTRAL;
			}
			else {
				if (_SW3_voltage_is_front(sw3)) {
					// New state to confirm.
					(sw3 -> internal_state) = SW3_STATE_CONFIRM_FRONT;
					// Reset confirm start time.
					(sw3 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if ((_SW3_voltage_is_back(sw3)) && (TIM2_get_milliseconds() > ((sw3 -> confirm_start_time)) + (sw3 -> debouncing_ms))) {
						// BACK position confirmed.
						(sw3 -> internal_state) = SW3_STATE_BACK;
					}
				}
			}
			break;
		case SW3_FRONT:
			if (_SW3_voltage_is_front(sw3)) {
				// Come back to FRONT without confirmation because it's the previous confirmed state.
				(sw3 -> internal_state) = SW3_STATE_FRONT;
			}
			else {
				if (_SW3_voltage_is_neutral(sw3)) {
					// New state to confirm.
					(sw3 -> internal_state) = SW3_STATE_CONFIRM_NEUTRAL;
					// Reset confirm start time.
					(sw3 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if ((_SW3_voltage_is_back(sw3)) && (TIM2_get_milliseconds() > ((sw3 -> confirm_start_time)) + (sw3 -> debouncing_ms))) {
						// BACK position confirmed.
						(sw3 -> internal_state) = SW3_STATE_BACK;
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
		(sw3 -> state) = SW3_BACK; // Switch is in back position.
		if (_SW3_voltage_is_neutral(sw3)) {
			(sw3 -> internal_state) = SW3_STATE_CONFIRM_NEUTRAL;
			// Reset confirm start time.
			(sw3 -> confirm_start_time) = TIM2_get_milliseconds();
		}
		else {
			if (_SW3_voltage_is_front(sw3)) {
				(sw3 -> internal_state) = SW3_STATE_CONFIRM_FRONT;
				// Reset confirm start time.
				(sw3 -> confirm_start_time) = TIM2_get_milliseconds();
			}
		}
		break;
	case SW3_STATE_CONFIRM_FRONT:
		// Check previous state.
		switch ((sw3 -> state)) {
		case SW3_BACK:
			if (_SW3_voltage_is_back(sw3)) {
				// Come back to BACK without confirmation because it's the previous confirmed state.
				(sw3 -> internal_state) = SW3_STATE_BACK;
			}
			else {
				if (_SW3_voltage_is_neutral(sw3)) {
					// New state to confirm.
					(sw3 -> internal_state) = SW3_STATE_CONFIRM_NEUTRAL;
					// Reset confirm start time.
					(sw3 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if ((_SW3_voltage_is_front(sw3)) && (TIM2_get_milliseconds() > ((sw3 -> confirm_start_time)) + (sw3 -> debouncing_ms))) {
						// FRONT position confirmed.
						(sw3 -> internal_state) = SW3_STATE_FRONT;
					}
				}
			}
			break;
		case SW3_NEUTRAL:
			if (_SW3_voltage_is_neutral(sw3)) {
				// Come back to NEUTRAL without confirmation because it's the previous confirmed state.
				(sw3 -> internal_state) = SW3_STATE_NEUTRAL;
			}
			else {
				if (_SW3_voltage_is_back(sw3)) {
					// New state to confirm.
					(sw3 -> internal_state) = SW3_STATE_CONFIRM_BACK;
					// Reset confirm start time.
					(sw3 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if ((_SW3_voltage_is_front(sw3)) && (TIM2_get_milliseconds() > ((sw3 -> confirm_start_time)) + (sw3 -> debouncing_ms))) {
						// FRONT position confirmed.
						(sw3 -> internal_state) = SW3_STATE_FRONT;
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
		(sw3 -> state) = SW3_FRONT; // Switch is in front position.
		if (_SW3_voltage_is_neutral(sw3)) {
			(sw3 -> internal_state) = SW3_STATE_CONFIRM_NEUTRAL;
			// Reset confirm start time.
			(sw3 -> confirm_start_time) = TIM2_get_milliseconds();
		}
		else {
			if (_SW3_voltage_is_back(sw3)) {
				(sw3 -> internal_state) = SW3_STATE_CONFIRM_BACK;
				// Reset confirm start time.
				(sw3 -> confirm_start_time) = TIM2_get_milliseconds();
			}
		}
		break;
	default:
		// Unknown state;
		break;
	}
}
