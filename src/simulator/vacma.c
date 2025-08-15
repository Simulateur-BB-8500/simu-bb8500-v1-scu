/*
 * vacma.c
 *
 *  Created on: 22 mar. 2020
 *      Author: Ludo
 */

#include "vacma.h"

#include "emergency.h"
#include "gpio.h"
#include "mapping.h"
#include "scu.h"
#include "sw2.h"
#include "tim.h"
#include "stdint.h"

/*** VACMA local macros ***/

#define VACMA_RELEASED_ALARM_START_MS	5000
#define VACMA_HOLD_ALARM_START_MS		55000
#define VACMA_ALARM_DURATION_MS			5000

/*** VACMA local structures ***/

/*******************************************************************/
typedef enum {
	VACMA_STATE_OFF,
	VACMA_STATE_HOLD,
	VACMA_STATE_HOLD_ALARM,
	VACMA_STATE_RELEASED,
	VACMA_STATE_RELEASED_ALARM,
	VACMA_STATE_EMERGENCY
} VACMA_state_t;

/*******************************************************************/
typedef struct {
	// Inputs.
	SW2_context_t bl_zva;
	SW2_context_t mp_va;
	// State machine.
	VACMA_state_t state;
	uint32_t switch_state_time; // In ms.
} VACMA_context_t;

/*** VACMA external global variables ***/

extern SCU_context_t scu_ctx;

/*** VACMA local global variables ***/

static VACMA_context_t vacma_ctx;

/*** VACMA functions ***/

