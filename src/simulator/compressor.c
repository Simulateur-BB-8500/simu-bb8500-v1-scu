/*
 * compressor.c
 *
 *  Created on: 7 may 2020
 *      Author: Ludo
 */

#include "compressor.h"

#include "manometer.h"
#include "mapping.h"
#include "scu.h"
#include "sgdu.h"
#include "sw2.h"
#include "stdint.h"

/*** COMPRESSOR macros ***/

#define COMPRESSOR_CP_HYSTERESIS_LOW_MBAR			7800 	// Low threshold of regulation hysteresis.
#define COMPRESSOR_CP_HYSTERESIS_HIGH_MBAR 			9000 	// High threshold of regulation hysteresis.
#define COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_MBAR		800 	// Pressure range within which no off command is sent (sound will stop by itself).

#define COMPRESSOR_CP_SPEED_UP_MBAR_PER_SECOND		54
#define COMPRESSOR_CP_SPEED_DOWN_MBAR_PER_SECOND	150

/*** COMPRESSOR local structures ***/

/*******************************************************************/
typedef enum {
	COMPRESSOR_STATE_OFF,
	COMPRESSOR_STATE_AUTO_ON,
	COMPRESSOR_STATE_AUTO_OFF,
	COMPRESSOR_STATE_DIRECT
} COMPRESSOR_state_t;

/*******************************************************************/
typedef struct {
	// Inputs.
	SW2_context_t zca;
	SW2_context_t zcd;
	// State machine.
	COMPRESSOR_state_t state;
	uint8_t sound_auto_off; // Set when off command is not required.
} COMPRESSOR_context_t;

/*** COMPRESSOR external global variables ***/

extern SCU_context_t scu_ctx;

/*** COMPRESSOR local global variables ***/

static COMPRESSOR_context_t compressor_ctx;

/*** COMPRESSOR functions ***/

/*******************************************************************/
void COMPRESSOR_init(void) {
	// Init GPIOs.
	SW2_init(&compressor_ctx.zca, &GPIO_BL_ZCA, 1, 100); // ZCA active high.
	SW2_init(&compressor_ctx.zcd, &GPIO_BL_ZCD, 0, 100); // ZCD active low.
	// Init context.
	compressor_ctx.state = COMPRESSOR_STATE_OFF;
	compressor_ctx.sound_auto_off = 0;
}

