/*
 * rcc.c
 *
 *  Created on: 9 sept. 2017
 *      Author: Ludo
 */

#include "rcc.h"

#include "flash_reg.h"
#include "rcc_reg.h"

/*** RCC functions ***/

/* CONFIGURE MCU CLOCK TREE.
 * @param: None.
 * @return: None.
 */
void RCC_Init(void) {
	// Use HSI first for start-up.
	RCC -> CR &= ~(0b1 << 16); // HSEON = '0'.
	RCC -> CR |= (0b1 << 0); // HSION='1'.
	while (((RCC -> CR) & (0b1 << 1)) == 0); // // Wait for HSI to be stable (HSIRDY='1').
	RCC -> CFGR &= ~(0b11 << 0); // Select HSI as system clock (SW='00').
	while (((RCC -> CFGR) & (0b11 << 0)) != 0b00); // // Wait for clock switch (SWS='00').
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
	// Use main PLL output clock as system clock.
	FLASH -> ACR |= (0b1 << 8); // Enable flash prefetch.
	// Increase flash latency according to new system clock frequency (see p.74 of RM0385 datasheet).
	// HCLK = SYSCLK = 100MHz -> flash latency must be set to 3 wait states (WS).
	FLASH -> ACR &= ~(0b1111 << 0); // Reset bits 0-3.
	FLASH -> ACR |= 3; // LATENCY=3.
	while (((FLASH -> ACR) & (0b1111 << 0)) != 3);
	RCC -> CFGR |= (0b10 << 0); // // Select PLLCLK (SW='10').
	while (((RCC -> CFGR) & (0b11 << 0)) != 0b10); // // Wait for clock switch (SWS='00').
#ifdef RCC_OUTPUT_CLOCK
	// Output HSI on MCO1 (PA8 as AF0) and SYSCLK on MCO2 (PC9 as AF0) with both prescalers = 4.
	RCC -> CFGR &= 0x369FFFFF;
	RCC -> CFGR |= 0x36000000;
#endif
}