/*******************************************************************/
void VACMA_init(void) {
	// Init GPIOs.
	SW2_init(&vacma_ctx.bl_zva, &GPIO_BL_ZVA, 0, 100); // MP_0 active low.
	SW2_init(&vacma_ctx.mp_va, &GPIO_MP_VA, 0, 100); // MP_0 active low.
	GPIO_configure(&GPIO_VACMA_HOLD_ALARM, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_VACMA_RELEASED_ALARM, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_write(&GPIO_VACMA_HOLD_ALARM, 0);
	GPIO_write(&GPIO_VACMA_RELEASED_ALARM, 0);
	// Init context.
	vacma_ctx.state = VACMA_STATE_OFF;
	vacma_ctx.switch_state_time = 0;
}

/*******************************************************************/
void VACMA_process(void) {
	// Update inputs.
	SW2_update_state(&vacma_ctx.bl_zva);
	SW2_update_state(&vacma_ctx.mp_va);
	// Perform state machine.
	switch (vacma_ctx.state) {
	case VACMA_STATE_OFF:
		// Check inputs.
		if ((scu_ctx.speed_kmh > 0) || (vacma_ctx.bl_zva.state == SW2_ON)) {
			// Enable VACMA.
			if (vacma_ctx.mp_va.state == SW2_ON) {
				vacma_ctx.state = VACMA_STATE_HOLD;
			}
			else {
				vacma_ctx.state = VACMA_STATE_RELEASED;
			}
			vacma_ctx.switch_state_time = TIM2_get_milliseconds();
		}
		break;
	case VACMA_STATE_HOLD:
		if ((scu_ctx.speed_kmh == 0) && (vacma_ctx.bl_zva.state == SW2_OFF)) {
			// Disable VACMA.
			GPIO_write(&GPIO_VACMA_HOLD_ALARM, 0);
			GPIO_write(&GPIO_VACMA_RELEASED_ALARM, 0);
			vacma_ctx.state = VACMA_STATE_OFF;
		}
		else {
			if (TIM2_get_milliseconds() > (vacma_ctx.switch_state_time + VACMA_HOLD_ALARM_START_MS)) {
				// Trigger hold alarm.
				GPIO_write(&GPIO_VACMA_HOLD_ALARM, 1);
				vacma_ctx.switch_state_time = TIM2_get_milliseconds();
				vacma_ctx.state = VACMA_STATE_HOLD_ALARM;
			}
			else {
				if (vacma_ctx.mp_va.state == SW2_OFF) {
					vacma_ctx.switch_state_time = TIM2_get_milliseconds();
					vacma_ctx.state = VACMA_STATE_RELEASED;
				}
			}
		}
		break;
	case VACMA_STATE_HOLD_ALARM:
		if ((scu_ctx.speed_kmh == 0) && (vacma_ctx.bl_zva.state == SW2_OFF)) {
			// Disable VACMA.
			GPIO_write(&GPIO_VACMA_HOLD_ALARM, 0);
			GPIO_write(&GPIO_VACMA_RELEASED_ALARM, 0);
			vacma_ctx.state = VACMA_STATE_OFF;
		}
		else {
			if (TIM2_get_milliseconds() > (vacma_ctx.switch_state_time + VACMA_ALARM_DURATION_MS)) {
				// Trigger emergency brake.
				GPIO_write(&GPIO_VACMA_HOLD_ALARM, 0);
				GPIO_write(&GPIO_VACMA_RELEASED_ALARM, 0);
				EMERGENCY_trigger();
				vacma_ctx.state = VACMA_STATE_EMERGENCY;
			}
			else {
				if (vacma_ctx.mp_va.state == SW2_OFF) {
					GPIO_write(&GPIO_VACMA_HOLD_ALARM, 0);
					GPIO_write(&GPIO_VACMA_RELEASED_ALARM, 0);
					vacma_ctx.switch_state_time = TIM2_get_milliseconds();
					vacma_ctx.state = VACMA_STATE_RELEASED;
				}
			}
		}
		break;
	case VACMA_STATE_RELEASED:
		if ((scu_ctx.speed_kmh == 0) && (vacma_ctx.bl_zva.state == SW2_OFF)) {
			// Disable VACMA.
			GPIO_write(&GPIO_VACMA_HOLD_ALARM, 0);
			GPIO_write(&GPIO_VACMA_RELEASED_ALARM, 0);
			vacma_ctx.state = VACMA_STATE_OFF;
		}
		else {
			if (TIM2_get_milliseconds() > (vacma_ctx.switch_state_time + VACMA_RELEASED_ALARM_START_MS)) {
				// Trigger released alarm.
				GPIO_write(&GPIO_VACMA_RELEASED_ALARM, 1);
				vacma_ctx.switch_state_time = TIM2_get_milliseconds();
				vacma_ctx.state = VACMA_STATE_RELEASED_ALARM;
			}
			else {
				if (vacma_ctx.mp_va.state == SW2_ON) {
					vacma_ctx.switch_state_time = TIM2_get_milliseconds();
					vacma_ctx.state = VACMA_STATE_HOLD;
				}
			}
		}
		break;
	case VACMA_STATE_RELEASED_ALARM:
		if ((scu_ctx.speed_kmh == 0) && (vacma_ctx.bl_zva.state == SW2_OFF)) {
			// Disable VACMA.
			GPIO_write(&GPIO_VACMA_HOLD_ALARM, 0);
			GPIO_write(&GPIO_VACMA_RELEASED_ALARM, 0);
			vacma_ctx.state = VACMA_STATE_OFF;
		}
		else {
			if (TIM2_get_milliseconds() > (vacma_ctx.switch_state_time + VACMA_ALARM_DURATION_MS)) {
				// Trigger emergency brake.
				GPIO_write(&GPIO_VACMA_HOLD_ALARM, 0);
				GPIO_write(&GPIO_VACMA_RELEASED_ALARM, 0);
				EMERGENCY_trigger();
				vacma_ctx.state = VACMA_STATE_EMERGENCY;
			}
			else {
				if (vacma_ctx.mp_va.state == SW2_ON) {
					GPIO_write(&GPIO_VACMA_HOLD_ALARM, 0);
					GPIO_write(&GPIO_VACMA_RELEASED_ALARM, 0);
					vacma_ctx.switch_state_time = TIM2_get_milliseconds();
					vacma_ctx.state = VACMA_STATE_HOLD;
				}
			}
		}
		break;
	case VACMA_STATE_EMERGENCY:
		// Stay in this state while emergency flag is set.
		if (scu_ctx.emergency == 0) {
			vacma_ctx.state = VACMA_STATE_OFF;
		}
		break;
	default:
		// Unknown state.
		vacma_ctx.state = VACMA_STATE_OFF;
		break;
	}
}
