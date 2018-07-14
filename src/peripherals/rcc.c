/*
 * rcc.c
 *
 *  Created on: 9 sept. 2017
 *      Author: Ludovic
 */

#include "flash_reg.h"
#include "rcc.h"
#include "rcc_reg.h"

/*** RCC functions ***/

/* CONFIGURE MCU CLOCK TREE.
 * @param: None.
 * @return: None.
 */
void RCC_Init(void) {

	/* Use HSI first for start-up*/

	// Disable HSE.
	RCC -> CR &= ~(0b11 << 16); // HSEON = '0'.
	// Enable HSI.
	RCC -> CR |= (0b1 <<0); // HSION='1'.
	// Wait for HSI to be stable.
	while (((RCC -> CR) & (0b1 << 1)) != (0b1 << 1)); // HSIRDY='1'.
	// Select HSI as system clock.
	RCC -> CFGR &= ~(0b11 << 0); // SW='00'.
	// Wait for clock switch.
	while (((RCC -> CFGR) & (0b11 << 0)) != 0b00); // SWS='00'.

	/* Peripherals clock prescalers */

	// HPRE = 1 -> HCLK = SYSCLK = 200MHz (max 216).
	// PPRE1 = 4 -> PCLK1 = HCLK/4 = 50MHz (max 54 for device, max 65 for TIM2 use (PSC register)).
	// PPRE2 = 4 -> PCLK2 = HCLK/4 = 50MHz (max 108).
	RCC -> CFGR &= 0xFFE0030F; // Reset bits 4-7 and 10-15 + HPRE='0000' (1).
	RCC -> CFGR |= (0b101 << 10) | (0b101 << 13); // PPRE1='101' (4) and PPRE2='101' (4).

	/* Configure main PLL */

	// Ensure PLL is off before configure it.
	RCC -> CR &= ~(0b1 << 24);
	// Source = HSI 16MHz
	// M = 16 -> input clock = 1MHz (typical value).
	// N = 400 -> VCO output clock = 400MHz (max 432).
	// P = 2 -> PLLCLK = 200MHz (max 216).
	// Q = 8 -> PLL48CLK = 50MHz (min 48 - max 75).
	RCC -> PLLCFGR = 0; // Reset all bits + PLLSRC='0' (HSI) + PLLP='00' (2)
	RCC -> PLLCFGR |= (16 << 0) | (400 << 6) | (8 << 24);
	// Enable PLL.
	RCC -> CR |= (0b1 << 24);
	// Wait for PLL to be ready.
	while (((RCC -> CR) & (0b1 << 25)) == 0);

	/* Use main PLL output clock as system clock */

	// Enable flash prefetch.
	FLASH -> ACR |= (0b1 << 8);
	// Increase flash latency according to new system clock frequency (see p.74 of RM0385 datasheet).
	// HCLK = SYSCLK = 200MHz -> flash latency must be set to 6 wait states (WS).
	FLASH -> ACR &= ~(0b1111 << 0); // Reset bits 0-3.
	FLASH -> ACR |= 6; // LATENCY=6.
	while (((FLASH -> ACR) & (0b1111 << 0)) != 6);

	// Select PLLCLK.
	RCC -> CFGR |= (0b10 << 0); // SW='10'.
	// Wait for clock switch.
	while (((RCC -> CFGR) & (0b11 << 0)) != 0b10); // SWS='00'.

#ifdef RCC_OUTPUT_CLOCK
	// Output HSI on MCO1 (PA8 as AF0) and SYSCLK on MCO2 (PC9 as AF0) with both prescalers = 4.
	RCC -> CFGR &= 0x369FFFFF;
	RCC -> CFGR |= 0x36000000;
#endif
}
