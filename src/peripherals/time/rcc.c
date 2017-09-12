/*
 * rcc.c
 *
 *  Created on: 9 sept. 2017
 *      Author: Ludovic
 */

#include "rcc.h"
#include "rcc_reg.h"

/* CONFIGURE MCU CLOCK SYSTEM.
 * @param: None.
 * @return: None.
 */
void RCC_Init(void) {

	// Enable HSI (X3 is not mounted on the Nucleo-F746ZG) (HSION = '1').
	RCC -> CR |= 0x00000001;

	// Select HSI as system clock (SW = '00').
	RCC -> CFGR &= 0xFFFFFFFC;

	// Enable GPIO clocks (GPIOxEN = '1').
	RCC -> AHB1ENR |= 0x000007FF;

	// Enable LSE (X2) (LSEON = '1').
	RCC -> BDCR |= 0x00000001;

	// Disable LSI (LSION = '0').
	RCC -> CSR &= 0xFFFFFFFE;
}
