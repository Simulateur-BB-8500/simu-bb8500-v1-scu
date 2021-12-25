/*
 * rcc.h
 *
 *  Created on: 9 sept. 2017
 *      Author: Ludo
 */

#ifndef RCC_H
#define RCC_H

/*** RCC structures ***/

typedef enum {
	RCC_CLOCK_SYSCLK,
	RCC_CLOCK_PCLK1,
	RCC_CLOCK_PCLK2,
	RCC_CLOCK_LAST
} RCC_Clock;

/*** RCC macros ***/

// If defined, output clocks on MCO1 and MCO2 pins.
//#define RCC_OUTPUT_CLOCK

/*** RCC functions ***/

void RCC_Init(void);
unsigned int RCC_GetClockFrequency(RCC_Clock rcc_clock);

#endif /* RCC_H */
