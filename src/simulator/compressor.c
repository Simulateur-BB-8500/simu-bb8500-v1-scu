/*
 * compressor.c
 *
 *  Created on: 7 may 2020
 *      Author: Ludo
 */

#include "compressor.h"

#include "lsmcu.h"
#include "lssgiu.h"
#include "manometer.h"
#include "mapping.h"
#include "sw2.h"

/*** COMPRESSOR macros ***/

#define COMPRESSOR_CP_HYSTERESIS_LOW_DECIBARS 		78 // Low threshold of regulation hysteresis.
#define COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_DECIBARS	8 // Pressure range within which no off command is sent (sound will stop by itself).
#define COMPRESSOR_CP_HYSTERESIS_HIGH_DECIBARS 		90 // High threshold of regulation hysteresis.
#define COMPRESSOR_CP_MAXIMUM_VALUE_DECIBARS	 	95 // Maximum value displayed.

/*** COMPRESSOR local structures ***/

typedef enum {
	COMPRESSOR_STATE_OFF,
	COMPRESSOR_STATE_AUTO_ON,
	COMPRESSOR_STATE_AUTO_OFF,
	COMPRESSOR_STATE_DIRECT
} COMPRESSOR_State;

typedef struct {
	// Inputs.
	SW2_Context compressor_zca;
	SW2_Context compressor_zcd;
	// State machine.
	COMPRESSOR_State compressor_state;
	unsigned char compressor_sound_auto_off; // Set when off command is not required.
} COMPRESSOR_Context;

/*** COMPRESSOR external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** COMPRESSOR local global variables ***/

static COMPRESSOR_Context compressor_ctx;

/*** COMPRESSOR functions ***/

/* INIT COMPRESSORRESSOR MODULE.
 * @param:	None.
 * @return:	None.
 */
void COMPRESSOR_Init(void) {
	// Init GPIOs.
	SW2_Init(&compressor_ctx.compressor_zca, &GPIO_BL_ZCA, 1, 100); // ZCA active high.
	SW2_Init(&compressor_ctx.compressor_zcd, &GPIO_BL_ZCD, 0, 100); // ZCD active low.
	// Init context.
	compressor_ctx.compressor_state = COMPRESSOR_STATE_OFF;
	compressor_ctx.compressor_sound_auto_off = 0;
	// Init global context.
	lsmcu_ctx.lsmcu_compressor_on = 0;
}

/* MAIN TASK OF COMPRESSORRESSOR MODULE.
 * @param:	None.
 * @return:	None.
 */
