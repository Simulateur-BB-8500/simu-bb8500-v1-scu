/*
 * mp.c
 *
 *  Created on: 22 mar. 2020
 *      Author: Ludo
 */

#include "mp.h"

#include "gpio.h"
#include "lsmcu.h"
#include "lssgiu.h"
#include "mapping.h"
#include "sw2.h"
#include "tim.h"
#include "types.h"
#include "usart.h"

/*** MP local macros ***/

#define MP_T_MORE_PERIOD_MS		500
#define MP_T_LESS_PERIOD_MS		350
#define MP_GEAR_MAX				16

/*** MP local structures ***/

typedef struct {
	// GPIOs.
	SW2_context_t zero;
	SW2_context_t t_more;
	uint8_t t_more_on;
	SW2_context_t t_less;
	uint8_t t_less_on;
	SW2_context_t t_fast;
	SW2_context_t preparation;
	uint8_t preparation_on;
	SW2_context_t f_more;
	uint8_t f_more_on;
	SW2_context_t f_less;
	uint8_t f_less_on;
	SW2_context_t f_fast;
	uint8_t f_fast_on;
	SW2_context_t transition;
	uint8_t transition_on;
	// Rheostat management.
	uint8_t gear_count;
	unsigned long gear_switch_next_time;
} MP_Context;

/*** MP external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** MP local global variables ***/

static MP_Context mp_ctx;

/*** MP local functions ***/

/* INCREASE GEAR COUNT.
 * @param:	None.
 * @return:	None.
 */
static void _MP_increase_gear(void) {
	// Check gear count.
	if (mp_ctx.gear_count < MP_GEAR_MAX) {
		// Increment gear and send command.
		mp_ctx.gear_count++;
		lsmcu_ctx.lsrh_blink_request = 1;
		LSSGIU_Send(LSMCU_OUT_MP_T_MORE);
	}
}

/* DECREASE GEAR COUNT.
 * @param:	None.
 * @return:	None.
 */
static void _MP_decrease_gear(void) {
	// Check gear count.
	if (mp_ctx.gear_count > 0) {
		// Decrement gear and send command.
		mp_ctx.gear_count--;
		lsmcu_ctx.lsrh_blink_request = 1;
		LSSGIU_Send(LSMCU_OUT_MP_T_LESS);
	}
}

/*** MP functions ***/

/* INIT MP MODULE.
 * @param:	None.
 * @return:	None.
 */
void MP_init(void) {
	// Init GPIOs.
	SW2_init(&mp_ctx.zero, &GPIO_MP_0, 1, 100); // MP_0 active high.
	SW2_init(&mp_ctx.t_more, &GPIO_MP_TM, 1, 100); // MP_TP active high.
	mp_ctx.t_more_on = 0;
	SW2_init(&mp_ctx.t_less, &GPIO_MP_TL, 1, 100); // MP_TM active high.
	mp_ctx.t_less_on = 0;
	SW2_init(&mp_ctx.t_fast, &GPIO_MP_TF, 1, 100); // MP_PR active high.
	SW2_init(&mp_ctx.preparation, &GPIO_MP_P, 1, 100); // MP_P active high.
	mp_ctx.preparation_on = 0;
	SW2_init(&mp_ctx.f_more, &GPIO_MP_FM, 1, 100); // MP_FP active high.
	mp_ctx.f_more_on = 0;
	SW2_init(&mp_ctx.f_less, &GPIO_MP_FL, 1, 100); // MP_FM active high.
	mp_ctx.f_less_on = 0;
	SW2_init(&mp_ctx.f_fast, &GPIO_MP_FF, 1, 100); // MP_FR active high.
	mp_ctx.f_fast_on = 0;
	SW2_init(&mp_ctx.transition, &GPIO_MP_TR, 0, 100); // MP_TR active low.
	mp_ctx.transition_on = 0;
	GPIO_configure(&GPIO_MP_SH_ENABLE, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init context.
	mp_ctx.gear_count = 0;
	mp_ctx.gear_switch_next_time = 0;
	// Init global context.
	lsmcu_ctx.rheostat_0 = 1;
	lsmcu_ctx.series_traction = 1;
}

/* MAIN ROUTINE OF MP MODULE.
 * @param:	None.
 * @return:	None.
 */
void MP_task(void) {
	// Update global context.
	lsmcu_ctx.rheostat_0 = (mp_ctx.gear_count == 0) ? 1 : 0;
	// MP.0.
	SW2_update_state(&mp_ctx.zero);
	if (mp_ctx.zero.state == SW2_ON) {
		// Decrease gear count until 0.
		if ((mp_ctx.gear_count > 0) && (TIM2_get_milliseconds() > (mp_ctx.gear_switch_next_time + MP_T_LESS_PERIOD_MS))) {
			_MP_decrease_gear();
			//if (mp_ctx.gear_count == 0) {
				//LSSGIU_Send(LSMCU_OUT_MP_0);
			//}
			// Update next time.
			mp_ctx.gear_switch_next_time = TIM2_get_milliseconds() + MP_T_LESS_PERIOD_MS;
		}
	}
	// MP.T+.
	SW2_update_state(&mp_ctx.t_more);
	if (mp_ctx.t_more.state == SW2_ON) {
		// Send command on change.
		if (mp_ctx.t_more_on == 0) {
			_MP_increase_gear();
		}
		mp_ctx.t_more_on = 1;
	}
	else {
		mp_ctx.t_more_on = 0;
	}
	// MP.T-.
	SW2_update_state(&mp_ctx.t_less);
	if (mp_ctx.t_less.state == SW2_ON) {
		// Send command on change.
		if (mp_ctx.t_less_on == 0) {
			_MP_decrease_gear();
		}
		mp_ctx.t_less_on = 1;
	}
	else {
		mp_ctx.t_less_on = 0;
	}
	// MP.PR
	SW2_update_state(&mp_ctx.t_fast);
	if (mp_ctx.t_fast.state == SW2_ON) {
		// Increase gear count until maximum.
		if ((mp_ctx.gear_count < MP_GEAR_MAX) && (TIM2_get_milliseconds() > (mp_ctx.gear_switch_next_time + MP_T_MORE_PERIOD_MS))) {
			_MP_increase_gear();
			// Update next time.
			mp_ctx.gear_switch_next_time = TIM2_get_milliseconds() + MP_T_MORE_PERIOD_MS;
		}
	}
}
