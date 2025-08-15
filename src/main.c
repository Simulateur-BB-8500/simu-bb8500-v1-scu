/*
 * main.c
 *
 *  Created on: 5 sep. 2017
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
// Simulator.
#include "am.h"
#include "bell.h"
#include "bl.h"
#include "bpgd.h"
#include "bpsa.h"
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
#include "scu.h"
#include "sgdu.h"
#include "string.h"

/*** MAIN global variables ***/

SCU_context_t scu_ctx;

/*** MAIN functions ***/

/*******************************************************************/
int main(void) {
	// Init interrupts, clocks and GPIOs.
	NVIC_init();
	RCC_init();
	GPIO_init();
	// Global time base.
	TIM2_init();
	// Analog.
	TIM1_init(ADC_CONVERSION_PERIOD_MS);
	DMA2_STR0_init();
	ADC1_init();
	// Communication interface.
	SGDU_init();
	// Simulator modules.
	AM_init();
	BELL_init();
	BL_init();
	BPGD_init();
	BPSA_init();
	COMPRESSOR_init();
	EMERGENCY_init();
	FD_init();
	FPB_init();
	KVB_init();
	KVB_print_software_version();
	MANOMETER_init();
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
	// Specific mode to start with CP full.
	GPIO_configure(&GPIO_BPEV, GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_UP);
	TIM2_delay_milliseconds(1000);
	if (GPIO_read(&GPIO_BPEV) == 0) {
		// Set CP and CF pressures.
		MANOMETER_set_pressure(scu_ctx.manometer_cp, 9000, 2000);
		MANOMETER_set_pressure(scu_ctx.manometer_cf1, 2900, 1500);
		MANOMETER_set_pressure(scu_ctx.manometer_cf2, 3100, 1500);
		// Wait for needles to be ready.
		while ((((scu_ctx.manometer_cp) -> flag_is_moving) != 0) || (((scu_ctx.manometer_cf1) -> flag_is_moving) != 0) || (((scu_ctx.manometer_cf2) -> flag_is_moving) != 0));
	}
	// Main loop.
	while (1) {
		// Communication tasks.
		SGDU_process();
		// Simulator tasks.
		AM_process();
		BELL_process();
		BL_process();
		BPGD_process();
		BPSA_process();
		COMPRESSOR_process();
		EMERGENCY_process();
		FD_process();
		FPB_process();
		KVB_process();
		MANOMETER_manage_power();
		MP_process();
		MPINV_process();
		PBL2_process();
		PICTOGRAMS_process();
		TCH_process();
		VACMA_process();
		WHISTLE_process();
		ZBA_process();
		ZPT_process();
		ZSUR_process();
	}
	return 0;
}
