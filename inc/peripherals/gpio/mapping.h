/*
 * mapping.h
 *
 *  Created on: 14 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_GPIO_MAPPING_H_
#define PERIPHERALS_GPIO_MAPPING_H_

#include "gpio.h"
#include "gpio_reg.h"

/*** Nucleo-F746ZG mapping configuration ***/

#define LED 		&((GPIO_Struct) {GPIOC, 0})
#define BUTTON		&((GPIO_Struct) {GPIOA, 3})

#ifdef OUTPUT_CLOCK
#define MCO1		&((GPIO_Struct) {GPIOA, 8})
#define MCO2		&((GPIO_Struct) {GPIOC, 9})
#endif

#endif /* PERIPHERALS_GPIO_MAPPING_H_ */
