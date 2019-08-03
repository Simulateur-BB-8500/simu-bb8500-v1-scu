/*
 * main.c
 *
 *  Created on: 5 sept. 2017
 *      Author: Ludovic
 */


#include "bl.h"
#include "kvb.h"
#include "lssgkcu.h"
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
	TIM2_Init();
	GPIO_Init();
	DAC_Init();
	ADC1_Init();

	/* Applicative layers */
	BL_Init();
	KVB_Init();
	LSSGKCU_Init();
	MPINV_Init();
	TCH_Init();
	ZPT_Init();

	/* Init context */
	lsmcu_ctx.bl_unlocked = 0;

	// Stepper test.
	STEPPER_Context cp;
	STEPPER_Init(&cp, &GPIO_MCP_1, &GPIO_MCP_2);
	unsigned int stepper_next_time_ms = 0;

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

		// Stepper test.
		if (TIM2_GetMs() > stepper_next_time_ms) {
			STEPPER_Up(&cp);
			stepper_next_time_ms = TIM2_GetMs() + 100;
			GPIO_Toggle(&GPIO_LED_BLUE);
		}
	}

	return (0);
}
