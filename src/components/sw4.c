/*
 * sw4.c
 *
 *  Created on: 26 dec. 2017
 *      Author: Ludo
 */

#include "sw4.h"

#include "adc.h"
#include "gpio.h"
#include "lsmcu.h"
#include "tim.h"
#include "stdint.h"

/*** SW4 local macros ***/

#define SW4_DEFAULT_VOLTAGE_MV		0 // P0 = 0V.
#define SW4_DELTA_HYSTERESIS_MV		100 // Set the voltage difference (in mV) between low and high thresholds.
#define SW4_P0P1_THRESHOLD_LOW 		((ADC_VCC_DEFAULT_MV / 6) - (SW4_DELTA_HYSTERESIS_MV / 2))
#define SW4_P0P1_THRESHOLD_HIGH 	((ADC_VCC_DEFAULT_MV / 6) + (SW4_DELTA_HYSTERESIS_MV / 2))
#define SW4_P1P2_THRESHOLD_LOW 		(((3 * ADC_VCC_DEFAULT_MV) / 6) - (SW4_DELTA_HYSTERESIS_MV / 2))
#define SW4_P1P2_THRESHOLD_HIGH 	(((3 * ADC_VCC_DEFAULT_MV) / 6) + (SW4_DELTA_HYSTERESIS_MV / 2))
#define SW4_P2P3_THRESHOLD_LOW 		(((5 * ADC_VCC_DEFAULT_MV) / 6) - (SW4_DELTA_HYSTERESIS_MV / 2))
#define SW4_P2P3_THRESHOLD_HIGH 	(((5 * ADC_VCC_DEFAULT_MV) / 6) + (SW4_DELTA_HYSTERESIS_MV / 2))

/*** SW4 external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** SW4 local functions ***/

/* CHECK IF A 4-POLES SWITCH IS IN P0 POSITION.
 * @param sw4:		The switch to analyze.
 * @return result:	'1' if switch voltage indicates a P0 position, '0' otherwise.
 */
static uint8_t _SW4_voltage_is_p0(SW4_context_t* sw4) {
	uint8_t result = 0;
	if ((sw4 -> voltage_mv) < SW4_P0P1_THRESHOLD_LOW) {
		result = 1;
	}
	return result;
}

/* CHECK IF A 4-POLES SWITCH IS IN P1 POSITION.
 * @param sw4:		The switch to analyze.
 * @return result:	'1' if switch voltage indicates a P1 position, '0' otherwise.
 */
static uint8_t _SW4_voltage_is_p1(SW4_context_t* sw4) {
	uint8_t result = 0;
	if (((sw4 -> voltage_mv) > SW4_P0P1_THRESHOLD_HIGH) && ((sw4 -> voltage_mv) < SW4_P1P2_THRESHOLD_LOW)) {
		result = 1;
	}
	return result;
}

/* CHECK IF A 4-POLES SWITCH IS IN P2 POSITION.
 * @param sw4:		The switch to analyze.
 * @return result:	'1' if switch voltage indicates a P2 position, '0' otherwise.
 */
static uint8_t _SW4_voltage_is_p2(SW4_context_t* sw4) {
	uint8_t result = 0;
	if (((sw4 -> voltage_mv) > SW4_P1P2_THRESHOLD_HIGH) && ((sw4 -> voltage_mv) < SW4_P2P3_THRESHOLD_LOW)) {
		result = 1;
	}
	return result;
}

/* CHECK IF A 4-POLES SWITCH IS IN P3 POSITION.
 * @param sw4:		The switch to analyze.
 * @return result:	'1' if switch voltage indicates a P3 position, '0' otherwise.
 */
static uint8_t _SW4_voltage_is_p3(SW4_context_t* sw4) {
	uint8_t result = 0;
	if ((sw4 -> voltage_mv) > SW4_P2P3_THRESHOLD_HIGH) {
		result = 1;
	}
	return result;
}

/*** SW4 functions ***/

/* INITIALISE AN SW4 STRUCTURE.
 * @param sw4:				Switch structure to initialize.
 * @param gpio:				GPIO attached to the switch.
 * @param debouncing_ms:	Delay before validating ON/OFF state (in ms).
 * @param adc_data_ptr:		Pointer to the 12-bits ADC data.
 * @return:					None;
 */