void COMPRESSOR_Task(void) {
	// Update ZCA.
	if (lsmcu_ctx.lsmcu_dj_locked != 0) {
		SW2_UpdateState(&compressor_ctx.compressor_zca);
		SW2_UpdateState(&compressor_ctx.compressor_zcd);
	}
	// Perform state machine.
	switch (compressor_ctx.compressor_state) {
	case COMPRESSOR_STATE_OFF:
		// Update global context.
		lsmcu_ctx.lsmcu_compressor_on = 0;
		// Check DJ.
		if (lsmcu_ctx.lsmcu_dj_locked != 0) {
			// ZCD overrides ZCA.
			if (compressor_ctx.compressor_zcd.sw2_state == SW2_ON) {
				// Play direct sound and set CP needle to maximum.
				LSSGIU_Send(LSMCU_OUT_COMPRESSOR_DIRECT_ON);
				MANOMETER_SetPressure(lsmcu_ctx.lsmcu_manometer_cp, COMPRESSOR_CP_MAXIMUM_VALUE_DECIBARS);
				MANOMETER_NeedleStart(lsmcu_ctx.lsmcu_manometer_cp);
				// Compute next state.
				compressor_ctx.compressor_state = COMPRESSOR_STATE_DIRECT;

			}
			else {
				if (compressor_ctx.compressor_zca.sw2_state == SW2_ON) {
					// Check CP value.
					if (MANOMETER_GetPressure(lsmcu_ctx.lsmcu_manometer_cp) < COMPRESSOR_CP_HYSTERESIS_LOW_DECIBARS) {
						// Play accurate sound.
						if (MANOMETER_GetPressure(lsmcu_ctx.lsmcu_manometer_cp) > (COMPRESSOR_CP_HYSTERESIS_LOW_DECIBARS - COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_DECIBARS)) {
							// Play minimum regulation sound.
							LSSGIU_Send(LSMCU_OUT_COMPRESSOR_AUTO_REG_MIN_ON);
							compressor_ctx.compressor_sound_auto_off = 1;
						}
						else {
							// Play maximum regulation sound.
							LSSGIU_Send(LSMCU_OUT_COMPRESSOR_AUTO_REG_MAX_ON);
							if (MANOMETER_GetPressure(lsmcu_ctx.lsmcu_manometer_cp) < COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_DECIBARS) {
								compressor_ctx.compressor_sound_auto_off = 1;
							}
							else {
								compressor_ctx.compressor_sound_auto_off = 0;
							}
						}
						// Set CP needle to maximum (will be stopped by hysteresis).
						MANOMETER_SetPressure(lsmcu_ctx.lsmcu_manometer_cp, COMPRESSOR_CP_MAXIMUM_VALUE_DECIBARS);
						MANOMETER_NeedleStart(lsmcu_ctx.lsmcu_manometer_cp);
						// Compute next state.
						compressor_ctx.compressor_state = COMPRESSOR_STATE_AUTO_ON;
					}
					else {
						// Auto mode in off zone.
						MANOMETER_NeedleStop(lsmcu_ctx.lsmcu_manometer_cp);
						// Compute next state.
						compressor_ctx.compressor_state = COMPRESSOR_STATE_AUTO_OFF;
					}
				}
			}
		}
		break;
	case COMPRESSOR_STATE_AUTO_ON:
		// Update global context.
		lsmcu_ctx.lsmcu_compressor_on = 1;
		// Check DJ
		if (lsmcu_ctx.lsmcu_dj_locked == 0) {
			// Stop compressor.
			LSSGIU_Send(LSMCU_OUT_COMPRESSOR_OFF);
			MANOMETER_NeedleStop(lsmcu_ctx.lsmcu_manometer_cp);
			// Compute next state.
			compressor_ctx.compressor_state = COMPRESSOR_STATE_OFF;
		}
		else {
			// ZCD overrides ZCA.
			if (compressor_ctx.compressor_zcd.sw2_state == SW2_ON) {
				// Play direct sound and set CP needle to maximum.
				LSSGIU_Send(LSMCU_OUT_COMPRESSOR_DIRECT_ON);
				MANOMETER_SetPressure(lsmcu_ctx.lsmcu_manometer_cp, COMPRESSOR_CP_MAXIMUM_VALUE_DECIBARS);
				MANOMETER_NeedleStart(lsmcu_ctx.lsmcu_manometer_cp);
				// Compute next state.
				compressor_ctx.compressor_state = COMPRESSOR_STATE_DIRECT;

			}
			else {
				if (compressor_ctx.compressor_zca.sw2_state == SW2_ON) {
					// Perform hysterersis.
					if (MANOMETER_GetPressure(lsmcu_ctx.lsmcu_manometer_cp) > COMPRESSOR_CP_HYSTERESIS_HIGH_DECIBARS) {
						// Stop regulation.
						if (compressor_ctx.compressor_sound_auto_off == 0) {
							LSSGIU_Send(LSMCU_OUT_COMPRESSOR_OFF);
						}
						MANOMETER_NeedleStop(lsmcu_ctx.lsmcu_manometer_cp);
						// Compute next state.
						compressor_ctx.compressor_state = COMPRESSOR_STATE_AUTO_OFF;
					}
				}
				else {
					// Compressor off.
					LSSGIU_Send(LSMCU_OUT_COMPRESSOR_OFF);
					MANOMETER_NeedleStop(lsmcu_ctx.lsmcu_manometer_cp);
					// Compute next state.
					compressor_ctx.compressor_state = COMPRESSOR_STATE_OFF;
				}
			}
		}
		break;
	case COMPRESSOR_STATE_AUTO_OFF:
		// Update global context.
		lsmcu_ctx.lsmcu_compressor_on = 0;
		// Check DJ
		if (lsmcu_ctx.lsmcu_dj_locked == 0) {
			// Compute next state.
			compressor_ctx.compressor_state = COMPRESSOR_STATE_OFF;
		}
		else {
			// ZCD overrides ZCA.
			if (compressor_ctx.compressor_zcd.sw2_state == SW2_ON) {
				// Play direct sound and set CP needle to maximum.
				LSSGIU_Send(LSMCU_OUT_COMPRESSOR_DIRECT_ON);
				MANOMETER_SetPressure(lsmcu_ctx.lsmcu_manometer_cp, COMPRESSOR_CP_MAXIMUM_VALUE_DECIBARS);
				MANOMETER_NeedleStart(lsmcu_ctx.lsmcu_manometer_cp);
				// Compute next state.
				compressor_ctx.compressor_state = COMPRESSOR_STATE_DIRECT;

			}
			else {
				if (compressor_ctx.compressor_zca.sw2_state == SW2_ON) {
					// Check CP value.
					if (MANOMETER_GetPressure(lsmcu_ctx.lsmcu_manometer_cp) < COMPRESSOR_CP_HYSTERESIS_LOW_DECIBARS) {
						// Play accurate sound.
						if (MANOMETER_GetPressure(lsmcu_ctx.lsmcu_manometer_cp) > (COMPRESSOR_CP_HYSTERESIS_LOW_DECIBARS - COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_DECIBARS)) {
							// Play minimum regulation sound.
							LSSGIU_Send(LSMCU_OUT_COMPRESSOR_AUTO_REG_MIN_ON);
							compressor_ctx.compressor_sound_auto_off = 1;
						}
						else {
							// Play maximum regulation sound.
							LSSGIU_Send(LSMCU_OUT_COMPRESSOR_AUTO_REG_MAX_ON);
							if (MANOMETER_GetPressure(lsmcu_ctx.lsmcu_manometer_cp) < COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_DECIBARS) {
								compressor_ctx.compressor_sound_auto_off = 1;
							}
							else {
								compressor_ctx.compressor_sound_auto_off = 0;
							}
						}
						// Set CP needle to maximum (will be stopped by hysteresis).
						MANOMETER_SetPressure(lsmcu_ctx.lsmcu_manometer_cp, COMPRESSOR_CP_MAXIMUM_VALUE_DECIBARS);
						MANOMETER_NeedleStart(lsmcu_ctx.lsmcu_manometer_cp);
						// Compute next state.
						compressor_ctx.compressor_state = COMPRESSOR_STATE_AUTO_ON;
					}
				}
				else {
					// Compute next state.
					compressor_ctx.compressor_state = COMPRESSOR_STATE_OFF;
				}
			}
		}
		break;
	case COMPRESSOR_STATE_DIRECT:
		// Update global context.
		lsmcu_ctx.lsmcu_compressor_on = 1;
		// Check DJ
		if (lsmcu_ctx.lsmcu_dj_locked == 0) {
			// Stop compressor.
			LSSGIU_Send(LSMCU_OUT_COMPRESSOR_OFF);
			MANOMETER_NeedleStop(lsmcu_ctx.lsmcu_manometer_cp);
			// Compute next state.
			compressor_ctx.compressor_state = COMPRESSOR_STATE_OFF;
		}
		else {
			// ZCD overrides ZCA.
			if (compressor_ctx.compressor_zcd.sw2_state == SW2_OFF) {
				// Check ZCA.
				if (compressor_ctx.compressor_zca.sw2_state == SW2_ON) {
					// Check CP value.
					if (MANOMETER_GetPressure(lsmcu_ctx.lsmcu_manometer_cp) < COMPRESSOR_CP_HYSTERESIS_LOW_DECIBARS) {
						// Play accurate sound.
						if (MANOMETER_GetPressure(lsmcu_ctx.lsmcu_manometer_cp) > (COMPRESSOR_CP_HYSTERESIS_LOW_DECIBARS - COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_DECIBARS)) {
							// Play minimum regulation sound.
							LSSGIU_Send(LSMCU_OUT_COMPRESSOR_AUTO_REG_MIN_ON);
							compressor_ctx.compressor_sound_auto_off = 1;
						}
						else {
							// Play maximum regulation sound.
							LSSGIU_Send(LSMCU_OUT_COMPRESSOR_AUTO_REG_MAX_ON);
							if (MANOMETER_GetPressure(lsmcu_ctx.lsmcu_manometer_cp) < COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_DECIBARS) {
								compressor_ctx.compressor_sound_auto_off = 1;
							}
							else {
								compressor_ctx.compressor_sound_auto_off = 0;
							}
						}
						// Set CP needle to maximum (will be stopped by hysteresis).
						MANOMETER_SetPressure(lsmcu_ctx.lsmcu_manometer_cp, COMPRESSOR_CP_MAXIMUM_VALUE_DECIBARS);
						MANOMETER_NeedleStart(lsmcu_ctx.lsmcu_manometer_cp);
						// Compute next state.
						compressor_ctx.compressor_state = COMPRESSOR_STATE_AUTO_ON;
					}
					else {
						// Auto mode in off zone.
						MANOMETER_NeedleStop(lsmcu_ctx.lsmcu_manometer_cp);
						// Compute next state.
						compressor_ctx.compressor_state = COMPRESSOR_STATE_AUTO_OFF;
					}
				}
				else {
					// Stop compressor.
					LSSGIU_Send(LSMCU_OUT_COMPRESSOR_OFF);
					MANOMETER_NeedleStop(lsmcu_ctx.lsmcu_manometer_cp);
					// Compute next state.
					compressor_ctx.compressor_state = COMPRESSOR_STATE_OFF;
				}
			}
		}
		break;
	default:
		// Unknown state.
		compressor_ctx.compressor_state = COMPRESSOR_STATE_OFF;
		break;
	}
}