/*******************************************************************/
void COMPRESSOR_process(void) {
	// Update switches.
	SW2_update_state(&compressor_ctx.zca);
	SW2_update_state(&compressor_ctx.zcd);
	// Perform state machine.
	switch (compressor_ctx.state) {
	case COMPRESSOR_STATE_OFF:
		// Check DJ.
		if (scu_ctx.dj_locked != 0) {
			// ZCD overrides ZCA.
			if (compressor_ctx.zcd.state == SW2_ON) {
				// Play direct sound and set CP needle to maximum.
				SGDU_write(SCU_OUT_ZCD_ON);
				MANOMETER_set_pressure(scu_ctx.manometer_cp, ((scu_ctx.manometer_cp) -> pressure_limit_mbar), COMPRESSOR_CP_SPEED_UP_MBAR_PER_SECOND);
				// Compute next state.
				compressor_ctx.state = COMPRESSOR_STATE_DIRECT;
			}
			else {
				if (compressor_ctx.zca.state == SW2_ON) {
					// Check CP value.
					if (MANOMETER_get_pressure(scu_ctx.manometer_cp) < COMPRESSOR_CP_HYSTERESIS_LOW_MBAR) {
						// Play accurate sound.
						if (MANOMETER_get_pressure(scu_ctx.manometer_cp) > (COMPRESSOR_CP_HYSTERESIS_LOW_MBAR - COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_MBAR)) {
							// Play minimum regulation sound.
							SGDU_write(SCU_OUT_ZCA_REGULATION_MIN);
							compressor_ctx.sound_auto_off = 1;
						}
						else {
							// Play maximum regulation sound.
							SGDU_write(SCU_OUT_ZCA_REGULATION_MAX);
							if (MANOMETER_get_pressure(scu_ctx.manometer_cp) < COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_MBAR) {
								compressor_ctx.sound_auto_off = 1;
							}
							else {
								compressor_ctx.sound_auto_off = 0;
							}
						}
						// Set CP needle to maximum (will be stopped by hysteresis).
						MANOMETER_set_pressure(scu_ctx.manometer_cp, ((scu_ctx.manometer_cp) -> pressure_limit_mbar), COMPRESSOR_CP_SPEED_UP_MBAR_PER_SECOND);
						// Compute next state.
						compressor_ctx.state = COMPRESSOR_STATE_AUTO_ON;
					}
					else {
						// Auto mode in off zone.
						MANOMETER_needle_stop(scu_ctx.manometer_cp);
						// Compute next state.
						compressor_ctx.state = COMPRESSOR_STATE_AUTO_OFF;
					}
				}
			}
		}
		break;
	case COMPRESSOR_STATE_AUTO_ON:
		// Check DJ
		if (scu_ctx.dj_locked == 0) {
			// Stop compressor.
			SGDU_write(SCU_OUT_ZCX_OFF);
			MANOMETER_needle_stop(scu_ctx.manometer_cp);
			// Compute next state.
			compressor_ctx.state = COMPRESSOR_STATE_OFF;
		}
		else {
			// ZCD overrides ZCA.
			if (compressor_ctx.zcd.state == SW2_ON) {
				// Play direct sound and set CP needle to maximum.
				SGDU_write(SCU_OUT_ZCD_ON);
				MANOMETER_set_pressure(scu_ctx.manometer_cp, ((scu_ctx.manometer_cp) -> pressure_limit_mbar), COMPRESSOR_CP_SPEED_UP_MBAR_PER_SECOND);
				// Compute next state.
				compressor_ctx.state = COMPRESSOR_STATE_DIRECT;
			}
			else {
				if (compressor_ctx.zca.state == SW2_ON) {
					// Perform hysteresis.
					if (MANOMETER_get_pressure(scu_ctx.manometer_cp) > COMPRESSOR_CP_HYSTERESIS_HIGH_MBAR) {
						// Stop regulation.
						if (compressor_ctx.sound_auto_off == 0) {
							SGDU_write(SCU_OUT_ZCX_OFF);
						}
						MANOMETER_needle_stop(scu_ctx.manometer_cp);
						// Compute next state.
						compressor_ctx.state = COMPRESSOR_STATE_AUTO_OFF;
					}
				}
				else {
					// Compressor off.
					SGDU_write(SCU_OUT_ZCX_OFF);
					MANOMETER_needle_stop(scu_ctx.manometer_cp);
					// Compute next state.
					compressor_ctx.state = COMPRESSOR_STATE_OFF;
				}
			}
		}
		break;
	case COMPRESSOR_STATE_AUTO_OFF:
		// Check DJ
		if (scu_ctx.dj_locked == 0) {
			// Compute next state.
			compressor_ctx.state = COMPRESSOR_STATE_OFF;
		}
		else {
			// ZCD overrides ZCA.
			if (compressor_ctx.zcd.state == SW2_ON) {
				// Play direct sound and set CP needle to maximum.
				SGDU_write(SCU_OUT_ZCD_ON);
				MANOMETER_set_pressure(scu_ctx.manometer_cp, ((scu_ctx.manometer_cp) -> pressure_limit_mbar), COMPRESSOR_CP_SPEED_UP_MBAR_PER_SECOND);
				// Compute next state.
				compressor_ctx.state = COMPRESSOR_STATE_DIRECT;

			}
			else {
				if (compressor_ctx.zca.state == SW2_ON) {
					// Check CP value.
					if (MANOMETER_get_pressure(scu_ctx.manometer_cp) < COMPRESSOR_CP_HYSTERESIS_LOW_MBAR) {
						// Play accurate sound.
						if (MANOMETER_get_pressure(scu_ctx.manometer_cp) > (COMPRESSOR_CP_HYSTERESIS_LOW_MBAR - COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_MBAR)) {
							// Play minimum regulation sound.
							SGDU_write(SCU_OUT_ZCA_REGULATION_MIN);
							compressor_ctx.sound_auto_off = 1;
						}
						else {
							// Play maximum regulation sound.
							SGDU_write(SCU_OUT_ZCA_REGULATION_MAX);
							if (MANOMETER_get_pressure(scu_ctx.manometer_cp) < COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_MBAR) {
								compressor_ctx.sound_auto_off = 1;
							}
							else {
								compressor_ctx.sound_auto_off = 0;
							}
						}
						// Set CP needle to maximum (will be stopped by hysteresis).
						MANOMETER_set_pressure(scu_ctx.manometer_cp, ((scu_ctx.manometer_cp) -> pressure_limit_mbar), COMPRESSOR_CP_SPEED_UP_MBAR_PER_SECOND);
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
		// Check DJ
		if (scu_ctx.dj_locked == 0) {
			// Stop compressor.
			SGDU_write(SCU_OUT_ZCX_OFF);
			MANOMETER_needle_stop(scu_ctx.manometer_cp);
			// Compute next state.
			compressor_ctx.state = COMPRESSOR_STATE_OFF;
		}
		else {
			// ZCD overrides ZCA.
			if (compressor_ctx.zcd.state == SW2_OFF) {
				// Check ZCA.
				if (compressor_ctx.zca.state == SW2_ON) {
					// Check CP value.
					if (MANOMETER_get_pressure(scu_ctx.manometer_cp) < COMPRESSOR_CP_HYSTERESIS_LOW_MBAR) {
						// Play accurate sound.
						if (MANOMETER_get_pressure(scu_ctx.manometer_cp) > (COMPRESSOR_CP_HYSTERESIS_LOW_MBAR - COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_MBAR)) {
							// Play minimum regulation sound.
							SGDU_write(SCU_OUT_ZCA_REGULATION_MIN);
							compressor_ctx.sound_auto_off = 1;
						}
						else {
							// Play maximum regulation sound.
							SGDU_write(SCU_OUT_ZCA_REGULATION_MAX);
							if (MANOMETER_get_pressure(scu_ctx.manometer_cp) < COMPRESSOR_CP_SOUND_AUTO_OFF_RANGE_MBAR) {
								compressor_ctx.sound_auto_off = 1;
							}
							else {
								compressor_ctx.sound_auto_off = 0;
							}
						}
						// Set CP needle to maximum (will be stopped by hysteresis).
						MANOMETER_set_pressure(scu_ctx.manometer_cp, ((scu_ctx.manometer_cp) -> pressure_limit_mbar), COMPRESSOR_CP_SPEED_UP_MBAR_PER_SECOND);
						// Compute next state.
						compressor_ctx.state = COMPRESSOR_STATE_AUTO_ON;
					}
					else {
						// Auto mode in off zone.
						MANOMETER_needle_stop(scu_ctx.manometer_cp);
						// Compute next state.
						compressor_ctx.state = COMPRESSOR_STATE_AUTO_OFF;
					}
				}
				else {
					// Stop compressor.
					SGDU_write(SCU_OUT_ZCX_OFF);
					MANOMETER_needle_stop(scu_ctx.manometer_cp);
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
	// Manage CP when compressor is inactive.
	if ((compressor_ctx.state == COMPRESSOR_STATE_OFF) || (compressor_ctx.state == COMPRESSOR_STATE_AUTO_OFF)) {
		// Check all manometers.
		if ((MANOMETER_is_pressure_increasing(scu_ctx.manometer_re))  ||
			(MANOMETER_is_pressure_increasing(scu_ctx.manometer_cg))  ||
			(MANOMETER_is_pressure_increasing(scu_ctx.manometer_cf1)) ||
			(MANOMETER_is_pressure_increasing(scu_ctx.manometer_cf2))) {
			// Empty CP if pressure is increasing in any other air circuit.
			MANOMETER_set_pressure(scu_ctx.manometer_cp, 0, COMPRESSOR_CP_SPEED_DOWN_MBAR_PER_SECOND);

		}
		else {
			// Stabilize CP needle.
			MANOMETER_needle_stop(scu_ctx.manometer_cp);
		}
	}
}
