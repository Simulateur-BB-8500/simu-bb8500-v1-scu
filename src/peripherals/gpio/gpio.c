/*
 * gpio.c
 *
 *  Created on: 12 sept. 2017
 *      Author: Ludovic
 */

#include "gpio.h"
#include "gpio_reg.h"

/* CONFIGURE MCU GPIOs.
 * @param: None.
 * @return: None.
 */
void GPIO_Init(void) {

	// PA3 configuration.
	// Output (MODERA3 = '01').
	GPIOA -> MODER &= 0xFFFFFF7F;
	GPIOA -> MODER |= 0x00000040;
	// Push-pull (OTA3 = '0').
	GPIOA -> OTYPER &= 0xFFFFFFF7;
	// No pull-up or pull-down resistor (PUPDRA3 = '00').
	GPIOA ->PUPDR &= 0xFFFFFF3F;
}
