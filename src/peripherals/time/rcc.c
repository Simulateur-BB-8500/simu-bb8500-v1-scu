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

	// Clock sources selection.
	RCC -> CR |= 0x00000001; // Enable HSI (X3 is not mounted on the Nucleo-F746ZG) (HSION = '1').
	RCC -> CFGR &= 0xFFFFFFFC; // Select HSI as system clock (SW = '00') -> SYSCLK = 16 MHz.
	RCC -> BDCR |= 0x00000001; // Enable LSE (X2) (LSEON = '1').
	RCC -> CSR &= 0xFFFFFFFE; // Disable LSI (LSION = '0').

#ifdef OUTPUT_CLOCK
	// Output HSI on MCO1 (PA8 as AF0) and SYSCLK on MCO2 (PC9 as AF0).
	// Prescaler = 4 so that output frequency is 4 MHz.
	RCC -> CFGR &= 0x369FFFFF;
	RCC -> CFGR |= 0x36000000;
#endif

	// Peripherals clocks prescaler.
	RCC -> CFGR &= 0xFFE0030F; // All prescalers = 1.

	// Enable peripherals clocks.
	RCC -> AHB1ENR |= 0x000007FF; // Enable GPIOx clock (GPIOxEN = '1').
	RCC -> APB1ENR |= 0x000001FF; // Enable TIM2-TIM7 and TIM12-14 clock (TIMxEN = '1').
	RCC -> APB2ENR |= 0x00070003; // Enable TIM1 and TIM8-11 clock (TIMxEN = '1').
}
