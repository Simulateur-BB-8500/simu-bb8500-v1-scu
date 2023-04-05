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
#include "zsur.h"
// Utils.
#include "stdint.h"
// Applicative.
#include "lsmcu.h"
#include "lssgiu.h"
#include "mode.h"
#include "string.h"

/*** MAIN global variables ***/

LSMCU_Context lsmcu_ctx;

/*** MAIN functions ***/

/* MAIN FUNCTION.
 * @param: 	None.
 * @return: 0.
 */
int main(void) {
	// Local variables.
	char str_value[16];
	uint32_t print_timestamp = 0;
	// Init Peripherals.
	NVIC_init();
	RCC_init();
	GPIO_init();
	TIM2_init(); // Time keeper.
	TIM5_init(); // Tachro.
	TIM6_init(); // KVB sweep.
	TIM7_init(); // Manometers.
	TIM8_init(); // LVAL PWM.
	ADC1_init();
	DAC_init();
	USART1_init();
	// Init communication interface.
	LSSGIU_Init();
	// Init simulator modules.
	BELL_init();
	BL_init();
	BPGD_init();
	COMPRESSOR_init();
	FD_init();
	FPB_init();
	KVB_init();
	MANOMETER_init_all();
	MP_init();
	MPINV_init();
	PICTOGRAMS_init();
	PBL2_init();
	TCH_init();
	URGENCY_init();
	VACMA_init();
	WHISTLE_init();
	ZBA_init();
	ZLFR_init();
	ZPT_init();
	ZSUR_init();
	// Main loop.
	while (1) {
		// Peripherals tasks.
		ADC1_task();
		// Communication tasks.
		LSSGIU_Task();
		// Simulator tasks.
		BELL_task();
		BL_task();
		BPGD_task();
		COMPRESSOR_task();
		FD_task();
		FPB_task();
		KVB_task();
		MANOMETER_manage_power_all();
		MP_task();
		MPINV_task();
		PBL2_task();
		PICTOGRAMS_task();
		TCH_task();
		URGENCY_task();
		VACMA_task();
		WHISTLE_task();
		ZBA_task();
		ZPT_task();
		ZSUR_task();
#ifdef DEBUG
		// Print manometers pressure.
		if (TIM2_get_milliseconds() > (print_timestamp + 1000)) {
			MANOMETER_print_data();
			print_timestamp = TIM2_get_milliseconds();
		}
#endif
	}
	return 0;
}
