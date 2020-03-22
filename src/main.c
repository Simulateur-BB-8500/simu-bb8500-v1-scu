/*
 * main.c
 *
 *  Created on: 5 sept. 2017
 *      Author: Ludo
 */

// Peripherals.
#include "adc.h"
#include "dac.h"
#include "gpio.h"
#include "rcc.h"
#include "tim.h"
#include "usart.h"
// Components.
#include "stepper.h"
// Applicative.
#include "bl.h"
#include "common.h"
#include "dep.h"
#include "fpb.h"
#include "fd.h"
#include "il.h"
#include "kvb.h"
#include "lssgkcu.h"
#include "mano.h"
#include "mapping.h"
#include "mp.h"
#include "mpinv.h"
#include "pbl2.h"
#include "s.h"
#include "tch.h"
#include "vacma.h"
#include "zba.h"
#include "zlfr.h"
#include "zpt.h"

/*** Main global variables ***/

LSMCU_Context lsmcu_ctx;

/* MAIN FUNCTION.
 * @param: 	None.
 * @return: 0.
 */
int main(void) {

	/* Init peripherals */
	// Peripherals.
	RCC_Init();
	GPIO_Init();
	TIM2_Init(); // Time keeper.
	TIM5_Init(); // Tachro.
	TIM6_Init(); // KVB sweep.
	TIM7_Init(); // Manometers.
	TIM8_Init(); // LVAL PWM.
	ADC1_Init();
	DAC_Init();
	USART1_Init();
	// Communication.
	LSSGKCU_Init();
	// Dashboard.
	BL_Init();
	DEP_Init();
	FD_Init();
	FPB_Init();
	IL_Init();
	KVB_Init();
	MANO_Init(&mano_cp, &stepper_cp, &GPIO_MCP_1, &GPIO_MCP_2, 100, 3072, 20, 100);
	MANO_Init(&mano_re, &stepper_re, &GPIO_MRE_1, &GPIO_MRE_2, 100, 3072, 20, 100);
	MANO_Init(&mano_cg, &stepper_cg, &GPIO_MCG_1, &GPIO_MCG_2, 100, 3072, 20, 100);
	MANO_Init(&mano_cf1, &stepper_cf1, &GPIO_MCF1_1, &GPIO_MCF1_2, 60, 3072, 20, 100);
	MANO_Init(&mano_cf2, &stepper_cf2, &GPIO_MCF2_1, &GPIO_MCF2_2, 60, 3072, 20, 100);
	MANOS_Init();
	MP_Init();
	MPINV_Init();
	PBL2_Init();
	S_Init();
	TCH_Init();
	VACMA_Init();
	ZBA_Init();
	ZLFR_Init();
	ZPT_Init();

	/* Init context */
	lsmcu_ctx.lsmcu_zba_closed = 0;
	lsmcu_ctx.lsmcu_bl_unlocked = 0;
	lsmcu_ctx.lsmcu_dj_closed = 0;
	lsmcu_ctx.lsmcu_dj_locked = 0;
	lsmcu_ctx.lsmcu_zpt_raised = 0;
	lsmcu_ctx.lsmcu_speed_kmh = 0;

	/* LSMCU main loop */
	while(1) {
		// Peripherals tasks.
		ADC1_Task(&lsmcu_ctx);
		// Communication tasks.
		LSSGKCU_Task();
		// Dashboard tasks.
		BL_Task(&lsmcu_ctx);
		DEP_Task(&lsmcu_ctx);
		FD_Task();
		FPB_Task();
		IL_Task(&lsmcu_ctx);
		KVB_Task(&lsmcu_ctx);
		MANO_NeedleTask(&mano_cp);
		MANO_NeedleTask(&mano_re);
		MANO_NeedleTask(&mano_cg);
		MANO_NeedleTask(&mano_cf1);
		MANO_NeedleTask(&mano_cf2);
		MP_Task(&lsmcu_ctx);
		MPINV_Task();
		PBL2_Task(&lsmcu_ctx);
		S_Task();
		TCH_Task();
		VACMA_Task(&lsmcu_ctx);
		ZBA_Task(&lsmcu_ctx);
		ZPT_Task(&lsmcu_ctx);
	}
	return (0);
}
