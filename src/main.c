/*
 * main.c
 *
 *  Created on: 5 sept. 2017
 *      Author: Ludo
 */

// Peripherals.
#include <lsmcu.h>
#include "adc.h"
#include "dac.h"
#include "gpio.h"
#include "rcc.h"
#include "tim.h"
#include "usart.h"
// Applicative.
#include "bl.h"
#include "comp.h"
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

/*** MAIN global variables ***/

LSMCU_Context lsmcu_ctx;

/*** MAIN functions ***/

/* MAIN FUNCTION.
 * @param: 	None.
 * @return: 0.
 */
int main(void) {
	// Init Peripherals.
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
	// Init communication interface.
	LSSGKCU_Init();
	// Init dashboard modules.
	BL_Init();
	COMP_Init();
	DEP_Init();
	FD_Init();
	FPB_Init();
	IL_Init();
	KVB_Init();
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
	// Main loop.
	while (1) {
		// Peripherals tasks.
		ADC1_Task();
		// Communication tasks.
		LSSGKCU_Task();
		// Dashboard tasks.
		BL_Task();
		COMP_Task();
		DEP_Task();
		FD_Task();
		FPB_Task();
		IL_Task();
		KVB_Task();
		MANOS_ManagePower();
		MP_Task();
		MPINV_Task();
		PBL2_Task();
		S_Task();
		TCH_Task();
		VACMA_Task();
		ZBA_Task();
		ZPT_Task();
	}
	return (0);
}
