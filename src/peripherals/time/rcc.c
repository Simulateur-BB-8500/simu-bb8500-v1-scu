/*
 * rcc.c
 *
 *  Created on: 9 sept. 2017
 *      Author: Ludovic
 */

#include "rcc.h"
#include "rcc_reg.h"

/*** RCC functions ***/

/* CONFIGURE MCU CLOCK SYSTEM.
 * @param: None.
 * @return: None.
 */
void RCC_Init(void) {

	RCC -> CR |= 0x00000001; // Enable HSI (X3 is not mounted on the Nucleo-F746ZG) (HSION = '1').
	RCC -> CFGR &= 0xFFFFFFFC; // Select HSI as system clock (SW = '00') -> SYSCLK = 16 MHz.

#ifdef OUTPUT_CLOCK
	// Output HSI on MCO1 (PA8 as AF0) and SYSCLK on MCO2 (PC9 as AF0).
	// Prescaler = 4 so that measured frequency is 4 MHz.
	RCC -> CFGR &= 0x369FFFFF;
	RCC -> CFGR |= 0x36000000;
#endif

	RCC -> AHB1ENR |= 0x000007FF; // Enable GPIO clocks (GPIOxEN = '1').
	RCC -> BDCR |= 0x00000001; // Enable LSE (X2) (LSEON = '1').
	RCC -> CSR &= 0xFFFFFFFE; // Disable LSI (LSION = '0').
}
