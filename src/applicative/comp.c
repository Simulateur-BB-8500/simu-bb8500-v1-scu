/*
 * comp.c
 *
 *  Created on: 7 may 2020
 *      Author: Ludo
 */

#include "comp.h"

#include "common.h"
#include "lssgkcu.h"
#include "mano.h"
#include "mapping.h"
#include "sw2.h"

/*** COMP macros ***/

#define COMP_CP_HYSTERESIS_LOW_DECIBARS 		78 // Low threshold of regulation hysteresis.
#define COMP_CP_SOUND_AUTO_OFF_RANGE_DECIBARS	8 // Pressure range within which no off command is sent (sound will stop by itself).
#define COMP_CP_HYSTERESIS_HIGH_DECIBARS 		90 // High threshold of regulation hysteresis.
#define COMP_CP_MAXIMUM_VALUE_DECIBARS	 		95 // Maximum value displayed.

/*** COMP local structures ***/

typedef enum {
	COMP_STATE_OFF,
	COMP_STATE_AUTO_ON,
	COMP_STATE_AUTO_OFF,
	COMP_STATE_DIRECT
} COMP_State;

typedef struct {
	// Inputs.
	SW2_Context comp_zca;
	SW2_Context comp_zcd;
	// State machine.
	COMP_State comp_state;
	unsigned char comp_sound_auto_off; // Set when off command is not required.
} COMP_Context;

/*** COMP local global variables ***/

static COMP_Context comp_ctx;

/*** COMP functions ***/

/* INIT COMPRESSOR MODULE.
 * @param:	None.
 * @return:	None.
 */
void COMP_Init(void) {
	// Init GPIOs.
	SW2_Init(&comp_ctx.comp_zca, &GPIO_BL_ZCA, 1, 100); // ZCA active high.
	SW2_Init(&comp_ctx.comp_zcd, &GPIO_BL_ZCD, 0, 100); // ZCD active low.
	// Init context.
	comp_ctx.comp_state = COMP_STATE_OFF;
	comp_ctx.comp_sound_auto_off = 0;
	// Init global context.
	lsmcu_ctx.lsmcu_compressor_on = 0;
}

/* MAIN TASK OF COMPRESSOR MODULE.
 * @param:	None.
 * @return:	None.
 */
