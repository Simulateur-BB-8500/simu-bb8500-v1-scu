/*
 * pictograms.c
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#include "pictograms.h"

#include "gpio.h"
#include "lsmcu.h"
#include "mapping.h"
#include "tim.h"

/*** PICTOGRAMS local macros ***/

#define PICTOGRAMS_ZBA_CLOSED_LSGR_DELAY_MS				2000
#define PICTOGRAMS_ZBA_CLOSED_LSBA_BLINK_DURATION_MS	200
#define PICTOGRAMS_DJ_LOCKED_LSCB_DELAY_MS				1000
#define PICTOGRAMS_DJ_LOCKED_LSDJ_DELAY_MS				200
#define PICTOGRAMS_ZDJ_LOCKING_DURATION_MS				1000
#define PICTOGRAMS_ZDJ_LOCKING_LSDJ_DELAY_MS			200
#define PICTOGRAMS_LSRH_BLINK_DURATION_MS				200

/*** PICTOGRAMS local structures ***/

// PICTOGRAMS bit index.
typedef enum {
	PICTOGRAMS_LSDJ_BIT_INDEX = 0,
	PICTOGRAMS_LSGR_BIT_INDEX,
	PICTOGRAMS_LSS_BIT_INDEX,
	PICTOGRAMS_LSCB_BIT_INDEX,
	PICTOGRAMS_LSP_BIT_INDEX,
	PICTOGRAMS_LSPAT_BIT_INDEX,
	PICTOGRAMS_LSBA_BIT_INDEX,
	PICTOGRAMS_LSPI_BIT_INDEX,
	PICTOGRAMS_LSRH_BIT_INDEX,
} PICTOGRAMS_BitIndex;

// PICTOGRAMS internal state machine.
typedef enum {
	PICTOGRAMS_STATE_OFF,
	PICTOGRAMS_STATE_ZBA_CLOSED_TRANSITION1,
	PICTOGRAMS_STATE_ZBA_CLOSED_TRANSITION2,
	PICTOGRAMS_STATE_ZBA_CLOSED,
	PICTOGRAMS_STATE_BL_UNLOCKED,
	PICTOGRAMS_STATE_DJ_CLOSED,
	PICTOGRAMS_STATE_DJ_LOCKED_TRANSITION1,
	PICTOGRAMS_STATE_DJ_LOCKED_TRANSITION2,
	PICTOGRAMS_STATE_DJ_LOCKED
} PICTOGRAMS_State;

typedef struct {
	PICTOGRAMS_State pictograms_state;
	unsigned long pictograms_switch_state_time; // In ms.
	unsigned long pictograms_lsrh_blink_start_time; // In ms.
} PICTOGRAMS_Context;

/*** PICTOGRAMS external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** PICTOGRAMS local global variables ***/

static PICTOGRAMS_Context pictograms_ctx;

/*** PICTOGRAMS local functions ***/

/* SET THE STATE OF ALL LIGHTS.
 * @param pictograms_state_mask:	Lights mask defined as [LSRH|LSPI|LSBA|LSPAT|LSP|LSCB|LSS|LSGR|LSDJ].
 * @return:					None.
 */
static void PICTOGRAMS_SetState(unsigned int pictograms_state_mask) {
	// Set all lights state.
	GPIO_Write(&GPIO_LSDJ, (pictograms_state_mask & (0b1 << PICTOGRAMS_LSDJ_BIT_INDEX)));
	GPIO_Write(&GPIO_LSGR, (pictograms_state_mask & (0b1 << PICTOGRAMS_LSGR_BIT_INDEX)));
	GPIO_Write(&GPIO_LSS, (pictograms_state_mask & (0b1 << PICTOGRAMS_LSS_BIT_INDEX)));
	GPIO_Write(&GPIO_LSCB, (pictograms_state_mask & (0b1 << PICTOGRAMS_LSCB_BIT_INDEX)));
	GPIO_Write(&GPIO_LSP, (pictograms_state_mask & (0b1 << PICTOGRAMS_LSP_BIT_INDEX)));
	GPIO_Write(&GPIO_LSPAT, (pictograms_state_mask & (0b1 << PICTOGRAMS_LSPAT_BIT_INDEX)));
	GPIO_Write(&GPIO_LSBA, (pictograms_state_mask & (0b1 << PICTOGRAMS_LSBA_BIT_INDEX)));
	GPIO_Write(&GPIO_LSPI, (pictograms_state_mask & (0b1 << PICTOGRAMS_LSPI_BIT_INDEX)));
	GPIO_Write(&GPIO_LSRH, (pictograms_state_mask & (0b1 << PICTOGRAMS_LSRH_BIT_INDEX)));
}

