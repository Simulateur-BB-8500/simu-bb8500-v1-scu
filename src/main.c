/*
 * main.c
 *
 *  Created on: 5 sept. 2017
 *      Author: Ludo
 */

// Peripherals.
#include "adc.h"
#include "dac.h"
#include "dma.h"
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
#include "emergency.h"
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
#include "vacma.h"
#include "whistle.h"
#include "zba.h"
#include "zlfr.h"
#include "zpt.h"
#include "zsur.h"
// Utils.
#include "stdint.h"
// Applicative.
#include "lsmcu.h"
#include "lsagiu.h"
#include "string.h"

/*** MAIN global variables ***/

LSMCU_Context lsmcu_ctx;

/*** MAIN functions ***/

/* MAIN FUNCTION.
 * @param: 	None.
 * @return: 0.
 */
int main(void) {
	// Init interrupts, clocks and GPIOs.
	NVIC_init();
	RCC_init();
	GPIO_init();
	// Global time base.
	TIM2_init();
	// Analog.
	TIM1_init(ADC_CONVERSION_PERIOD_MS); // ADC trigger.
	DMA2_STR0_init();
	ADC1_init();
	DAC_init();
	// Communication interface.
	LSAGIU_init();
	// Simulator modules.
	BELL_init();
	BL_init();
	BPGD_init();
	COMPRESSOR_init();
	EMERGENCY_init();
	FD_init();
	FPB_init();
	KVB_init();
	MANOMETER_init_all();
	MP_init();
	MPINV_init();
	PICTOGRAMS_init();
	PBL2_init();
	TCH_init();
	VACMA_init();
	WHISTLE_init();
	ZBA_init();
	ZLFR_init();
	ZPT_init();
	ZSUR_init();
	// Main loop.
	while (1) {
		// Communication tasks.
		LSAGIU_task();
		// Simulator tasks.
		BELL_task();
		BL_task();
		BPGD_task();
		COMPRESSOR_task();
		EMERGENCY_task();
		FD_task();
		FPB_task();
		KVB_task();
		MANOMETER_manage_power_all();
		MP_task();
		MPINV_task();
		PBL2_task();
		PICTOGRAMS_task();
		TCH_task();
		VACMA_task();
		WHISTLE_task();
		ZBA_task();
		ZPT_task();
		ZSUR_task();
	}
	return 0;
}
