/*
 * main.c
 *
 *  Created on: 5 sept. 2017
 *      Author: Ludovic
 */

#include "gpio.h"
#include "gpio_reg.h"
#include "rcc.h"

/* MAIN FUNCTION.
 * @param: None.
 * @return: 0.
 */
int main(void) {

	// Peripherals initialisation.
	RCC_Init();
	GPIO_Init();

	// Global variables initialisation.
	int i = 0;

	// Main loop.
	while(1) {
		// Switch on the LED.
		GPIOA -> ODR |= 0x00000008;
		for(i=0 ; i<500000 ; i++);
		// Switch off the LED.
		GPIOA -> ODR &= 0xFFFFFFF7;
		for(i=0 ; i<500000 ; i++);
	}

	return 0;
}
