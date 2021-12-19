/*
 * sw4.c
 *
 *  Created on: 26 dec. 2017
 *      Author: Ludo
 */

#include "sw4.h"

#include "adc.h"
#include "gpio.h"
#include "tim.h"

/*** SW4 local macros ***/

#define SW4_DELTA_HYSTERESIS_MV		100 // Set the voltage difference (in mV) between low and high thresholds.
#define SW4_P0P1_THRESHOLD_LOW 		((VCC_MV/6)-(SW4_DELTA_HYSTERESIS_MV/2))
#define SW4_P0P1_THRESHOLD_HIGH 	((VCC_MV/6)+(SW4_DELTA_HYSTERESIS_MV/2))
#define SW4_P1P2_THRESHOLD_LOW 		(((3*VCC_MV)/6)-(SW4_DELTA_HYSTERESIS_MV/2))
#define SW4_P1P2_THRESHOLD_HIGH 	(((3*VCC_MV)/6)+(SW4_DELTA_HYSTERESIS_MV/2))
#define SW4_P2P3_THRESHOLD_LOW 		(((5*VCC_MV)/6)-(SW4_DELTA_HYSTERESIS_MV/2))
#define SW4_P2P3_THRESHOLD_HIGH 	(((5*VCC_MV)/6)+(SW4_DELTA_HYSTERESIS_MV/2))

/*** SW4 local functions ***/

/* CHECK IF A 4-POLES SWITCH IS IN P0 POSITION.
 * @param sw4:		The switch to analyse.
 * @return result:	'1' if switch voltage indicates a P0 position, '0' otherwise.
 */
static unsigned char SW4_VoltageIsP0(SW4_Context* sw4) {
	unsigned char result = 0;
	if ((sw4 -> voltage) < SW4_P0P1_THRESHOLD_LOW) {
		result = 1;
	}
	return result;
}

/* CHECK IF A 4-POLES SWITCH IS IN P1 POSITION.
 * @param sw4:		The switch to analyse.
 * @return result:	'1' if switch voltage indicates a P1 position, '0' otherwise.
 */
static unsigned char SW4_VoltageIsP1(SW4_Context* sw4) {
	unsigned char result = 0;
	if (((sw4 -> voltage) > SW4_P0P1_THRESHOLD_HIGH) && ((sw4 -> voltage) < SW4_P1P2_THRESHOLD_LOW)) {
		result = 1;
	}
	return result;
}

/* CHECK IF A 4-POLES SWITCH IS IN P2 POSITION.
 * @param sw4:		The switch to analyse.
 * @return result:	'1' if switch voltage indicates a P2 position, '0' otherwise.
 */
static unsigned char SW4_VoltageIsP2(SW4_Context* sw4) {
	unsigned char result = 0;
	if (((sw4 -> voltage) > SW4_P1P2_THRESHOLD_HIGH) && ((sw4 -> voltage) < SW4_P2P3_THRESHOLD_LOW)) {
		result = 1;
	}
	return result;
}

/* CHECK IF A 4-POLES SWITCH IS IN P3 POSITION.
 * @param sw4:		The switch to analyse.
 * @return result:	'1' if switch voltage indicates a P3 position, '0' otherwise.
 */
static unsigned char SW4_VoltageIsP3(SW4_Context* sw4) {
	unsigned char result = 0;
	if ((sw4 -> voltage) > SW4_P2P3_THRESHOLD_HIGH) {
		result = 1;
	}
	return result;
}

/*** SW4 functions ***/

/* INITIALISE AN SW4 STRUCTURE.
 * @param sw4:				Switch structure to initialize.
 * @param gpio:				GPIO attached to the switch.
 * @param debouncing_ms:	Delay before validating ON/OFF state (in ms).
 * @return:					None;
 */
