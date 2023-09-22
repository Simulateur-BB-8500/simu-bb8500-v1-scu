/*
 * compressor.c
 *
 *  Created on: 7 may 2020
 *      Author: Ludo
 */

#include "compressor.h"

#include "lsmcu.h"
#include "lsagiu.h"
#include "manometer.h"
#include "mapping.h"
#include "sw2.h"
#include "stdint.h"

/*** COMPRESSOR macros ***/

#define COMPRESSOR_CP_HYSTERESIS_LOW_MBAR		7800 	// Low threshold of regulation hysteresis.
#define COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_MBAR	800 	// Pressure range within which no off command is sent (sound will stop by itself).
#define COMPRESSOR_CP_HYSTERESIS_HIGH_MBAR 		9000 	// High threshold of regulation hysteresis.
#define COMPRESSOR_CP_MAXIMUM_VALUE_MBAR	 	9500 	// Maximum value displayed.
#define COMPRESSOR_CP_SPEED_MBAR_PER_SECOND		54

/*** COMPRESSOR local structures ***/

typedef enum {
	COMPRESSOR_STATE_OFF,
	COMPRESSOR_STATE_AUTO_ON,
	COMPRESSOR_STATE_AUTO_OFF,
	COMPRESSOR_STATE_DIRECT
} COMPRESSOR_State;

typedef struct {
	// Inputs.
	SW2_context_t zca;
	SW2_context_t zcd;
	// State machine.
	COMPRESSOR_State state;
	uint8_t sound_auto_off; // Set when off command is not required.
} COMPRESSOR_context_t;

/*** COMPRESSOR external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** COMPRESSOR local global variables ***/

static COMPRESSOR_context_t compressor_ctx;

/*** COMPRESSOR functions ***/

/* INIT COMPRESSORRESSOR MODULE.
 * @param:	None.
 * @return:	None.
 */
void COMPRESSOR_init(void) {
	// Init GPIOs.
	SW2_init(&compressor_ctx.zca, &GPIO_BL_ZCA, 1, 100); // ZCA active high.
	SW2_init(&compressor_ctx.zcd, &GPIO_BL_ZCD, 0, 100); // ZCD active low.
	// Init context.
	compressor_ctx.state = COMPRESSOR_STATE_OFF;
	compressor_ctx.sound_auto_off = 0;
	// Init global context.
	lsmcu_ctx.compressor_on = 0;
}

/* MAIN TASK OF COMPRESSOR MODULE.
 * @param:	None.
 * @return:	None.
 */
