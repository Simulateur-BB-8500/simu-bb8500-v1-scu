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

/*** Nucleo-F746ZG mapping ***/

#ifdef OUTPUT_CLOCK
#define MCO1		&((GPIO_Struct) {GPIOA, 8})
#define MCO2		&((GPIO_Struct) {GPIOC, 9})
#endif
#define DAC1		&((GPIO_Struct) {GPIOA, 4})
#define DAC2		&((GPIO_Struct) {GPIOA, 5})

/*** SCU mapping ***/

#define LED1 		&((GPIO_Struct) {GPIOC, 0})
#define LED2		&((GPIO_Struct) {GPIOC, 3})
#define BUTTON		&((GPIO_Struct) {GPIOA, 3})
#define AMP			DAC1

#endif /* PERIPHERALS_GPIO_MAPPING_H_ */
