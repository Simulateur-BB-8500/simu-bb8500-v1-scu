/*
 * mp.c
 *
 *  Created on: 22 mar. 2020
 *      Author: Ludo
 */

#include "mp.h"

#include "common.h"
#include "gpio.h"
#include "mapping.h"
#include "sw2.h"

/*** MP local structures ***/

typedef struct {
	SW2_Context mp_0;
	unsigned char mp_0_on;
	SW2_Context mp_tp;
	unsigned char mp_tp_on;
	SW2_Context mp_tm;
	unsigned char mp_tm_on;
	SW2_Context mp_pr;
	unsigned char mp_pr_on;
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
} MP_Context;

/*** MP local global variables ***/

static MP_Context mp_ctx;

/*** MP functions ***/

/* INIT MP MODULE.
 * @param:	None.
 * @return:	None.
 */
void MP_Init(void) {
	// Init GPIOs.
	SW2_Init(&mp_ctx.mp_0, &GPIO_MP_0, 1, 100); // MP_0 active high.
	mp_ctx.mp_0_on = 1;
	SW2_Init(&mp_ctx.mp_tp, &GPIO_MP_TP, 1, 100); // MP_TP active high.
	mp_ctx.mp_tp_on = 0;
	SW2_Init(&mp_ctx.mp_tm, &GPIO_MP_TM, 1, 100); // MP_TM active high.
	mp_ctx.mp_tm_on = 0;
	SW2_Init(&mp_ctx.mp_pr, &GPIO_MP_PR, 1, 100); // MP_PR active high.
	mp_ctx.mp_pr_on = 0;
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
}

/* MAIN ROUTINE OF MP MODULE.
 * @param:	None.
 * @return:	None.
 */
void MP_Task(void) {

}
