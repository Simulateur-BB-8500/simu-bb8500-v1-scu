/*
 * vacma.c
 *
 *  Created on: 22 mar. 2020
 *      Author: Ludo
 */

#include "vacma.h"

#include "gpio.h"
#include "lsmcu.h"
#include "mapping.h"
#include "sw2.h"
#include "tim.h"

/*** VACMA local macros ***/

#define VACMA_RELEASED_ALARM_START_MS	5000
#define VACMA_HOLD_ALARM_START_MS		55000
#define VACMA_ALARM_DURATION_MS			5000

/*** VACMA local structures ***/

typedef enum {
	VACMA_STATE_OFF,
	VACMA_STATE_HOLD,
	VACMA_STATE_HOLD_ALARM,
	VACMA_STATE_RELEASED,
	VACMA_STATE_RELEASED_ALARM,
	VACMA_STATE_URGENCY
} VACMA_State;

typedef struct {
	// Inputs.
	SW2_Context bl_zva;
	SW2_Context mp_va;
	// State machine.
	VACMA_State state;
	unsigned int switch_state_time; // In ms.
} VACMA_Context;

/*** VACMA external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** VACMA local global variables ***/

static VACMA_Context vacma_ctx;

/*** VACMA functions ***/

/* INIT VACMA MODULE.
 * @param:	None.
 * @return:	None.
 */