void COMP_Task(void) {
	// Update ZCA.
	if (lsmcu_ctx.lsmcu_dj_locked != 0) {
		SW2_UpdateState(&comp_ctx.comp_zca);
		SW2_UpdateState(&comp_ctx.comp_zcd);
	}
	// Perform state machine.
	switch (comp_ctx.comp_state) {
	case COMP_STATE_OFF:
		// Update global context.
		lsmcu_ctx.lsmcu_compressor_on = 0;
		// Check DJ.
		if (lsmcu_ctx.lsmcu_dj_locked != 0) {
			// ZCD overrides ZCA.
			if (comp_ctx.comp_zcd.sw2_state == SW2_ON) {
				// Play direct sound and set CP needle to maximum.
				LSSGKCU_Send(LSMCU_OUT_COMP_DIRECT_ON);
				MANO_SetTarget(&(lsmcu_ctx.lsmcu_mano_cp), COMP_CP_MAXIMUM_VALUE_DECIBARS);
				MANO_StartNeedle(&(lsmcu_ctx.lsmcu_mano_cp));
				// Compute next state.
				comp_ctx.comp_state = COMP_STATE_DIRECT;

			}
			else {
				if (comp_ctx.comp_zca.sw2_state == SW2_ON) {
					// Check CP value.
					if (MANO_GetPressure(&(lsmcu_ctx.lsmcu_mano_cp)) < COMP_CP_HYSTERESIS_LOW_DECIBARS) {
						// Play accurate sound.
						if (MANO_GetPressure(&(lsmcu_ctx.lsmcu_mano_cp)) > (COMP_CP_HYSTERESIS_LOW_DECIBARS - COMP_CP_SOUND_AUTO_OFF_RANGE_DECIBARS)) {
							// Play minimum regulation sound.
							LSSGKCU_Send(LSMCU_OUT_COMP_AUTO_REG_MIN_ON);
							comp_ctx.comp_sound_auto_off = 1;
						}
						else {
							// Play maximum regulation sound.
							LSSGKCU_Send(LSMCU_OUT_COMP_AUTO_REG_MAX_ON);
							if (MANO_GetPressure(&(lsmcu_ctx.lsmcu_mano_cp)) < COMP_CP_SOUND_AUTO_OFF_RANGE_DECIBARS) {
								comp_ctx.comp_sound_auto_off = 1;
							}
							else {
								comp_ctx.comp_sound_auto_off = 0;
							}
						}
						// Set CP needle to maximum (will be stopped by hysteresis).
						MANO_SetTarget(&(lsmcu_ctx.lsmcu_mano_cp), COMP_CP_MAXIMUM_VALUE_DECIBARS);
						MANO_StartNeedle(&(lsmcu_ctx.lsmcu_mano_cp));
						// Compute next state.
						comp_ctx.comp_state = COMP_STATE_AUTO_ON;
					}
					else {
						// Auto mode in off zone.
						MANO_StopNeedle(&(lsmcu_ctx.lsmcu_mano_cp));
						// Compute next state.
						comp_ctx.comp_state = COMP_STATE_AUTO_OFF;
					}
				}
			}
		}
		break;
	case COMP_STATE_AUTO_ON:
		// Update global context.
		lsmcu_ctx.lsmcu_compressor_on = 1;
		// Check DJ
		if (lsmcu_ctx.lsmcu_dj_locked == 0) {
			// Stop compressor.
			LSSGKCU_Send(LSMCU_OUT_COMP_OFF);
			MANO_StopNeedle(&(lsmcu_ctx.lsmcu_mano_cp));
			// Compute next state.
			comp_ctx.comp_state = COMP_STATE_OFF;
		}
		else {
			// ZCD overrides ZCA.
			if (comp_ctx.comp_zcd.sw2_state == SW2_ON) {
				// Play direct sound and set CP needle to maximum.
				LSSGKCU_Send(LSMCU_OUT_COMP_DIRECT_ON);
				MANO_SetTarget(&(lsmcu_ctx.lsmcu_mano_cp), COMP_CP_MAXIMUM_VALUE_DECIBARS);
				MANO_StartNeedle(&(lsmcu_ctx.lsmcu_mano_cp));
				// Compute next state.
				comp_ctx.comp_state = COMP_STATE_DIRECT;

			}
			else {
				if (comp_ctx.comp_zca.sw2_state == SW2_ON) {
					// Perform hysterersis.
					if (MANO_GetPressure(&(lsmcu_ctx.lsmcu_mano_cp)) > COMP_CP_HYSTERESIS_HIGH_DECIBARS) {
						// Stop regulation.
						if (comp_ctx.comp_sound_auto_off == 0) {
							LSSGKCU_Send(LSMCU_OUT_COMP_OFF);
						}
						MANO_StopNeedle(&(lsmcu_ctx.lsmcu_mano_cp));
						// Compute next state.
						comp_ctx.comp_state = COMP_STATE_AUTO_OFF;
					}
				}
				else {
					// Compressor off.
					LSSGKCU_Send(LSMCU_OUT_COMP_OFF);
					MANO_StopNeedle(&(lsmcu_ctx.lsmcu_mano_cp));
					// Compute next state.
					comp_ctx.comp_state = COMP_STATE_OFF;
				}
			}
		}
		break;
	case COMP_STATE_AUTO_OFF:
		// Update global context.
		lsmcu_ctx.lsmcu_compressor_on = 0;
		// Check DJ
		if (lsmcu_ctx.lsmcu_dj_locked == 0) {
			// Compute next state.
			comp_ctx.comp_state = COMP_STATE_OFF;
		}
		else {
			// ZCD overrides ZCA.
			if (comp_ctx.comp_zcd.sw2_state == SW2_ON) {
				// Play direct sound and set CP needle to maximum.
				LSSGKCU_Send(LSMCU_OUT_COMP_DIRECT_ON);
				MANO_SetTarget(&(lsmcu_ctx.lsmcu_mano_cp), COMP_CP_MAXIMUM_VALUE_DECIBARS);
				MANO_StartNeedle(&(lsmcu_ctx.lsmcu_mano_cp));
				// Compute next state.
				comp_ctx.comp_state = COMP_STATE_DIRECT;

			}
			else {
				if (comp_ctx.comp_zca.sw2_state == SW2_ON) {
					// Check CP value.
					if (MANO_GetPressure(&(lsmcu_ctx.lsmcu_mano_cp)) < COMP_CP_HYSTERESIS_LOW_DECIBARS) {
						// Play accurate sound.
						if (MANO_GetPressure(&(lsmcu_ctx.lsmcu_mano_cp)) > (COMP_CP_HYSTERESIS_LOW_DECIBARS - COMP_CP_SOUND_AUTO_OFF_RANGE_DECIBARS)) {
							// Play minimum regulation sound.
							LSSGKCU_Send(LSMCU_OUT_COMP_AUTO_REG_MIN_ON);
							comp_ctx.comp_sound_auto_off = 1;
						}
						else {
							// Play maximum regulation sound.
							LSSGKCU_Send(LSMCU_OUT_COMP_AUTO_REG_MAX_ON);
							if (MANO_GetPressure(&(lsmcu_ctx.lsmcu_mano_cp)) < COMP_CP_SOUND_AUTO_OFF_RANGE_DECIBARS) {
								comp_ctx.comp_sound_auto_off = 1;
							}
							else {
								comp_ctx.comp_sound_auto_off = 0;
							}
						}
						// Set CP needle to maximum (will be stopped by hysteresis).
						MANO_SetTarget(&(lsmcu_ctx.lsmcu_mano_cp), COMP_CP_MAXIMUM_VALUE_DECIBARS);
						MANO_StartNeedle(&(lsmcu_ctx.lsmcu_mano_cp));
						// Compute next state.
						comp_ctx.comp_state = COMP_STATE_AUTO_ON;
					}
				}
				else {
					// Compute next state.
					comp_ctx.comp_state = COMP_STATE_OFF;
				}
			}
		}
		break;
	case COMP_STATE_DIRECT:
		// Update global context.
		lsmcu_ctx.lsmcu_compressor_on = 1;
		// Check DJ
		if (lsmcu_ctx.lsmcu_dj_locked == 0) {
			// Stop compressor.
			LSSGKCU_Send(LSMCU_OUT_COMP_OFF);
			MANO_StopNeedle(&(lsmcu_ctx.lsmcu_mano_cp));
			// Compute next state.
			comp_ctx.comp_state = COMP_STATE_OFF;
		}
		else {
			// ZCD overrides ZCA.
			if (comp_ctx.comp_zcd.sw2_state == SW2_OFF) {
				// Check ZCA.
				if (comp_ctx.comp_zca.sw2_state == SW2_ON) {
					// Check CP value.
					if (MANO_GetPressure(&(lsmcu_ctx.lsmcu_mano_cp)) < COMP_CP_HYSTERESIS_LOW_DECIBARS) {
						// Play accurate sound.
						if (MANO_GetPressure(&(lsmcu_ctx.lsmcu_mano_cp)) > (COMP_CP_HYSTERESIS_LOW_DECIBARS - COMP_CP_SOUND_AUTO_OFF_RANGE_DECIBARS)) {
							// Play minimum regulation sound.
							LSSGKCU_Send(LSMCU_OUT_COMP_AUTO_REG_MIN_ON);
							comp_ctx.comp_sound_auto_off = 1;
						}
						else {
							// Play maximum regulation sound.
							LSSGKCU_Send(LSMCU_OUT_COMP_AUTO_REG_MAX_ON);
							if (MANO_GetPressure(&(lsmcu_ctx.lsmcu_mano_cp)) < COMP_CP_SOUND_AUTO_OFF_RANGE_DECIBARS) {
								comp_ctx.comp_sound_auto_off = 1;
							}
							else {
								comp_ctx.comp_sound_auto_off = 0;
							}
						}
						// Set CP needle to maximum (will be stopped by hysteresis).
						MANO_SetTarget(&(lsmcu_ctx.lsmcu_mano_cp), COMP_CP_MAXIMUM_VALUE_DECIBARS);
						MANO_StartNeedle(&(lsmcu_ctx.lsmcu_mano_cp));
						// Compute next state.
						comp_ctx.comp_state = COMP_STATE_AUTO_ON;
					}
					else {
						// Auto mode in off zone.
						MANO_StopNeedle(&(lsmcu_ctx.lsmcu_mano_cp));
						// Compute next state.
						comp_ctx.comp_state = COMP_STATE_AUTO_OFF;
					}
				}
				else {
					// Stop compressor.
					LSSGKCU_Send(LSMCU_OUT_COMP_OFF);
					MANO_StopNeedle(&(lsmcu_ctx.lsmcu_mano_cp));
					// Compute next state.
					comp_ctx.comp_state = COMP_STATE_OFF;
				}
			}
		}
		break;
	default:
		// Unknown state.
		comp_ctx.comp_state = COMP_STATE_OFF;
		break;
	}
}
