/*
 * rcc.c
 *
 *  Created on: 9 sept. 2017
 *      Author: Ludo
 */

#include "rcc.h"

#include "flash.h"
#include "rcc_reg.h"
#include "stdint.h"

/*** RCC local macros ***/

#define RCC_HSI_FREQUENCY_KHZ	16000

/*** RCC local global variables ***/

static uint32_t rcc_clock_frequency[RCC_CLOCK_LAST] = {RCC_HSI_FREQUENCY_KHZ, RCC_HSI_FREQUENCY_KHZ, RCC_HSI_FREQUENCY_KHZ};

/*** RCC functions ***/

/* CONFIGURE MCU CLOCK TREE.
 * @param: None.
 * @return: None.
 */
void RCC_init(void) {
	// Peripherals clock prescalers:
	// HPRE = 1 -> HCLK = SYSCLK = 100MHz (max 216).
	// PPRE1 = 4 -> PCLK1 = HCLK/4 = 25MHz (max 54).
	// PPRE2 = 4 -> PCLK2 = HCLK/4 = 25MHz (max 108).
	RCC -> CFGR &= 0xFFE0030F; // Reset bits 4-7 and 10-15 + HPRE='0000' (1).
	RCC -> CFGR |= (0b101 << 10) | (0b101 << 13); // PPRE1='101' (4) and PPRE2='101' (4).
	RCC -> DKCFGR1 &= ~(0b1 << 24); // Timers clock is 2*PLCKx (PPRE1 and PPRE2 != 1).
	// Configure main PLL.
	RCC -> CR &= ~(0b1 << 24);
	// Source = HSI 16MHz
	// M = 16 -> input clock = 1MHz (typical value).
	// N = 200 -> VCO output clock = 200MHz (max 432).
	// P = 2 -> PLLCLK = 100MHz (max 216).
	// Q = 4 -> PLL48CLK = 50MHz (min 48 - max 75).
	RCC -> PLLCFGR = 0; // Reset all bits + PLLSRC='0' (HSI) + PLLP='00' (2)
	RCC -> PLLCFGR |= (16 << 0) | (200 << 6) | (4 << 24);
	// Enable PLL.
	RCC -> CR |= (0b1 << 24);
	// Wait for PLL to be ready.
	while (((RCC -> CR) & (0b1 << 25)) == 0);
	// Increase flash latency according to new system clock frequency (see p.74 of RM0385 datasheet).
	// HCLK = SYSCLK = 100MHz -> flash latency must be set to 3 wait states (WS).
	FLASH_set_latency(3);
	// Switch to PLLCLK.
	RCC -> CFGR |= (0b10 << 0); // SW='10'.
	while (((RCC -> CFGR) & (0b11 << 0)) != 0b10); // // Wait for clock switch (SWS='00').
#ifdef RCC_OUTPUT_CLOCK
	// Output HSI on MCO1 (PA8 as AF0) and SYSCLK on MCO2 (PC9 as AF0) with both prescalers = 4.
	RCC -> CFGR &= 0x369FFFFF;
	RCC -> CFGR |= 0x36000000;
#endif
	// Update frequencies.
	rcc_clock_frequency[RCC_CLOCK_SYSCLK] = 100000;
	rcc_clock_frequency[RCC_CLOCK_PCLK1] = 25000;
	rcc_clock_frequency[RCC_CLOCK_PCLK2] = 25000;
}

/* GET RCC CLOC FREQUENCY.
 * @param rcc_clock:	RCC clock source.
 * @return freq_khz:	RCC clock frequency in kHz.
 */
uint32_t RCC_get_clock_frequency(RCC_clock_t rcc_clock) {
	// Local variables.
	uint32_t freq_khz = 0;
	// Check parameter.
	if (rcc_clock < RCC_CLOCK_LAST) {
		freq_khz = rcc_clock_frequency[rcc_clock];
	}
	return freq_khz;
}
