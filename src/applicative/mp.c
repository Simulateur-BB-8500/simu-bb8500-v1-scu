/*
 * mp.c
 *
 *  Created on: 22 mar. 2020
 *      Author: Ludo
 */

#include "mp.h"

#include "common.h"
#include "gpio.h"
#include "lssgkcu.h"
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
	SW2_Context mp_0;
	SW2_Context mp_tp;
	unsigned char mp_tp_on;
	SW2_Context mp_tm;
	unsigned char mp_tm_on;
	SW2_Context mp_pr;
	SW2_Context mp_p;
	unsigned char mp_p_on;
	SW2_Context mp_fp;
	unsigned char mp_fp_on;
	SW2_Context mp_fm;
	unsigned char mp_fm_on;
	SW2_Context mp_fr;
	unsigned char mp_fr_on;
	SW2_Context mp_tr;
	unsigned char mp_tr_on;
	// Rheostat management.
	unsigned char mp_gear_count;
	unsigned long mp_gear_switch_next_time;
} MP_Context;

/*** MP local global variables ***/

static MP_Context mp_ctx;

/*** MP local functions ***/

/* INCREASE GEAR COUNT.
 * @param:	None.
 * @return:	None.
 */
void MP_IncreaseGear(void) {
	// Check gear count.
	if (mp_ctx.mp_gear_count < MP_GEAR_MAX) {
		// Increment gear and send command.
		mp_ctx.mp_gear_count++;
		lsmcu_ctx.lsmcu_lsrh_blink_request = 1;
		LSSGKCU_Send(LSMCU_OUT_MP_T_MORE);
	}
}

/* DECREASE GEAR COUNT.
 * @param:	None.
 * @return:	None.
 */
void MP_DecreaseGear(void) {
	// Check gear count.
	if (mp_ctx.mp_gear_count > 0) {
		// Decrement gear and send command.
		mp_ctx.mp_gear_count--;
		lsmcu_ctx.lsmcu_lsrh_blink_request = 1;
		LSSGKCU_Send(LSMCU_OUT_MP_T_LESS);
	}
}

/*** MP functions ***/

/* INIT MP MODULE.
 * @param:	None.
 * @return:	None.
 */
void MP_Init(void) {
	// Init GPIOs.
	SW2_Init(&mp_ctx.mp_0, &GPIO_MP_0, 1, 100); // MP_0 active high.
	SW2_Init(&mp_ctx.mp_tp, &GPIO_MP_TP, 1, 100); // MP_TP active high.
	mp_ctx.mp_tp_on = 0;
	SW2_Init(&mp_ctx.mp_tm, &GPIO_MP_TM, 1, 100); // MP_TM active high.
	mp_ctx.mp_tm_on = 0;
	SW2_Init(&mp_ctx.mp_pr, &GPIO_MP_PR, 1, 100); // MP_PR active high.
	SW2_Init(&mp_ctx.mp_p, &GPIO_MP_P, 1, 100); // MP_P active high.
	mp_ctx.mp_p_on = 0;
	SW2_Init(&mp_ctx.mp_fp, &GPIO_MP_FP, 1, 100); // MP_FP active high.
	mp_ctx.mp_fp_on = 0;
	SW2_Init(&mp_ctx.mp_fm, &GPIO_MP_FM, 1, 100); // MP_FM active high.
	mp_ctx.mp_fm_on = 0;
	SW2_Init(&mp_ctx.mp_fr, &GPIO_MP_FR, 1, 100); // MP_FR active high.
	mp_ctx.mp_fr_on = 0;
	SW2_Init(&mp_ctx.mp_tr, &GPIO_MP_TR, 0, 100); // MP_TR active low.
	mp_ctx.mp_tr_on = 0;
	GPIO_Configure(&GPIO_MP_SH_ENABLE, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init context.
	mp_ctx.mp_gear_count = 0;
	mp_ctx.mp_gear_switch_next_time = 0;
	// Init global context.
	lsmcu_ctx.lsmcu_rheostat_0 = 1;
	lsmcu_ctx.lsmcu_series_traction = 1;
}

/* MAIN ROUTINE OF MP MODULE.
 * @param:	None.
 * @return:	None.
 */
void MP_Task(void) {
	// Update global context.
	lsmcu_ctx.lsmcu_rheostat_0 = (mp_ctx.mp_gear_count == 0) ? 1 : 0;
	// MP.0.
	SW2_UpdateState(&mp_ctx.mp_0);
	if (mp_ctx.mp_0.sw2_state == SW2_ON) {
		// Decrease gear count until 0.
		if ((mp_ctx.mp_gear_count > 0) && (TIM2_GetMs() > (mp_ctx.mp_gear_switch_next_time + MP_T_LESS_PERIOD_MS))) {
			MP_DecreaseGear();
			//if (mp_ctx.mp_gear_count == 0) {
				//LSSGKCU_Send(LSMCU_OUT_MP_0);
			//}
			// Update next time.
			mp_ctx.mp_gear_switch_next_time = TIM2_GetMs() + MP_T_LESS_PERIOD_MS;
		}
	}
	// MP.T+.
	SW2_UpdateState(&mp_ctx.mp_tp);
	if (mp_ctx.mp_tp.sw2_state == SW2_ON) {
		// Send command on change.
		if (mp_ctx.mp_tp_on == 0) {
			MP_IncreaseGear();
		}
		mp_ctx.mp_tp_on = 1;
	}
	else {
		mp_ctx.mp_tp_on = 0;
	}
	// MP.T-.
	SW2_UpdateState(&mp_ctx.mp_tm);
	if (mp_ctx.mp_tm.sw2_state == SW2_ON) {
		// Send command on change.
		if (mp_ctx.mp_tm_on == 0) {
			MP_DecreaseGear();
		}
		mp_ctx.mp_tm_on = 1;
	}
	else {
		mp_ctx.mp_tm_on = 0;
	}
	// MP.PR
	SW2_UpdateState(&mp_ctx.mp_pr);
	if (mp_ctx.mp_pr.sw2_state == SW2_ON) {
		// Increase gear count until maximum.
		if ((mp_ctx.mp_gear_count < MP_GEAR_MAX) && (TIM2_GetMs() > (mp_ctx.mp_gear_switch_next_time + MP_T_MORE_PERIOD_MS))) {
			MP_IncreaseGear();
			// Update next time.
			mp_ctx.mp_gear_switch_next_time = TIM2_GetMs() + MP_T_MORE_PERIOD_MS;
		}
	}
}
