/*
 * rcc.h
 *
 *  Created on: 9 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_TIME_RCC_H_
#define PERIPHERALS_TIME_RCC_H_

/*** RCC #define ***/

// Clock source selection.
//#define USE_HSE	// Use external high speed oscillator if defined, otherwise use the internal one (HSI).
#define USE_LSE		// Use external low speed oscillator if defined, otherwise use the internal one (LSI).

#define SYSCLK 16000000
//#define OUTPUT_CLOCK

/*** RCC functions ***/

void RCC_Init(void);

#endif /* PERIPHERALS_TIME_RCC_H_ */
