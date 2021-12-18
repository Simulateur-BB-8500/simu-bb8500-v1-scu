/*
 * il.c
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#include "il.h"

#include "gpio.h"
#include "lsmcu.h"
#include "mapping.h"
#include "tim.h"

/*** IL local macros ***/

#define IL_ZBA_CLOSED_LSGR_DELAY_MS				2000
#define IL_ZBA_CLOSED_LSBA_BLINK_DURATION_MS	200
#define IL_DJ_LOCKED_LSCB_DELAY_MS				1000
#define IL_DJ_LOCKED_LSDJ_DELAY_MS				200
#define IL_ZDJ_LOCKING_DURATION_MS				1000
#define IL_ZDJ_LOCKING_LSDJ_DELAY_MS			200
#define IL_LSRH_BLINK_DURATION_MS				200

/*** IL local structures ***/

// IL bit index.
typedef enum {
	IL_LSDJ_BIT_INDEX = 0,
	IL_LSGR_BIT_INDEX,
	IL_LSS_BIT_INDEX,
	IL_LSCB_BIT_INDEX,
	IL_LSP_BIT_INDEX,
	IL_LSPAT_BIT_INDEX,
	IL_LSBA_BIT_INDEX,
	IL_LSPI_BIT_INDEX,
	IL_LSRH_BIT_INDEX,
} IL_BitIndex;

// IL internal state machine.
typedef enum {
	IL_STATE_OFF,
	IL_STATE_ZBA_CLOSED_TRANSITION1,
	IL_STATE_ZBA_CLOSED_TRANSITION2,
	IL_STATE_ZBA_CLOSED,
	IL_STATE_BL_UNLOCKED,
	IL_STATE_DJ_CLOSED,
	IL_STATE_DJ_LOCKED_TRANSITION1,
	IL_STATE_DJ_LOCKED_TRANSITION2,
	IL_STATE_DJ_LOCKED
} IL_State;

typedef struct {
	IL_State il_state;
	unsigned long il_switch_state_time; // In ms.
	unsigned long il_lsrh_blink_start_time; // In ms.
} IL_Context;

/*** IL external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** IL local global variables ***/

static IL_Context il_ctx;

/*** IL local functions ***/

/* SET THE STATE OF ALL LIGHTS.
 * @param il_state_mask:	Lights mask defined as [LSRH|LSPI|LSBA|LSPAT|LSP|LSCB|LSS|LSGR|LSDJ].
 * @return:					None.
 */
void IL_SetState(unsigned int il_state_mask) {
	// Set all lights state.
	GPIO_Write(&GPIO_LSDJ, (il_state_mask & (0b1 << IL_LSDJ_BIT_INDEX)));
	GPIO_Write(&GPIO_LSGR, (il_state_mask & (0b1 << IL_LSGR_BIT_INDEX)));
	GPIO_Write(&GPIO_LSS, (il_state_mask & (0b1 << IL_LSS_BIT_INDEX)));
	GPIO_Write(&GPIO_LSCB, (il_state_mask & (0b1 << IL_LSCB_BIT_INDEX)));
	GPIO_Write(&GPIO_LSP, (il_state_mask & (0b1 << IL_LSP_BIT_INDEX)));
	GPIO_Write(&GPIO_LSPAT, (il_state_mask & (0b1 << IL_LSPAT_BIT_INDEX)));
	GPIO_Write(&GPIO_LSBA, (il_state_mask & (0b1 << IL_LSBA_BIT_INDEX)));
	GPIO_Write(&GPIO_LSPI, (il_state_mask & (0b1 << IL_LSPI_BIT_INDEX)));
	GPIO_Write(&GPIO_LSRH, (il_state_mask & (0b1 << IL_LSRH_BIT_INDEX)));
}

/*** IL functions ***/

/* INIT IL MODULE.
 * @param:	None.
 * @return:	None.
 */
void IL_Init(void) {
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
	IL_SetState(0);
	// Init context.
	il_ctx.il_state = IL_STATE_OFF;
	// Init global context.
	lsmcu_ctx.lsmcu_lsrh_blink_request = 0;
}

/* MAIN TASK OF IL MODULE.
 * @param:	None.
 * @return:	None.
 */
