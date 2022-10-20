/*
 * rcc.h
 *
 *  Created on: 9 sept. 2017
 *      Author: Ludo
 */

#ifndef __RCC_H__
#define __RCC_H__

/*** RCC structures ***/

typedef enum {
	RCC_CLOCK_SYSCLK,
	RCC_CLOCK_PCLK1,
	RCC_CLOCK_PCLK2,
	RCC_CLOCK_LAST
} RCC_clock_t;

/*** RCC macros ***/

// If defined, output clocks on MCO1 and MCO2 pins.
//#define RCC_OUTPUT_CLOCK

/*** RCC functions ***/

void RCC_init(void);
unsigned int RCC_get_clock_frequency(RCC_clock_t rcc_clock);

#endif /* __RCC_H__ */
