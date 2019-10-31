/*
 * main.c
 *
 *  Created on: 5 sept. 2017
 *      Author: Ludovic
 */


#include "bl.h"
#include "kvb.h"
#include "lssgkcu.h"
#include "mano.h"
#include "mapping.h"
#include "mpinv.h"
#include "tch.h"
#include "zpt.h"
#include "adc.h"
#include "dac.h"
#include "gpio.h"
#include "rcc.h"
#include "stepper.h"
#include "tim.h"
#include "usart.h"

/*** Main structures ***/

typedef struct {
	unsigned char bl_unlocked;
} LSMCU_Context;

/*** Main global variables ***/

static LSMCU_Context lsmcu_ctx;

/* MAIN FUNCTION.
 * @param: 	None.
 * @return: 0.
 */
int main(void) {

	/* Init peripherals */
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

	/* Applicative layers */
	BL_Init();
	KVB_Init();
	LSSGKCU_Init();
	MPINV_Init();
	TCH_Init();
	ZPT_Init();
	MANO_Init(&mano_cp, &stepper_cp, &GPIO_MCP_1, &GPIO_MCP_2, 100, 3072, 20, 100);
	MANO_Init(&mano_re, &stepper_re, &GPIO_MRE_1, &GPIO_MRE_2, 100, 3072, 20, 100);
	MANO_Init(&mano_cg, &stepper_cg, &GPIO_MCG_1, &GPIO_MCG_2, 100, 3072, 20, 100);
	MANO_Init(&mano_cf1, &stepper_cf1, &GPIO_MCF1_1, &GPIO_MCF1_2, 60, 3072, 20, 100);
	MANO_Init(&mano_cf2, &stepper_cf2, &GPIO_MCF2_1, &GPIO_MCF2_2, 60, 3072, 20, 100);
	MANOS_Init();

	/* Init context */
	lsmcu_ctx.bl_unlocked = 0;

	/* LSMCU main loop */
	while(1) {

		// Peripherals tasks.
		ADC1_Task(lsmcu_ctx.bl_unlocked);

		// Applicative tasks.
		BL_Task(&lsmcu_ctx.bl_unlocked);
		KVB_Task(lsmcu_ctx.bl_unlocked);
		LSSGKCU_Task();
		MPINV_Task();
		TCH_Task();
		ZPT_Task();
	}

	return (0);
}