/*** PICTOGRAMS functions ***/

/* INIT PICTOGRAMS MODULE.
 * @param:	None.
 * @return:	None.
 */
void PICTOGRAMS_Init(void) {
	// Init GPIOs.
	GPIO_Configure(&GPIO_LSDJ, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_LSGR, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_LSS, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_LSCB, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_LSP, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_LSPAT, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_LSBA, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_LSPI, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_LSRH, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	PICTOGRAMS_SetState(0);
	// Init context.
	pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_OFF;
	// Init global context.
	lsmcu_ctx.lsmcu_lsrh_blink_request = 0;
}

/* MAIN TASK OF PICTOGRAMS MODULE.
 * @param:	None.
 * @return:	None.
 */
void PICTOGRAMS_Task(void) {
	// Perform state machine.
	switch (pictograms_ctx.pictograms_state) {
	case PICTOGRAMS_STATE_OFF:
		// Check ZBA.
		if (lsmcu_ctx.lsmcu_zba_closed != 0) {
			// Turn LSDJ on.
			PICTOGRAMS_SetState(0b000000001);
			// Compute next state.
			pictograms_ctx.pictograms_switch_state_time = TIM2_GetMs();
			pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_ZBA_CLOSED_TRANSITION1;
		}
		break;
	case PICTOGRAMS_STATE_ZBA_CLOSED_TRANSITION1:
		if (lsmcu_ctx.lsmcu_zba_closed == 0) {
			// Turn all lights off.
			PICTOGRAMS_SetState(0);
			pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_OFF;
		}
		else {
			if (TIM2_GetMs() > (pictograms_ctx.pictograms_switch_state_time + PICTOGRAMS_ZBA_CLOSED_LSGR_DELAY_MS)) {
				// Turn LSGR and LSBA on.
				PICTOGRAMS_SetState(0b001000011);
				// Compute next state.
				pictograms_ctx.pictograms_switch_state_time = TIM2_GetMs();
				pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_ZBA_CLOSED_TRANSITION2;
			}
		}
		break;
	case PICTOGRAMS_STATE_ZBA_CLOSED_TRANSITION2:
		if (lsmcu_ctx.lsmcu_zba_closed == 0) {
			// Turn all lights off.
			PICTOGRAMS_SetState(0);
			pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_OFF;
		}
		else {
			if (TIM2_GetMs() > (pictograms_ctx.pictograms_switch_state_time + PICTOGRAMS_ZBA_CLOSED_LSBA_BLINK_DURATION_MS)) {
				// Turn LSBA off.
				PICTOGRAMS_SetState(0b000000011);
				// Compute next state.
				pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_ZBA_CLOSED;
			}
		}
		break;
	case PICTOGRAMS_STATE_ZBA_CLOSED:
		if (lsmcu_ctx.lsmcu_zba_closed == 0) {
			// Turn all lights off.
			PICTOGRAMS_SetState(0);
			pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_OFF;
		}
		else {
			if (lsmcu_ctx.lsmcu_bl_unlocked != 0) {
				// Turn all missing lights on.
				PICTOGRAMS_SetState(0b111111111);
				// Compute next state.
				pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_BL_UNLOCKED;
			}
		}
		break;
	case PICTOGRAMS_STATE_BL_UNLOCKED:
		if (lsmcu_ctx.lsmcu_zba_closed == 0) {
			// Turn all lights off.
			PICTOGRAMS_SetState(0);
			pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_OFF;
		}
		else {
			if (lsmcu_ctx.lsmcu_bl_unlocked == 0) {
				// Turn all new lights off.
				PICTOGRAMS_SetState(0b000000011);
				// Come back to previous state.
				pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_ZBA_CLOSED;
			}
			else {
				if (lsmcu_ctx.lsmcu_dj_closed != 0) {
					// Turn LSS, LSP, LSPAT, LSBA, LSPI and LSRH off.
					PICTOGRAMS_SetState(0b000101011);
					// Compute next state.
					pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_DJ_CLOSED;
				}
			}
		}
		break;
	case PICTOGRAMS_STATE_DJ_CLOSED:
		if (lsmcu_ctx.lsmcu_zba_closed == 0) {
			// Turn all lights off.
			PICTOGRAMS_SetState(0);
			pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_OFF;
		}
		else {
			if (lsmcu_ctx.lsmcu_dj_closed == 0) {
				// Come back to previous state.
				PICTOGRAMS_SetState(0b111111111);
				// Compute next state.
				pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_BL_UNLOCKED;
			}
			else {
				if (lsmcu_ctx.lsmcu_dj_locked != 0) {
					// Compute next state.
					pictograms_ctx.pictograms_switch_state_time = TIM2_GetMs();
					pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_DJ_LOCKED_TRANSITION1;
				}
			}
		}
		break;
	case PICTOGRAMS_STATE_DJ_LOCKED_TRANSITION1:
		// Wait for locking operation.
		if (TIM2_GetMs() > (pictograms_ctx.pictograms_switch_state_time + PICTOGRAMS_ZDJ_LOCKING_DURATION_MS)) {
			// Turn LSPAT and LSCB off.
			PICTOGRAMS_SetState(0b000000011);
			// Compute next state.
			pictograms_ctx.pictograms_switch_state_time = TIM2_GetMs();
			pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_DJ_LOCKED_TRANSITION2;
		}
		break;
	case PICTOGRAMS_STATE_DJ_LOCKED_TRANSITION2:
		// Wait for LSDJ delay.
		if (TIM2_GetMs() > (pictograms_ctx.pictograms_switch_state_time + PICTOGRAMS_ZDJ_LOCKING_LSDJ_DELAY_MS)) {
			// Turn LSDJ off.
			PICTOGRAMS_SetState(0b000000010);
			// Compute next state.
			pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_DJ_LOCKED;
		}
		break;
	case PICTOGRAMS_STATE_DJ_LOCKED:
		// Manage LSGR, LSS and LSP.
		GPIO_Write(&GPIO_LSGR, (lsmcu_ctx.lsmcu_rheostat_0 != 0));
		GPIO_Write(&GPIO_LSS, ((lsmcu_ctx.lsmcu_rheostat_0 == 0) && (lsmcu_ctx.lsmcu_series_traction != 0)));
		GPIO_Write(&GPIO_LSP, ((lsmcu_ctx.lsmcu_rheostat_0 == 0) && (lsmcu_ctx.lsmcu_series_traction == 0)));
		// Manage LSRH.
		if (lsmcu_ctx.lsmcu_rheostat_0 == 0) {
			if ((lsmcu_ctx.lsmcu_lsrh_blink_request != 0) && (GPIO_Read(&GPIO_LSRH) != 0)) {
				GPIO_Write(&GPIO_LSRH, 0);
				pictograms_ctx.pictograms_lsrh_blink_start_time = TIM2_GetMs();
			}
			else {
				if (TIM2_GetMs() > (pictograms_ctx.pictograms_lsrh_blink_start_time + PICTOGRAMS_LSRH_BLINK_DURATION_MS)) {
					// End blink.
					GPIO_Write(&GPIO_LSRH, 1);
					// Reset flag.
					lsmcu_ctx.lsmcu_lsrh_blink_request = 0;
				}
			}
		}
		else {
			GPIO_Write(&GPIO_LSRH, 0);
		}
		// Check ZBA.
		if (lsmcu_ctx.lsmcu_zba_closed == 0) {
			// Turn all lights off.
			PICTOGRAMS_SetState(0);
			pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_OFF;
		}
		else {
			// Check DJ.
			if (lsmcu_ctx.lsmcu_dj_closed == 0) {
				// Come back to previous state.
				PICTOGRAMS_SetState(0b111111111);
				// Compute next state.
				pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_BL_UNLOCKED;
			}
		}
		break;
	default:
		// Turn all lights off.
		pictograms_ctx.pictograms_state = PICTOGRAMS_STATE_OFF;
	}
}
