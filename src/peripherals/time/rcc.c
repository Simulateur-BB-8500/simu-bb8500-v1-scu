/*
 * rcc.c
 *
 *  Created on: 9 sept. 2017
 *      Author: Ludovic
 */

#include "masks.h"
#include "rcc.h"
#include "rcc_reg.h"

/*** RCC functions ***/

/* CONFIGURE MCU CLOCK SYSTEM.
 * @param: None.
 * @return: None.
 */
void RCC_Init(void) {

	// Clock sources selection.
#ifdef USE_HSE

#else
	// Disable HSE.
	RCC -> CR &= ~BIT_MASK[16]; // Disable HSE (HSEON = '0').
	// Enable HSI.
	RCC -> CR |= BIT_MASK[0]; // Enable HSI (HSION = '1').
	// Wait for HSI to be stable.
	//while (((RCC -> CR) & BIT_MASK[1]) != 1); // Wait for HSIRDY = '1'.
	// Select HSI.
	RCC -> CFGR &= 0xFFFFFFFC; // HSI selected as system clock (SW = '00') -> SYSCLK = 16 MHz.
	// Wait for HSI to be selected.
	//while (((RCC -> CFGR) & 0x00000003) != 0); // Wait for SWS = '00'.
#endif

#ifdef USE_LSE
	RCC -> CSR &= ~BIT_MASK[0]; // Disable LSI (LSION = '0').
	RCC -> BDCR |= BIT_MASK[0]; // Enable LSE (X2) (LSEON = '1').
#else

#endif

#ifdef OUTPUT_CLOCK
	// Output HSI on MCO1 (PA8 as AF0) and SYSCLK on MCO2 (PC9 as AF0).
	// Prescaler = 4 so that output frequency is 4 MHz.
	RCC -> CFGR &= 0x369FFFFF;
	RCC -> CFGR |= 0x36000000;
#endif

	// Peripherals clock prescaler.
	RCC -> CFGR &= 0xFFE0030F; // All prescalers = 1.



	// Enable TIM clocks.
	RCC -> APB1ENR |= 0x000001FF; // Enable TIM2-TIM7 and TIM12-14 clock (TIMxEN = '1').
	RCC -> APB2ENR |= 0x00070003; // Enable TIM1 and TIM8-11 clock (TIMxEN = '1').

	// Enable DAC clock.
	RCC -> APB1ENR |= BIT_MASK[29]; // (DACEN = '1').

	// Enable all ADC clocks
	RCC -> APB2ENR |= 0x00000700; // (ADCxEN = '1').

	// Enable all USART clocks.
	RCC -> APB1ENR |= 0xC01E0000;
	RCC -> APB2ENR |= 0x00000030;
}