void VACMA_Init(void) {
	// Init GPIOs.
	SW2_Init(&vacma_ctx.bl_zva, &GPIO_BL_ZVA, 0, 100); // MP_0 active low.
	SW2_Init(&vacma_ctx.mp_va, &GPIO_MP_VA, 0, 100); // MP_0 active low.
	GPIO_Configure(&GPIO_VACMA_HOLD_ALARM, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_VACMA_RELEASED_ALARM, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Write(&GPIO_VACMA_HOLD_ALARM, 0);
	GPIO_Write(&GPIO_VACMA_RELEASED_ALARM, 0);
	// Init context.
	vacma_ctx.state = VACMA_STATE_OFF;
	vacma_ctx.switch_state_time = 0;
}

/* MAIN ROUTINE OF VACMA MODULE.
 * @param:	None.
 * @return:	None.
 */
void VACMA_Task(void) {
	// Update inputs.
	SW2_UpdateState(&vacma_ctx.bl_zva);
	SW2_UpdateState(&vacma_ctx.mp_va);
	// Perform state machine.
	switch (vacma_ctx.state) {
	case VACMA_STATE_OFF:
		// Check inputs.
		if ((lsmcu_ctx.speed_kmh > 0) || (vacma_ctx.bl_zva.state == SW2_ON)) {
			// Enable VACMA.
			if (vacma_ctx.mp_va.state == SW2_ON) {
				vacma_ctx.state = VACMA_STATE_HOLD;
			}
			else {
				vacma_ctx.state = VACMA_STATE_RELEASED;
			}
			vacma_ctx.switch_state_time = TIM2_GetMs();
		}
		break;
	case VACMA_STATE_HOLD:
		if ((lsmcu_ctx.speed_kmh == 0) && (vacma_ctx.bl_zva.state == SW2_OFF)) {
			// Disable VACMA.
			GPIO_Write(&GPIO_VACMA_HOLD_ALARM, 0);
			GPIO_Write(&GPIO_VACMA_RELEASED_ALARM, 0);
			vacma_ctx.state = VACMA_STATE_OFF;
		}
		else {
			if (TIM2_GetMs() > (vacma_ctx.switch_state_time + VACMA_HOLD_ALARM_START_MS)) {
				// Trigger hold alarm.
				GPIO_Write(&GPIO_VACMA_HOLD_ALARM, 1);
				vacma_ctx.switch_state_time = TIM2_GetMs();
				vacma_ctx.state = VACMA_STATE_HOLD_ALARM;
			}
			else {
				if (vacma_ctx.mp_va.state == SW2_OFF) {
					vacma_ctx.switch_state_time = TIM2_GetMs();
					vacma_ctx.state = VACMA_STATE_RELEASED;
				}
			}
		}
		break;
	case VACMA_STATE_HOLD_ALARM:
		if ((lsmcu_ctx.speed_kmh == 0) && (vacma_ctx.bl_zva.state == SW2_OFF)) {
			// Disable VACMA.
			GPIO_Write(&GPIO_VACMA_HOLD_ALARM, 0);
			GPIO_Write(&GPIO_VACMA_RELEASED_ALARM, 0);
			vacma_ctx.state = VACMA_STATE_OFF;
		}
		else {
			if (TIM2_GetMs() > (vacma_ctx.switch_state_time + VACMA_ALARM_DURATION_MS)) {
				// Trigger urgency brake.
				GPIO_Write(&GPIO_VACMA_HOLD_ALARM, 0);
				GPIO_Write(&GPIO_VACMA_RELEASED_ALARM, 0);
				lsmcu_ctx.urgency = 1;
				vacma_ctx.state = VACMA_STATE_URGENCY;
			}
			else {
				if (vacma_ctx.mp_va.state == SW2_OFF) {
					GPIO_Write(&GPIO_VACMA_HOLD_ALARM, 0);
					GPIO_Write(&GPIO_VACMA_RELEASED_ALARM, 0);
					vacma_ctx.switch_state_time = TIM2_GetMs();
					vacma_ctx.state = VACMA_STATE_RELEASED;
				}
			}
		}
		break;
	case VACMA_STATE_RELEASED:
		if ((lsmcu_ctx.speed_kmh == 0) && (vacma_ctx.bl_zva.state == SW2_OFF)) {
			// Disable VACMA.
			GPIO_Write(&GPIO_VACMA_HOLD_ALARM, 0);
			GPIO_Write(&GPIO_VACMA_RELEASED_ALARM, 0);
			vacma_ctx.state = VACMA_STATE_OFF;
		}
		else {
			if (TIM2_GetMs() > (vacma_ctx.switch_state_time + VACMA_RELEASED_ALARM_START_MS)) {
				// Trigger released alarm.
				GPIO_Write(&GPIO_VACMA_RELEASED_ALARM, 1);
				vacma_ctx.switch_state_time = TIM2_GetMs();
				vacma_ctx.state = VACMA_STATE_RELEASED_ALARM;
			}
			else {
				if (vacma_ctx.mp_va.state == SW2_ON) {
					vacma_ctx.switch_state_time = TIM2_GetMs();
					vacma_ctx.state = VACMA_STATE_HOLD;
				}
			}
		}
		break;
	case VACMA_STATE_RELEASED_ALARM:
		if ((lsmcu_ctx.speed_kmh == 0) && (vacma_ctx.bl_zva.state == SW2_OFF)) {
			// Disable VACMA.
			GPIO_Write(&GPIO_VACMA_HOLD_ALARM, 0);
			GPIO_Write(&GPIO_VACMA_RELEASED_ALARM, 0);
			vacma_ctx.state = VACMA_STATE_OFF;
		}
		else {
			if (TIM2_GetMs() > (vacma_ctx.switch_state_time + VACMA_ALARM_DURATION_MS)) {
				// Trigger urgency brake.
				GPIO_Write(&GPIO_VACMA_HOLD_ALARM, 0);
				GPIO_Write(&GPIO_VACMA_RELEASED_ALARM, 0);
				lsmcu_ctx.urgency = 1;
				vacma_ctx.state = VACMA_STATE_URGENCY;
			}
			else {
				if (vacma_ctx.mp_va.state == SW2_ON) {
					GPIO_Write(&GPIO_VACMA_HOLD_ALARM, 0);
					GPIO_Write(&GPIO_VACMA_RELEASED_ALARM, 0);
					vacma_ctx.switch_state_time = TIM2_GetMs();
					vacma_ctx.state = VACMA_STATE_HOLD;
				}
			}
		}
		break;
	case VACMA_STATE_URGENCY:
		// Stay in this state while urgency flag is set.
		if (lsmcu_ctx.urgency == 0) {
			vacma_ctx.state = VACMA_STATE_OFF;
		}
		break;
	default:
		// Unknown state.
		vacma_ctx.state = VACMA_STATE_OFF;
		break;
	}
}