void COMPRESSOR_task(void) {
	// Update ZCA.
	if (lsmcu_ctx.dj_locked != 0) {
		SW2_update_state(&compressor_ctx.zca);
		SW2_update_state(&compressor_ctx.zcd);
	}
	// Perform state machine.
	switch (compressor_ctx.state) {
	case COMPRESSOR_STATE_OFF:
		// Update global context.
		lsmcu_ctx.compressor_on = 0;
		// Check DJ.
		if (lsmcu_ctx.dj_locked != 0) {
			// ZCD overrides ZCA.
			if (compressor_ctx.zcd.state == SW2_ON) {
				// Play direct sound and set CP needle to maximum.
				LSAGIU_Send(LSMCU_OUT_ZCD_ON);
				MANOMETER_set_pressure(lsmcu_ctx.manometer_cp, COMPRESSOR_CP_MAXIMUM_VALUE_MBAR, COMPRESSOR_CP_SPEED_MBAR_PER_SECOND);
				// Compute next state.
				compressor_ctx.state = COMPRESSOR_STATE_DIRECT;

			}
			else {
				if (compressor_ctx.zca.state == SW2_ON) {
					// Check CP value.
					if (MANOMETER_get_pressure(lsmcu_ctx.manometer_cp) < COMPRESSOR_CP_HYSTERESIS_LOW_MBAR) {
						// Play accurate sound.
						if (MANOMETER_get_pressure(lsmcu_ctx.manometer_cp) > (COMPRESSOR_CP_HYSTERESIS_LOW_MBAR - COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_MBAR)) {
							// Play minimum regulation sound.
							LSAGIU_Send(LSMCU_OUT_ZCA_REGULATION_MIN);
							compressor_ctx.sound_auto_off = 1;
						}
						else {
							// Play maximum regulation sound.
							LSAGIU_Send(LSMCU_OUT_ZCA_REGULATION_MAX);
							if (MANOMETER_get_pressure(lsmcu_ctx.manometer_cp) < COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_MBAR) {
								compressor_ctx.sound_auto_off = 1;
							}
							else {
								compressor_ctx.sound_auto_off = 0;
							}
						}
						// Set CP needle to maximum (will be stopped by hysteresis).
						MANOMETER_set_pressure(lsmcu_ctx.manometer_cp, COMPRESSOR_CP_MAXIMUM_VALUE_MBAR, COMPRESSOR_CP_SPEED_MBAR_PER_SECOND);
						// Compute next state.
						compressor_ctx.state = COMPRESSOR_STATE_AUTO_ON;
					}
					else {
						// Auto mode in off zone.
						MANOMETER_needle_stop(lsmcu_ctx.manometer_cp);
						// Compute next state.
						compressor_ctx.state = COMPRESSOR_STATE_AUTO_OFF;
					}
				}
			}
		}
		break;
	case COMPRESSOR_STATE_AUTO_ON:
		// Update global context.
		lsmcu_ctx.compressor_on = 1;
		// Check DJ
		if (lsmcu_ctx.dj_locked == 0) {
			// Stop compressor.
			LSAGIU_Send(LSMCU_OUT_ZCX_OFF);
			MANOMETER_needle_stop(lsmcu_ctx.manometer_cp);
			// Compute next state.
			compressor_ctx.state = COMPRESSOR_STATE_OFF;
		}
		else {
			// ZCD overrides ZCA.
			if (compressor_ctx.zcd.state == SW2_ON) {
				// Play direct sound and set CP needle to maximum.
				LSAGIU_Send(LSMCU_OUT_ZCD_ON);
				MANOMETER_set_pressure(lsmcu_ctx.manometer_cp, COMPRESSOR_CP_MAXIMUM_VALUE_MBAR, COMPRESSOR_CP_SPEED_MBAR_PER_SECOND);
				// Compute next state.
				compressor_ctx.state = COMPRESSOR_STATE_DIRECT;

			}
			else {
				if (compressor_ctx.zca.state == SW2_ON) {
					// Perform hysteresis.
					if (MANOMETER_get_pressure(lsmcu_ctx.manometer_cp) > COMPRESSOR_CP_HYSTERESIS_HIGH_MBAR) {
						// Stop regulation.
						if (compressor_ctx.sound_auto_off == 0) {
							LSAGIU_Send(LSMCU_OUT_ZCX_OFF);
						}
						MANOMETER_needle_stop(lsmcu_ctx.manometer_cp);
						// Compute next state.
						compressor_ctx.state = COMPRESSOR_STATE_AUTO_OFF;
					}
				}
				else {
					// Compressor off.
					LSAGIU_Send(LSMCU_OUT_ZCX_OFF);
					MANOMETER_needle_stop(lsmcu_ctx.manometer_cp);
					// Compute next state.
					compressor_ctx.state = COMPRESSOR_STATE_OFF;
				}
			}
		}
		break;
	case COMPRESSOR_STATE_AUTO_OFF:
		// Update global context.
		lsmcu_ctx.compressor_on = 0;
		// Check DJ
		if (lsmcu_ctx.dj_locked == 0) {
			// Compute next state.
			compressor_ctx.state = COMPRESSOR_STATE_OFF;
		}
		else {
			// ZCD overrides ZCA.
			if (compressor_ctx.zcd.state == SW2_ON) {
				// Play direct sound and set CP needle to maximum.
				LSAGIU_Send(LSMCU_OUT_ZCD_ON);
				MANOMETER_set_pressure(lsmcu_ctx.manometer_cp, COMPRESSOR_CP_MAXIMUM_VALUE_MBAR, COMPRESSOR_CP_SPEED_MBAR_PER_SECOND);
				// Compute next state.
				compressor_ctx.state = COMPRESSOR_STATE_DIRECT;

			}
			else {
				if (compressor_ctx.zca.state == SW2_ON) {
					// Check CP value.
					if (MANOMETER_get_pressure(lsmcu_ctx.manometer_cp) < COMPRESSOR_CP_HYSTERESIS_LOW_MBAR) {
						// Play accurate sound.
						if (MANOMETER_get_pressure(lsmcu_ctx.manometer_cp) > (COMPRESSOR_CP_HYSTERESIS_LOW_MBAR - COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_MBAR)) {
							// Play minimum regulation sound.
							LSAGIU_Send(LSMCU_OUT_ZCA_REGULATION_MIN);
							compressor_ctx.sound_auto_off = 1;
						}
						else {
							// Play maximum regulation sound.
							LSAGIU_Send(LSMCU_OUT_ZCA_REGULATION_MAX);
							if (MANOMETER_get_pressure(lsmcu_ctx.manometer_cp) < COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_MBAR) {
								compressor_ctx.sound_auto_off = 1;
							}
							else {
								compressor_ctx.sound_auto_off = 0;
							}
						}
						// Set CP needle to maximum (will be stopped by hysteresis).
						MANOMETER_set_pressure(lsmcu_ctx.manometer_cp, COMPRESSOR_CP_MAXIMUM_VALUE_MBAR, COMPRESSOR_CP_SPEED_MBAR_PER_SECOND);
						// Compute next state.
						compressor_ctx.state = COMPRESSOR_STATE_AUTO_ON;
					}
				}
				else {
					// Compute next state.
					compressor_ctx.state = COMPRESSOR_STATE_OFF;
				}
			}
		}
		break;
	case COMPRESSOR_STATE_DIRECT:
		// Update global context.
		lsmcu_ctx.compressor_on = 1;
		// Check DJ
		if (lsmcu_ctx.dj_locked == 0) {
			// Stop compressor.
			LSAGIU_Send(LSMCU_OUT_ZCX_OFF);
			MANOMETER_needle_stop(lsmcu_ctx.manometer_cp);
			// Compute next state.
			compressor_ctx.state = COMPRESSOR_STATE_OFF;
		}
		else {
			// ZCD overrides ZCA.
			if (compressor_ctx.zcd.state == SW2_OFF) {
				// Check ZCA.
				if (compressor_ctx.zca.state == SW2_ON) {
					// Check CP value.
					if (MANOMETER_get_pressure(lsmcu_ctx.manometer_cp) < COMPRESSOR_CP_HYSTERESIS_LOW_MBAR) {
						// Play accurate sound.
						if (MANOMETER_get_pressure(lsmcu_ctx.manometer_cp) > (COMPRESSOR_CP_HYSTERESIS_LOW_MBAR - COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_MBAR)) {
							// Play minimum regulation sound.
							LSAGIU_Send(LSMCU_OUT_ZCA_REGULATION_MIN);
							compressor_ctx.sound_auto_off = 1;
						}
						else {
							// Play maximum regulation sound.
							LSAGIU_Send(LSMCU_OUT_ZCA_REGULATION_MAX);
							if (MANOMETER_get_pressure(lsmcu_ctx.manometer_cp) < COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_MBAR) {
								compressor_ctx.sound_auto_off = 1;
							}
							else {
								compressor_ctx.sound_auto_off = 0;
							}
						}
						// Set CP needle to maximum (will be stopped by hysteresis).
						MANOMETER_set_pressure(lsmcu_ctx.manometer_cp, COMPRESSOR_CP_MAXIMUM_VALUE_MBAR, COMPRESSOR_CP_SPEED_MBAR_PER_SECOND);
						// Compute next state.
						compressor_ctx.state = COMPRESSOR_STATE_AUTO_ON;
					}
					else {
						// Auto mode in off zone.
						MANOMETER_needle_stop(lsmcu_ctx.manometer_cp);
						// Compute next state.
						compressor_ctx.state = COMPRESSOR_STATE_AUTO_OFF;
					}
				}
				else {
					// Stop compressor.
					LSAGIU_Send(LSMCU_OUT_ZCX_OFF);
					MANOMETER_needle_stop(lsmcu_ctx.manometer_cp);
					// Compute next state.
					compressor_ctx.state = COMPRESSOR_STATE_OFF;
				}
			}
		}
		break;
	default:
		// Unknown state.
		compressor_ctx.state = COMPRESSOR_STATE_OFF;
		break;
	}
}