void SW4_init(SW4_context_t* sw4, const GPIO* gpio, uint32_t debouncing_ms, uint32_t* adc_data_ptr) {
	// Init GPIO.
	GPIO_configure(gpio, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init context.
	(sw4 -> adc_data_ptr) = adc_data_ptr;
	(sw4 -> voltage_mv) = SW4_DEFAULT_VOLTAGE_MV;
	(sw4 -> internal_state) = SW4_STATE_P0;
	(sw4 -> state) = SW4_P0;
	(sw4 -> debouncing_ms) = debouncing_ms;
	(sw4 -> confirm_start_time) = 0;
}

/* UPDATE THE STATE OF AN SW4 STRUCTURE PERFORMING HYSTERESIS AND CONFIRMATION.
 * @param sw4:	The switch to analyze.
 * @return:		None.
 */
void SW4_update_state(SW4_context_t* sw4) {
	// Update voltage (only if ZBA is closed).
	(sw4 -> voltage_mv) = (lsmcu_ctx.zba_closed != 0) ? ADC1_convert_to_mv(*(sw4 -> adc_data_ptr)) : SW4_DEFAULT_VOLTAGE_MV;
	// Perform debouncing state machine.
	switch((sw4 -> internal_state)) {
	case SW4_STATE_CONFIRM_P0:
		// Check previous state.
		switch ((sw4 -> state)) {
		case SW4_P1:
			if (_SW4_voltage_is_p1(sw4)) {
				// Come back to P1 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P1;
			}
			else {
				if (_SW4_voltage_is_p2(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if (_SW4_voltage_is_p3(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
					}
					else {
						if ((_SW4_voltage_is_p0(sw4)) && (TIM2_get_milliseconds() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P0 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P0;
						}
					}
				}
			}
			break;
		case SW4_P2:
			if (_SW4_voltage_is_p2(sw4)) {
				// Come back to P2 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P2;
			}
			else {
				if (_SW4_voltage_is_p1(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if (_SW4_voltage_is_p3(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
					}
					else {
						if ((_SW4_voltage_is_p0(sw4)) && (TIM2_get_milliseconds() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P0 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P0;
						}
					}
				}
			}
			break;
		case SW4_P3:
			if (_SW4_voltage_is_p3(sw4)) {
				// Come back to P3 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P3;
			}
			else {
				if (_SW4_voltage_is_p1(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if (_SW4_voltage_is_p2(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
					}
					else {
						if ((_SW4_voltage_is_p0(sw4)) && (TIM2_get_milliseconds() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P0 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P0;
						}
					}
				}
			}
			break;
		default:
			// Impossible state.
			break;
		}
		break;
	case SW4_STATE_P0:
		(sw4 -> state) = SW4_P0; // Switch is in P0 position.
		if (_SW4_voltage_is_p1(sw4)) {
			(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
			// Reset confirm start time.
			(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
		}
		else {
			if (_SW4_voltage_is_p2(sw4)) {
				(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
				// Reset confirm start time.
				(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
			}
			else {
				if (_SW4_voltage_is_p3(sw4)) {
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
				}
			}
		}
		break;
	case SW4_STATE_CONFIRM_P1:
		// Check previous state.
		switch ((sw4 -> state)) {
		case SW4_P0:
			if (_SW4_voltage_is_p0(sw4)) {
				// Come back to P0 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P0;
			}
			else {
				if (_SW4_voltage_is_p2(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if (_SW4_voltage_is_p3(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
					}
					else {
						if ((_SW4_voltage_is_p1(sw4)) && (TIM2_get_milliseconds() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P1 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P1;
						}
					}
				}
			}
			break;
		case SW4_P2:
			if (_SW4_voltage_is_p2(sw4)) {
				// Come back to P2 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P2;
			}
			else {
				if (_SW4_voltage_is_p0(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if (_SW4_voltage_is_p3(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
					}
					else {
						if ((_SW4_voltage_is_p1(sw4)) && (TIM2_get_milliseconds() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P1 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P1;
						}
					}
				}
			}
			break;
		case SW4_P3:
			if (_SW4_voltage_is_p3(sw4)) {
				// Come back to P3 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P3;
			}
			else {
				if (_SW4_voltage_is_p0(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if (_SW4_voltage_is_p2(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
					}
					else {
						if ((_SW4_voltage_is_p1(sw4)) && (TIM2_get_milliseconds() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P1 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P1;
						}
					}
				}
			}
			break;
		default:
			// Impossible state.
			break;
		}
		break;
	case SW4_STATE_P1:
		(sw4 -> state) = SW4_P1; // Switch is in P1 position.
		if (_SW4_voltage_is_p0(sw4)) {
			(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
			// Reset confirm start time.
			(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
		}
		else {
			if (_SW4_voltage_is_p2(sw4)) {
				(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
				// Reset confirm start time.
				(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
			}
			else {
				if (_SW4_voltage_is_p3(sw4)) {
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
				}
			}
		}
		break;
	case SW4_STATE_CONFIRM_P2:
		// Check previous state.
		switch ((sw4 -> state)) {
		case SW4_P0:
			if (_SW4_voltage_is_p0(sw4)) {
				// Come back to P0 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P0;
			}
			else {
				if (_SW4_voltage_is_p1(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if (_SW4_voltage_is_p3(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
					}
					else {
						if ((_SW4_voltage_is_p2(sw4)) && (TIM2_get_milliseconds() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P2 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P2;
						}
					}
				}
			}
			break;
		case SW4_P1:
			if (_SW4_voltage_is_p1(sw4)) {
				// Come back to P2 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P1;
			}
			else {
				if (_SW4_voltage_is_p0(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if (_SW4_voltage_is_p3(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
					}
					else {
						if ((_SW4_voltage_is_p2(sw4)) && (TIM2_get_milliseconds() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P2 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P2;
						}
					}
				}
			}
			break;
		case SW4_P3:
			if (_SW4_voltage_is_p3(sw4)) {
				// Come back to P3 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P3;
			}
			else {
				if (_SW4_voltage_is_p1(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if (_SW4_voltage_is_p0(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
					}
					else {
						if ((_SW4_voltage_is_p2(sw4)) && (TIM2_get_milliseconds() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P2 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P2;
						}
					}
				}
			}
			break;
		default:
			// Impossible state.
			break;
		}
		break;
	case SW4_STATE_P2:
		(sw4 -> state) = SW4_P2; // Switch is in P2 position.
		if (_SW4_voltage_is_p0(sw4)) {
			(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
			// Reset confirm start time.
			(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
		}
		else {
			if (_SW4_voltage_is_p1(sw4)) {
				(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
				// Reset confirm start time.
				(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
			}
			else {
				if (_SW4_voltage_is_p3(sw4)) {
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
				}
			}
		}
		break;
	case SW4_STATE_CONFIRM_P3:
		// Check previous state.
		switch ((sw4 -> state)) {
		case SW4_P0:
			if (_SW4_voltage_is_p0(sw4)) {
				// Come back to P1 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P0;
			}
			else {
				if (_SW4_voltage_is_p1(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if (_SW4_voltage_is_p2(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
					}
					else {
						if ((_SW4_voltage_is_p3(sw4)) && (TIM2_get_milliseconds() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P3 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P3;
						}
					}
				}
			}
			break;
		case SW4_P1:
			if (_SW4_voltage_is_p1(sw4)) {
				// Come back to P1 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P1;
			}
			else {
				if (_SW4_voltage_is_p0(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if (_SW4_voltage_is_p2(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
					}
					else {
						if ((_SW4_voltage_is_p3(sw4)) && (TIM2_get_milliseconds() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P3 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P3;
						}
					}
				}
			}
			break;
		case SW4_P2:
			if (_SW4_voltage_is_p2(sw4)) {
				// Come back to P2 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P2;
			}
			else {
				if (_SW4_voltage_is_p0(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
				}
				else {
					if (_SW4_voltage_is_p1(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
					}
					else {
						if ((_SW4_voltage_is_p3(sw4)) && (TIM2_get_milliseconds() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P3 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P3;
						}
					}
				}
			}
			break;
		default:
			// Impossible state.
			break;
		}
		break;
	case SW4_STATE_P3:
		(sw4 -> state) = SW4_P3; // Switch is in P3 position.
		if (_SW4_voltage_is_p0(sw4)) {
			(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
			// Reset confirm start time.
			(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
		}
		else {
			if (_SW4_voltage_is_p1(sw4)) {
				(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
				// Reset confirm start time.
				(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
			}
			else {
				if (_SW4_voltage_is_p2(sw4)) {
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_get_milliseconds();
				}
			}
		}
		break;
	}
}
