/*
 * main.c
 *
 *  Created on: 5 sept. 2017
 *      Author: Ludovic
 */

#include "adc.h"
#include "at.h"
#include "bl.h"
#include "dac.h"
#include "gpio.h"
#include "kvb.h"
#include "mpinv.h"
#include "rcc.h"
#include "tch.h"
#include "tim.h"
#include "zpt.h"

/* MAIN FUNCTION.
 * @param: 	None.
 * @return: 0.
 */
int main(void) {

	/* LSMCU init */

	// Peripherals.
	RCC_Init();
	TIM2_Init();
	GPIO_Init();
	DAC_Init();
	ADC1_Init();

	// Generic.
	AT_Init();

	// Simulator.
	BL_Init();
	MPINV_Init();
	ZPT_Init();
	KVB_Init();
	TCH_Init();

	// Global variables.
	unsigned char bl_unlocked = 0;

	/* LSMCU main loop */

	while(1) {

		// Peripherals.
		ADC1_Routine(bl_unlocked);

		// Generic.
		AT_Routine();

		// Simulator.
		BL_Routine(&bl_unlocked);
		MPINV_Routine();
		ZPT_Routine();
		KVB_Routine(bl_unlocked);
		TCH_Routine();
	}

	return (0);
}
