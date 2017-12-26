/*
 * sw4.c
 *
 *  Created on: 26 dec. 2017
 *      Author: Ludovic
 */

#include "adc.h"
#include "gpio.h"
#include "sw4.h"
#include "tim.h"
#include "enum.h"

/*** 4-poles switch internal functions ***/

/* CHECK IF A 4-POLES SWITCH IS IN P0 POSITION.
 * @param sw4:		The switch to analyse.
 * @return result:	'true' if switch voltage indicates a P0 position, 'false' otherwise.
 */
boolean SW4_VoltageIsP0(SW4_Struct* sw4) {
	boolean result = false;
	if ((sw4 -> voltage) < SW4_P0P1_THRESHOLD_LOW) {
		result = true;
	}
	return result;
}

/* CHECK IF A 4-POLES SWITCH IS IN P1 POSITION.
 * @param sw4:		The switch to analyse.
 * @return result:	'true' if switch voltage indicates a P1 position, 'false' otherwise.
 */
boolean SW4_VoltageIsP1(SW4_Struct* sw4) {
	boolean result = false;
	if (((sw4 -> voltage) > SW4_P0P1_THRESHOLD_HIGH) && ((sw4 -> voltage) < SW4_P1P2_THRESHOLD_LOW)) {
		result = true;
	}
	return result;
}

/* CHECK IF A 4-POLES SWITCH IS IN P2 POSITION.
 * @param sw4:		The switch to analyse.
 * @return result:	'true' if switch voltage indicates a P2 position, 'false' otherwise.
 */
boolean SW4_VoltageIsP2(SW4_Struct* sw4) {
	boolean result = false;
	if (((sw4 -> voltage) > SW4_P1P2_THRESHOLD_HIGH) && ((sw4 -> voltage) < SW4_P2P3_THRESHOLD_LOW)) {
		result = true;
	}
	return result;
}

/* CHECK IF A 4-POLES SWITCH IS IN P3 POSITION.
 * @param sw4:		The switch to analyse.
 * @return result:	'true' if switch voltage indicates a P3 position, 'false' otherwise.
 */
boolean SW4_VoltageIsP3(SW4_Struct* sw4) {
	boolean result = false;
	if ((sw4 -> voltage) > SW4_P2P3_THRESHOLD_HIGH) {
		result = true;
	}
	return result;
}

/*** 4-poles switch functions ***/

/* INITIALISE AN SW4 STRUCTURE.
 * @param sw4:				Switch structure to initialise.
 * @param pGpio:			GPIO reading the switch.
 * @param pAdc:				ADC used for measuring the voltage.
 * @param pDebouncingMs:	Delay before validating ON/ODD state (in ms).
 * @return:					None;
 */
void SW4_Init(SW4_Struct* sw4, GPIO_Struct* pGpio, ADC_Struct* pAdc, unsigned int pDebouncingMs) {
	sw4 -> gpio = pGpio;
	sw4 -> adc = pAdc;
	sw4 -> voltage = 0; // P0 = 0V.
	sw4 -> machineState = SW4_P0;
	sw4 -> state = P0;
	sw4 -> debouncingMs = pDebouncingMs;
	sw4 -> confirmStartTime = 0;
}

/* SET THE CURRENT VOLTAGE OF A 3-POLES SWITCH.
 * @param sw4:			The switch to set.
 * @param newVoltage:	New voltage measured by ADC.
 */
void SW4_SetVoltage(SW4_Struct* sw4, unsigned int newVoltage) {
	sw4 -> voltage = newVoltage;
}

/* UPDATE THE STATE OF AN SW4 STRUCTURE.
 * @param sw4:	The switch to analyse.
 * @return:		None.
 */
