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
#include "usart.h"

/*** MP local macros ***/

#define MP_T_MORE_PERIOD_MS		500
#define MP_T_LESS_PERIOD_MS		350
#define MP_GEAR_MAX				16

/*** MP local structures ***/

typedef struct {
	// GPIOs.
	SW2_Context zero;
	SW2_Context t_more;
	unsigned char t_more_on;
	SW2_Context t_less;
	unsigned char t_less_on;
	SW2_Context t_fast;
	SW2_Context preparation;
	unsigned char preparation_on;
	SW2_Context f_more;
	unsigned char f_more_on;
	SW2_Context f_less;
	unsigned char f_less_on;
	SW2_Context f_fast;
	unsigned char f_fast_on;
	SW2_Context transition;
	unsigned char transition_on;
	// Rheostat management.
	unsigned char gear_count;
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
void MP_IncreaseGear(void) {
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
void MP_DecreaseGear(void) {
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
void MP_Init(void) {
	// Init GPIOs.
	SW2_Init(&mp_ctx.zero, &GPIO_MP_0, 1, 100); // MP_0 active high.
	SW2_Init(&mp_ctx.t_more, &GPIO_MP_TM, 1, 100); // MP_TP active high.
	mp_ctx.t_more_on = 0;
	SW2_Init(&mp_ctx.t_less, &GPIO_MP_TL, 1, 100); // MP_TM active high.
	mp_ctx.t_less_on = 0;
	SW2_Init(&mp_ctx.t_fast, &GPIO_MP_TF, 1, 100); // MP_PR active high.
	SW2_Init(&mp_ctx.preparation, &GPIO_MP_P, 1, 100); // MP_P active high.
	mp_ctx.preparation_on = 0;
	SW2_Init(&mp_ctx.f_more, &GPIO_MP_FM, 1, 100); // MP_FP active high.
	mp_ctx.f_more_on = 0;
	SW2_Init(&mp_ctx.f_less, &GPIO_MP_FL, 1, 100); // MP_FM active high.
	mp_ctx.f_less_on = 0;
	SW2_Init(&mp_ctx.f_fast, &GPIO_MP_FF, 1, 100); // MP_FR active high.
	mp_ctx.f_fast_on = 0;
	SW2_Init(&mp_ctx.transition, &GPIO_MP_TR, 0, 100); // MP_TR active low.
	mp_ctx.transition_on = 0;
	GPIO_Configure(&GPIO_MP_SH_ENABLE, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
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
void MP_Task(void) {
	// Update global context.
	lsmcu_ctx.rheostat_0 = (mp_ctx.gear_count == 0) ? 1 : 0;
	// MP.0.
	SW2_UpdateState(&mp_ctx.zero);
	if (mp_ctx.zero.state == SW2_ON) {
		// Decrease gear count until 0.
		if ((mp_ctx.gear_count > 0) && (TIM2_GetMs() > (mp_ctx.gear_switch_next_time + MP_T_LESS_PERIOD_MS))) {
			MP_DecreaseGear();
			//if (mp_ctx.gear_count == 0) {
				//LSSGIU_Send(LSMCU_OUT_MP_0);
			//}
			// Update next time.
			mp_ctx.gear_switch_next_time = TIM2_GetMs() + MP_T_LESS_PERIOD_MS;
		}
	}
	// MP.T+.
	SW2_UpdateState(&mp_ctx.t_more);
	if (mp_ctx.t_more.state == SW2_ON) {
		// Send command on change.
		if (mp_ctx.t_more_on == 0) {
			MP_IncreaseGear();
		}
		mp_ctx.t_more_on = 1;
	}
	else {
		mp_ctx.t_more_on = 0;
	}
	// MP.T-.
	SW2_UpdateState(&mp_ctx.t_less);
	if (mp_ctx.t_less.state == SW2_ON) {
		// Send command on change.
		if (mp_ctx.t_less_on == 0) {
			MP_DecreaseGear();
		}
		mp_ctx.t_less_on = 1;
	}
	else {
		mp_ctx.t_less_on = 0;
	}
	// MP.PR
	SW2_UpdateState(&mp_ctx.t_fast);
	if (mp_ctx.t_fast.state == SW2_ON) {
		// Increase gear count until maximum.
		if ((mp_ctx.gear_count < MP_GEAR_MAX) && (TIM2_GetMs() > (mp_ctx.gear_switch_next_time + MP_T_MORE_PERIOD_MS))) {
			MP_IncreaseGear();
			// Update next time.
			mp_ctx.gear_switch_next_time = TIM2_GetMs() + MP_T_MORE_PERIOD_MS;
		}
	}
}