void IL_Task(void) {
	// Perform state machine.
	switch (il_ctx.il_state) {
	case IL_STATE_OFF:
		// Check ZBA.
		if (lsmcu_ctx.lsmcu_zba_closed != 0) {
			// Turn LSDJ on.
			IL_SetState(0b000000001);
			// Compute next state.
			il_ctx.il_switch_state_time = TIM2_GetMs();
			il_ctx.il_state = IL_STATE_ZBA_CLOSED_TRANSITION1;
		}
		break;
	case IL_STATE_ZBA_CLOSED_TRANSITION1:
		if (lsmcu_ctx.lsmcu_zba_closed == 0) {
			// Turn all lights off.
			IL_SetState(0);
			il_ctx.il_state = IL_STATE_OFF;
		}
		else {
			if (TIM2_GetMs() > (il_ctx.il_switch_state_time + IL_ZBA_CLOSED_LSGR_DELAY_MS)) {
				// Turn LSGR and LSBA on.
				IL_SetState(0b001000011);
				// Compute next state.
				il_ctx.il_switch_state_time = TIM2_GetMs();
				il_ctx.il_state = IL_STATE_ZBA_CLOSED_TRANSITION2;
			}
		}
		break;
	case IL_STATE_ZBA_CLOSED_TRANSITION2:
		if (lsmcu_ctx.lsmcu_zba_closed == 0) {
			// Turn all lights off.
			IL_SetState(0);
			il_ctx.il_state = IL_STATE_OFF;
		}
		else {
			if (TIM2_GetMs() > (il_ctx.il_switch_state_time + IL_ZBA_CLOSED_LSBA_BLINK_DURATION_MS)) {
				// Turn LSBA off.
				IL_SetState(0b000000011);
				// Compute next state.
				il_ctx.il_state = IL_STATE_ZBA_CLOSED;
			}
		}
		break;
	case IL_STATE_ZBA_CLOSED:
		if (lsmcu_ctx.lsmcu_zba_closed == 0) {
			// Turn all lights off.
			IL_SetState(0);
			il_ctx.il_state = IL_STATE_OFF;
		}
		else {
			if (lsmcu_ctx.lsmcu_bl_unlocked != 0) {
				// Turn all missing lights on.
				IL_SetState(0b111111111);
				// Compute next state.
				il_ctx.il_state = IL_STATE_BL_UNLOCKED;
			}
		}
		break;
	case IL_STATE_BL_UNLOCKED:
		if (lsmcu_ctx.lsmcu_zba_closed == 0) {
			// Turn all lights off.
			IL_SetState(0);
			il_ctx.il_state = IL_STATE_OFF;
		}
		else {
			if (lsmcu_ctx.lsmcu_bl_unlocked == 0) {
				// Turn all new lights off.
				IL_SetState(0b000000011);
				// Come back to previous state.
				il_ctx.il_state = IL_STATE_ZBA_CLOSED;
			}
			else {
				if (lsmcu_ctx.lsmcu_dj_closed != 0) {
					// Turn LSS, LSP, LSPAT, LSBA, LSPI and LSRH off.
					IL_SetState(0b000101011);
					// Compute next state.
					il_ctx.il_state = IL_STATE_DJ_CLOSED;
				}
			}
		}
		break;
	case IL_STATE_DJ_CLOSED:
		if (lsmcu_ctx.lsmcu_zba_closed == 0) {
			// Turn all lights off.
			IL_SetState(0);
			il_ctx.il_state = IL_STATE_OFF;
		}
		else {
			if (lsmcu_ctx.lsmcu_dj_closed == 0) {
				// Come back to previous state.
				IL_SetState(0b111111111);
				// Compute next state.
				il_ctx.il_state = IL_STATE_BL_UNLOCKED;
			}
			else {
				if (lsmcu_ctx.lsmcu_dj_locked != 0) {
					// Compute next state.
					il_ctx.il_switch_state_time = TIM2_GetMs();
					il_ctx.il_state = IL_STATE_DJ_LOCKED_TRANSITION1;
				}
			}
		}
		break;
	case IL_STATE_DJ_LOCKED_TRANSITION1:
		// Wait for locking operation.
		if (TIM2_GetMs() > (il_ctx.il_switch_state_time + IL_ZDJ_LOCKING_DURATION_MS)) {
			// Turn LSPAT and LSCB off.
			IL_SetState(0b000000011);
			// Compute next state.
			il_ctx.il_switch_state_time = TIM2_GetMs();
			il_ctx.il_state = IL_STATE_DJ_LOCKED_TRANSITION2;
		}
		break;
	case IL_STATE_DJ_LOCKED_TRANSITION2:
		// Wait for LSDJ delay.
		if (TIM2_GetMs() > (il_ctx.il_switch_state_time + IL_ZDJ_LOCKING_LSDJ_DELAY_MS)) {
			// Turn LSDJ off.
			IL_SetState(0b000000010);
			// Compute next state.
			il_ctx.il_state = IL_STATE_DJ_LOCKED;
		}
		break;
	case IL_STATE_DJ_LOCKED:
		// Manage LSGR, LSS and LSP.
		GPIO_Write(&GPIO_LSGR, (lsmcu_ctx.lsmcu_rheostat_0 != 0));
		GPIO_Write(&GPIO_LSS, ((lsmcu_ctx.lsmcu_rheostat_0 == 0) && (lsmcu_ctx.lsmcu_series_traction != 0)));
		GPIO_Write(&GPIO_LSP, ((lsmcu_ctx.lsmcu_rheostat_0 == 0) && (lsmcu_ctx.lsmcu_series_traction == 0)));
		// Manage LSRH.
		if (lsmcu_ctx.lsmcu_rheostat_0 == 0) {
			if ((lsmcu_ctx.lsmcu_lsrh_blink_request != 0) && (GPIO_Read(&GPIO_LSRH) != 0)) {
				GPIO_Write(&GPIO_LSRH, 0);
				il_ctx.il_lsrh_blink_start_time = TIM2_GetMs();
			}
			else {
				if (TIM2_GetMs() > (il_ctx.il_lsrh_blink_start_time + IL_LSRH_BLINK_DURATION_MS)) {
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
			IL_SetState(0);
			il_ctx.il_state = IL_STATE_OFF;
		}
		else {
			// Check DJ.
			if (lsmcu_ctx.lsmcu_dj_closed == 0) {
				// Come back to previous state.
				IL_SetState(0b111111111);
				// Compute next state.
				il_ctx.il_state = IL_STATE_BL_UNLOCKED;
			}
		}
		break;
	default:
		// Turn all lights off.
		il_ctx.il_state = IL_STATE_OFF;
	}
}