void SW4_UpdateState(SW4_Struct* sw4) {
	switch(sw4 -> machineState) {
	case SW4_CONFIRM_P0:
		// Check previous state.
		switch (sw4 -> state) {
		case P1:
			if (SW4_VoltageIsP1(sw4)) {
				// Come back to P1 without confirmation because it's the previous confirmed state.
				sw4 -> machineState = SW4_P1;
			}
			else {
				if (SW4_VoltageIsP2(sw4)) {
					// New state to confirm.
					sw4 -> machineState = SW4_CONFIRM_P2;
					// Reset confirm start time.
					sw4 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if (SW4_VoltageIsP3(sw4)) {
						// New state to confirm.
						sw4 -> machineState = SW4_CONFIRM_P3;
						// Reset confirm start time.
						sw4 -> confirmStartTime = TIM_GetMs();
					}
					else {
						if ((SW4_VoltageIsP0(sw4)) && (TIM_GetMs() > (sw4 -> confirmStartTime) + (sw4 -> debouncingMs))) {
							// P0 position confirmed.
							sw4 -> machineState = SW4_P0;
						}
					}
				}
			}
			break;
		case P2:
			if (SW4_VoltageIsP2(sw4)) {
				// Come back to P2 without confirmation because it's the previous confirmed state.
				sw4 -> machineState = SW4_P2;
			}
			else {
				if (SW4_VoltageIsP1(sw4)) {
					// New state to confirm.
					sw4 -> machineState = SW4_CONFIRM_P1;
					// Reset confirm start time.
					sw4 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if (SW4_VoltageIsP3(sw4)) {
						// New state to confirm.
						sw4 -> machineState = SW4_CONFIRM_P3;
						// Reset confirm start time.
						sw4 -> confirmStartTime = TIM_GetMs();
					}
					else {
						if ((SW4_VoltageIsP0(sw4)) && (TIM_GetMs() > (sw4 -> confirmStartTime) + (sw4 -> debouncingMs))) {
							// P0 position confirmed.
							sw4 -> machineState = SW4_P0;
						}
					}
				}
			}
			break;
		case P3:
			if (SW4_VoltageIsP3(sw4)) {
				// Come back to P3 without confirmation because it's the previous confirmed state.
				sw4 -> machineState = SW4_P3;
			}
			else {
				if (SW4_VoltageIsP1(sw4)) {
					// New state to confirm.
					sw4 -> machineState = SW4_CONFIRM_P1;
					// Reset confirm start time.
					sw4 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if (SW4_VoltageIsP2(sw4)) {
						// New state to confirm.
						sw4 -> machineState = SW4_CONFIRM_P2;
						// Reset confirm start time.
						sw4 -> confirmStartTime = TIM_GetMs();
					}
					else {
						if ((SW4_VoltageIsP0(sw4)) && (TIM_GetMs() > (sw4 -> confirmStartTime) + (sw4 -> debouncingMs))) {
							// P0 position confirmed.
							sw4 -> machineState = SW4_P0;
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
	case SW4_P0:
		sw4 -> state = P0; // Switch is in P0 position.
		if (SW4_VoltageIsP1(sw4)) {
			sw4 -> machineState = SW4_CONFIRM_P1;
			// Reset confirm start time.
			sw4 -> confirmStartTime = TIM_GetMs();
		}
		else {
			if (SW4_VoltageIsP2(sw4)) {
				sw4 -> machineState = SW4_CONFIRM_P2;
				// Reset confirm start time.
				sw4 -> confirmStartTime = TIM_GetMs();
			}
			else {
				if (SW4_VoltageIsP3(sw4)) {
					sw4 -> machineState = SW4_CONFIRM_P3;
					// Reset confirm start time.
					sw4 -> confirmStartTime = TIM_GetMs();
				}
			}
		}
		break;
	case SW4_CONFIRM_P1:
		// Check previous state.
		switch (sw4 -> state) {
		case P0:
			if (SW4_VoltageIsP0(sw4)) {
				// Come back to P0 without confirmation because it's the previous confirmed state.
				sw4 -> machineState = SW4_P0;
			}
			else {
				if (SW4_VoltageIsP2(sw4)) {
					// New state to confirm.
					sw4 -> machineState = SW4_CONFIRM_P2;
					// Reset confirm start time.
					sw4 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if (SW4_VoltageIsP3(sw4)) {
						// New state to confirm.
						sw4 -> machineState = SW4_CONFIRM_P3;
						// Reset confirm start time.
						sw4 -> confirmStartTime = TIM_GetMs();
					}
					else {
						if ((SW4_VoltageIsP1(sw4)) && (TIM_GetMs() > (sw4 -> confirmStartTime) + (sw4 -> debouncingMs))) {
							// P1 position confirmed.
							sw4 -> machineState = SW4_P1;
						}
					}
				}
			}
			break;
		case P2:
			if (SW4_VoltageIsP2(sw4)) {
				// Come back to P2 without confirmation because it's the previous confirmed state.
				sw4 -> machineState = SW4_P2;
			}
			else {
				if (SW4_VoltageIsP0(sw4)) {
					// New state to confirm.
					sw4 -> machineState = SW4_CONFIRM_P0;
					// Reset confirm start time.
					sw4 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if (SW4_VoltageIsP3(sw4)) {
						// New state to confirm.
						sw4 -> machineState = SW4_CONFIRM_P3;
						// Reset confirm start time.
						sw4 -> confirmStartTime = TIM_GetMs();
					}
					else {
						if ((SW4_VoltageIsP1(sw4)) && (TIM_GetMs() > (sw4 -> confirmStartTime) + (sw4 -> debouncingMs))) {
							// P1 position confirmed.
							sw4 -> machineState = SW4_P1;
						}
					}
				}
			}
			break;
		case P3:
			if (SW4_VoltageIsP3(sw4)) {
				// Come back to P3 without confirmation because it's the previous confirmed state.
				sw4 -> machineState = SW4_P3;
			}
			else {
				if (SW4_VoltageIsP0(sw4)) {
					// New state to confirm.
					sw4 -> machineState = SW4_CONFIRM_P0;
					// Reset confirm start time.
					sw4 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if (SW4_VoltageIsP2(sw4)) {
						// New state to confirm.
						sw4 -> machineState = SW4_CONFIRM_P2;
						// Reset confirm start time.
						sw4 -> confirmStartTime = TIM_GetMs();
					}
					else {
						if ((SW4_VoltageIsP1(sw4)) && (TIM_GetMs() > (sw4 -> confirmStartTime) + (sw4 -> debouncingMs))) {
							// P1 position confirmed.
							sw4 -> machineState = SW4_P1;
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
	case SW4_P1:
		sw4 -> state = P1; // Switch is in P1 position.
		if (SW4_VoltageIsP0(sw4)) {
			sw4 -> machineState = SW4_CONFIRM_P0;
			// Reset confirm start time.
			sw4 -> confirmStartTime = TIM_GetMs();
		}
		else {
			if (SW4_VoltageIsP2(sw4)) {
				sw4 -> machineState = SW4_CONFIRM_P2;
				// Reset confirm start time.
				sw4 -> confirmStartTime = TIM_GetMs();
			}
			else {
				if (SW4_VoltageIsP3(sw4)) {
					sw4 -> machineState = SW4_CONFIRM_P3;
					// Reset confirm start time.
					sw4 -> confirmStartTime = TIM_GetMs();
				}
			}
		}
		break;
	case SW4_CONFIRM_P2:
		// Check previous state.
		switch (sw4 -> state) {
		case P0:
			if (SW4_VoltageIsP0(sw4)) {
				// Come back to P0 without confirmation because it's the previous confirmed state.
				sw4 -> machineState = SW4_P0;
			}
			else {
				if (SW4_VoltageIsP1(sw4)) {
					// New state to confirm.
					sw4 -> machineState = SW4_CONFIRM_P1;
					// Reset confirm start time.
					sw4 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if (SW4_VoltageIsP3(sw4)) {
						// New state to confirm.
						sw4 -> machineState = SW4_CONFIRM_P3;
						// Reset confirm start time.
						sw4 -> confirmStartTime = TIM_GetMs();
					}
					else {
						if ((SW4_VoltageIsP2(sw4)) && (TIM_GetMs() > (sw4 -> confirmStartTime) + (sw4 -> debouncingMs))) {
							// P2 position confirmed.
							sw4 -> machineState = SW4_P2;
						}
					}
				}
			}
			break;
		case P1:
			if (SW4_VoltageIsP1(sw4)) {
				// Come back to P2 without confirmation because it's the previous confirmed state.
				sw4 -> machineState = SW4_P1;
			}
			else {
				if (SW4_VoltageIsP0(sw4)) {
					// New state to confirm.
					sw4 -> machineState = SW4_CONFIRM_P0;
					// Reset confirm start time.
					sw4 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if (SW4_VoltageIsP3(sw4)) {
						// New state to confirm.
						sw4 -> machineState = SW4_CONFIRM_P3;
						// Reset confirm start time.
						sw4 -> confirmStartTime = TIM_GetMs();
					}
					else {
						if ((SW4_VoltageIsP2(sw4)) && (TIM_GetMs() > (sw4 -> confirmStartTime) + (sw4 -> debouncingMs))) {
							// P2 position confirmed.
							sw4 -> machineState = SW4_P2;
						}
					}
				}
			}
			break;
		case P3:
			if (SW4_VoltageIsP3(sw4)) {
				// Come back to P3 without confirmation because it's the previous confirmed state.
				sw4 -> machineState = SW4_P3;
			}
			else {
				if (SW4_VoltageIsP1(sw4)) {
					// New state to confirm.
					sw4 -> machineState = SW4_CONFIRM_P1;
					// Reset confirm start time.
					sw4 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if (SW4_VoltageIsP0(sw4)) {
						// New state to confirm.
						sw4 -> machineState = SW4_CONFIRM_P0;
						// Reset confirm start time.
						sw4 -> confirmStartTime = TIM_GetMs();
					}
					else {
						if ((SW4_VoltageIsP2(sw4)) && (TIM_GetMs() > (sw4 -> confirmStartTime) + (sw4 -> debouncingMs))) {
							// P2 position confirmed.
							sw4 -> machineState = SW4_P2;
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
	case SW4_P2:
		sw4 -> state = P2; // Switch is in P2 position.
		if (SW4_VoltageIsP0(sw4)) {
			sw4 -> machineState = SW4_CONFIRM_P0;
			// Reset confirm start time.
			sw4 -> confirmStartTime = TIM_GetMs();
		}
		else {
			if (SW4_VoltageIsP1(sw4)) {
				sw4 -> machineState = SW4_CONFIRM_P1;
				// Reset confirm start time.
				sw4 -> confirmStartTime = TIM_GetMs();
			}
			else {
				if (SW4_VoltageIsP3(sw4)) {
					sw4 -> machineState = SW4_CONFIRM_P3;
					// Reset confirm start time.
					sw4 -> confirmStartTime = TIM_GetMs();
				}
			}
		}
		break;
	case SW4_CONFIRM_P3:
		// Check previous state.
		switch (sw4 -> state) {
		case P0:
			if (SW4_VoltageIsP0(sw4)) {
				// Come back to P1 without confirmation because it's the previous confirmed state.
				sw4 -> machineState = SW4_P0;
			}
			else {
				if (SW4_VoltageIsP1(sw4)) {
					// New state to confirm.
					sw4 -> machineState = SW4_CONFIRM_P1;
					// Reset confirm start time.
					sw4 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if (SW4_VoltageIsP2(sw4)) {
						// New state to confirm.
						sw4 -> machineState = SW4_CONFIRM_P2;
						// Reset confirm start time.
						sw4 -> confirmStartTime = TIM_GetMs();
					}
					else {
						if ((SW4_VoltageIsP3(sw4)) && (TIM_GetMs() > (sw4 -> confirmStartTime) + (sw4 -> debouncingMs))) {
							// P3 position confirmed.
							sw4 -> machineState = SW4_P3;
						}
					}
				}
			}
			break;
		case P1:
			if (SW4_VoltageIsP1(sw4)) {
				// Come back to P1 without confirmation because it's the previous confirmed state.
				sw4 -> machineState = SW4_P1;
			}
			else {
				if (SW4_VoltageIsP0(sw4)) {
					// New state to confirm.
					sw4 -> machineState = SW4_CONFIRM_P0;
					// Reset confirm start time.
					sw4 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if (SW4_VoltageIsP2(sw4)) {
						// New state to confirm.
						sw4 -> machineState = SW4_CONFIRM_P2;
						// Reset confirm start time.
						sw4 -> confirmStartTime = TIM_GetMs();
					}
					else {
						if ((SW4_VoltageIsP3(sw4)) && (TIM_GetMs() > (sw4 -> confirmStartTime) + (sw4 -> debouncingMs))) {
							// P3 position confirmed.
							sw4 -> machineState = SW4_P3;
						}
					}
				}
			}
			break;
		case P2:
			if (SW4_VoltageIsP2(sw4)) {
				// Come back to P2 without confirmation because it's the previous confirmed state.
				sw4 -> machineState = SW4_P2;
			}
			else {
				if (SW4_VoltageIsP0(sw4)) {
					// New state to confirm.
					sw4 -> machineState = SW4_CONFIRM_P0;
					// Reset confirm start time.
					sw4 -> confirmStartTime = TIM_GetMs();
				}
				else {
					if (SW4_VoltageIsP1(sw4)) {
						// New state to confirm.
						sw4 -> machineState = SW4_CONFIRM_P1;
						// Reset confirm start time.
						sw4 -> confirmStartTime = TIM_GetMs();
					}
					else {
						if ((SW4_VoltageIsP3(sw4)) && (TIM_GetMs() > (sw4 -> confirmStartTime) + (sw4 -> debouncingMs))) {
							// P3 position confirmed.
							sw4 -> machineState = SW4_P3;
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
	case SW4_P3:
		sw4 -> state = P3; // Switch is in P3 position.
		if (SW4_VoltageIsP0(sw4)) {
			sw4 -> machineState = SW4_CONFIRM_P0;
			// Reset confirm start time.
			sw4 -> confirmStartTime = TIM_GetMs();
		}
		else {
			if (SW4_VoltageIsP1(sw4)) {
				sw4 -> machineState = SW4_CONFIRM_P1;
				// Reset confirm start time.
				sw4 -> confirmStartTime = TIM_GetMs();
			}
			else {
				if (SW4_VoltageIsP2(sw4)) {
					sw4 -> machineState = SW4_CONFIRM_P2;
					// Reset confirm start time.
					sw4 -> confirmStartTime = TIM_GetMs();
				}
			}
		}
		break;
	}
}