void SW4_Init(SW4_Context* sw4, const GPIO* gpio, unsigned int debouncing_ms) {
	// Init GPIO.
	GPIO_Configure(gpio, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init context.
	(sw4 -> voltage) = 0; // P0 = 0V.
	(sw4 -> internal_state) = SW4_STATE_P0;
	(sw4 -> state) = SW4_P0;
	(sw4 -> debouncing_ms) = debouncing_ms;
	(sw4 -> confirm_start_time) = 0;
}

/* SET THE CURRENT VOLTAGE OF A 3-POLES SWITCH.
 * @param sw4:			The switch to set.
 * @param newVoltage:	New voltage measured by ADC.
 */
void SW4_SetVoltageMv(SW4_Context* sw4, unsigned int voltage_mv) {
	(sw4 -> voltage) = voltage_mv;
}

/* UPDATE THE STATE OF AN SW4 STRUCTURE PERFORMING HYSTERESIS AND CONFIRMATION.
 * @param sw4:	The switch to analyse.
 * @return:		None.
 */
void SW4_UpdateState(SW4_Context* sw4) {
	switch((sw4 -> internal_state)) {
	case SW4_STATE_CONFIRM_P0:
		// Check previous state.
		switch ((sw4 -> state)) {
		case SW4_P1:
			if (SW4_VoltageIsP1(sw4)) {
				// Come back to P1 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P1;
			}
			else {
				if (SW4_VoltageIsP2(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_GetMs();
				}
				else {
					if (SW4_VoltageIsP3(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_GetMs();
					}
					else {
						if ((SW4_VoltageIsP0(sw4)) && (TIM2_GetMs() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P0 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P0;
						}
					}
				}
			}
			break;
		case SW4_P2:
			if (SW4_VoltageIsP2(sw4)) {
				// Come back to P2 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P2;
			}
			else {
				if (SW4_VoltageIsP1(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_GetMs();
				}
				else {
					if (SW4_VoltageIsP3(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_GetMs();
					}
					else {
						if ((SW4_VoltageIsP0(sw4)) && (TIM2_GetMs() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P0 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P0;
						}
					}
				}
			}
			break;
		case SW4_P3:
			if (SW4_VoltageIsP3(sw4)) {
				// Come back to P3 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P3;
			}
			else {
				if (SW4_VoltageIsP1(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_GetMs();
				}
				else {
					if (SW4_VoltageIsP2(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_GetMs();
					}
					else {
						if ((SW4_VoltageIsP0(sw4)) && (TIM2_GetMs() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
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
		if (SW4_VoltageIsP1(sw4)) {
			(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
			// Reset confirm start time.
			(sw4 -> confirm_start_time) = TIM2_GetMs();
		}
		else {
			if (SW4_VoltageIsP2(sw4)) {
				(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
				// Reset confirm start time.
				(sw4 -> confirm_start_time) = TIM2_GetMs();
			}
			else {
				if (SW4_VoltageIsP3(sw4)) {
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_GetMs();
				}
			}
		}
		break;
	case SW4_STATE_CONFIRM_P1:
		// Check previous state.
		switch ((sw4 -> state)) {
		case SW4_P0:
			if (SW4_VoltageIsP0(sw4)) {
				// Come back to P0 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P0;
			}
			else {
				if (SW4_VoltageIsP2(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_GetMs();
				}
				else {
					if (SW4_VoltageIsP3(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_GetMs();
					}
					else {
						if ((SW4_VoltageIsP1(sw4)) && (TIM2_GetMs() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P1 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P1;
						}
					}
				}
			}
			break;
		case SW4_P2:
			if (SW4_VoltageIsP2(sw4)) {
				// Come back to P2 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P2;
			}
			else {
				if (SW4_VoltageIsP0(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_GetMs();
				}
				else {
					if (SW4_VoltageIsP3(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_GetMs();
					}
					else {
						if ((SW4_VoltageIsP1(sw4)) && (TIM2_GetMs() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P1 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P1;
						}
					}
				}
			}
			break;
		case SW4_P3:
			if (SW4_VoltageIsP3(sw4)) {
				// Come back to P3 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P3;
			}
			else {
				if (SW4_VoltageIsP0(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_GetMs();
				}
				else {
					if (SW4_VoltageIsP2(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_GetMs();
					}
					else {
						if ((SW4_VoltageIsP1(sw4)) && (TIM2_GetMs() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
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
		if (SW4_VoltageIsP0(sw4)) {
			(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
			// Reset confirm start time.
			(sw4 -> confirm_start_time) = TIM2_GetMs();
		}
		else {
			if (SW4_VoltageIsP2(sw4)) {
				(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
				// Reset confirm start time.
				(sw4 -> confirm_start_time) = TIM2_GetMs();
			}
			else {
				if (SW4_VoltageIsP3(sw4)) {
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_GetMs();
				}
			}
		}
		break;
	case SW4_STATE_CONFIRM_P2:
		// Check previous state.
		switch ((sw4 -> state)) {
		case SW4_P0:
			if (SW4_VoltageIsP0(sw4)) {
				// Come back to P0 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P0;
			}
			else {
				if (SW4_VoltageIsP1(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_GetMs();
				}
				else {
					if (SW4_VoltageIsP3(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_GetMs();
					}
					else {
						if ((SW4_VoltageIsP2(sw4)) && (TIM2_GetMs() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P2 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P2;
						}
					}
				}
			}
			break;
		case SW4_P1:
			if (SW4_VoltageIsP1(sw4)) {
				// Come back to P2 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P1;
			}
			else {
				if (SW4_VoltageIsP0(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_GetMs();
				}
				else {
					if (SW4_VoltageIsP3(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_GetMs();
					}
					else {
						if ((SW4_VoltageIsP2(sw4)) && (TIM2_GetMs() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P2 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P2;
						}
					}
				}
			}
			break;
		case SW4_P3:
			if (SW4_VoltageIsP3(sw4)) {
				// Come back to P3 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P3;
			}
			else {
				if (SW4_VoltageIsP1(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_GetMs();
				}
				else {
					if (SW4_VoltageIsP0(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_GetMs();
					}
					else {
						if ((SW4_VoltageIsP2(sw4)) && (TIM2_GetMs() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
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
		if (SW4_VoltageIsP0(sw4)) {
			(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
			// Reset confirm start time.
			(sw4 -> confirm_start_time) = TIM2_GetMs();
		}
		else {
			if (SW4_VoltageIsP1(sw4)) {
				(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
				// Reset confirm start time.
				(sw4 -> confirm_start_time) = TIM2_GetMs();
			}
			else {
				if (SW4_VoltageIsP3(sw4)) {
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P3;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_GetMs();
				}
			}
		}
		break;
	case SW4_STATE_CONFIRM_P3:
		// Check previous state.
		switch ((sw4 -> state)) {
		case SW4_P0:
			if (SW4_VoltageIsP0(sw4)) {
				// Come back to P1 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P0;
			}
			else {
				if (SW4_VoltageIsP1(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_GetMs();
				}
				else {
					if (SW4_VoltageIsP2(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_GetMs();
					}
					else {
						if ((SW4_VoltageIsP3(sw4)) && (TIM2_GetMs() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P3 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P3;
						}
					}
				}
			}
			break;
		case SW4_P1:
			if (SW4_VoltageIsP1(sw4)) {
				// Come back to P1 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P1;
			}
			else {
				if (SW4_VoltageIsP0(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_GetMs();
				}
				else {
					if (SW4_VoltageIsP2(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_GetMs();
					}
					else {
						if ((SW4_VoltageIsP3(sw4)) && (TIM2_GetMs() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
							// P3 position confirmed.
							(sw4 -> internal_state) = SW4_STATE_P3;
						}
					}
				}
			}
			break;
		case SW4_P2:
			if (SW4_VoltageIsP2(sw4)) {
				// Come back to P2 without confirmation because it's the previous confirmed state.
				(sw4 -> internal_state) = SW4_STATE_P2;
			}
			else {
				if (SW4_VoltageIsP0(sw4)) {
					// New state to confirm.
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_GetMs();
				}
				else {
					if (SW4_VoltageIsP1(sw4)) {
						// New state to confirm.
						(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
						// Reset confirm start time.
						(sw4 -> confirm_start_time) = TIM2_GetMs();
					}
					else {
						if ((SW4_VoltageIsP3(sw4)) && (TIM2_GetMs() > ((sw4 -> confirm_start_time)) + (sw4 -> debouncing_ms))) {
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
		if (SW4_VoltageIsP0(sw4)) {
			(sw4 -> internal_state) = SW4_STATE_CONFIRM_P0;
			// Reset confirm start time.
			(sw4 -> confirm_start_time) = TIM2_GetMs();
		}
		else {
			if (SW4_VoltageIsP1(sw4)) {
				(sw4 -> internal_state) = SW4_STATE_CONFIRM_P1;
				// Reset confirm start time.
				(sw4 -> confirm_start_time) = TIM2_GetMs();
			}
			else {
				if (SW4_VoltageIsP2(sw4)) {
					(sw4 -> internal_state) = SW4_STATE_CONFIRM_P2;
					// Reset confirm start time.
					(sw4 -> confirm_start_time) = TIM2_GetMs();
				}
			}
		}
		break;
	}
}
