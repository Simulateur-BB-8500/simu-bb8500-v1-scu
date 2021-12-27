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
#include "nvic.h"
#include "rcc.h"
#include "tim.h"
#include "usart.h"
// Simulator.
#include "bell.h"
#include "bl.h"
#include "bpgd.h"
#include "compressor.h"
#include "fpb.h"
#include "fd.h"
#include "kvb.h"
#include "manometer.h"
#include "mapping.h"
#include "mp.h"
#include "mpinv.h"
#include "pbl2.h"
#include "pictograms.h"
#include "tch.h"
#include "urgency.h"
#include "vacma.h"
#include "whistle.h"
#include "zba.h"
#include "zlfr.h"
#include "zpt.h"
// Applicative.
#include "lsmcu.h"
#include "lssgiu.h"

/*** MAIN global variables ***/

LSMCU_Context lsmcu_ctx;

/*** MAIN functions ***/

/* MAIN FUNCTION.
 * @param: 	None.
 * @return: 0.
 */
int main(void) {
	// Init Peripherals.
	NVIC_Init();
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
	LSSGIU_Init();
	// Init simulator modules.
	BELL_Init();
	BL_Init();
	BPGD_Init();
	COMPRESSOR_Init();
	FD_Init();
	FPB_Init();
	KVB_Init();
	MANOMETER_InitAll();
	MP_Init();
	MPINV_Init();
	PICTOGRAMS_Init();
	PBL2_Init();
	TCH_Init();
	URGENCY_Init();
	VACMA_Init();
	WHISTLE_Init();
	ZBA_Init();
	ZLFR_Init();
	ZPT_Init();
	// Main loop.
	while (1) {
		// Peripherals tasks.
		ADC1_Task();
		// Communication tasks.
		LSSGIU_Task();
		// Simulator tasks.
		BELL_Task();
		BL_Task();
		BPGD_Task();
		COMPRESSOR_Task();
		FD_Task();
		FPB_Task();
		KVB_Task();
		MANOMETER_ManagePowerAll();
		MP_Task();
		MPINV_Task();
		PBL2_Task();
		PICTOGRAMS_Task();
		TCH_Task();
		URGENCY_Task();
		VACMA_Task();
		WHISTLE_Task();
		ZBA_Task();
		ZPT_Task();
	}
	return 0;
}
