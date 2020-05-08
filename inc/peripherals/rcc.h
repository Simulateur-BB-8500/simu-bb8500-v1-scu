/*
 * rcc.h
 *
 *  Created on: 9 sept. 2017
 *      Author: Ludo
 */

#ifndef RCC_H
#define RCC_H

/*** RCC macros ***/

// If defined, output clocks on MCO1 and MCO2 pins.
//#define RCC_OUTPUT_CLOCK
// System clocks frequency in kHz.
#define RCC_SYSCLK_KHZ	100000
#define RCC_PCLK1_KHZ	25000
#define RCC_PCLK2_KHZ	25000

/*** RCC functions ***/

void RCC_Init(void);

#endif /* RCC_H */
