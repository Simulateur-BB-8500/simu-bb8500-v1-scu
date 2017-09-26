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

// Output clock.
#ifdef OUTPUT_CLOCK
#define MCO1		&((GPIO_Struct) {GPIOA, 8})
#define MCO2		&((GPIO_Struct) {GPIOC, 9})
#endif
// DAC
#define DAC1		&((GPIO_Struct) {GPIOA, 4})
#define DAC2		&((GPIO_Struct) {GPIOA, 5})
// ADC
#define ADC123_IN0	&((GPIO_Struct) {GPIOA, 0})
#define ADC123_IN1	&((GPIO_Struct) {GPIOA, 1})
#define ADC123_IN2	&((GPIO_Struct) {GPIOA, 2})
#define ADC123_IN3	&((GPIO_Struct) {GPIOA, 3})
#define ADC123_IN10	&((GPIO_Struct) {GPIOC, 0})
#define ADC123_IN11	&((GPIO_Struct) {GPIOC, 1})
#define ADC123_IN12	&((GPIO_Struct) {GPIOC, 2})
#define ADC123_IN13	&((GPIO_Struct) {GPIOC, 3})
#define ADC12_IN4	&((GPIO_Struct) {GPIOA, 4})
#define ADC12_IN5	&((GPIO_Struct) {GPIOA, 5})
#define ADC12_IN6	&((GPIO_Struct) {GPIOA, 6})
#define ADC12_IN7	&((GPIO_Struct) {GPIOA, 7})
#define ADC12_IN8	&((GPIO_Struct) {GPIOB, 0})
#define ADC12_IN9	&((GPIO_Struct) {GPIOB, 1})
#define ADC12_IN14	&((GPIO_Struct) {GPIOC, 4})
#define ADC12_IN15	&((GPIO_Struct) {GPIOC, 5})
#define ADC3_IN4	&((GPIO_Struct) {GPIOF, 6})
#define ADC3_IN5	&((GPIO_Struct) {GPIOF, 7})
#define ADC3_IN6	&((GPIO_Struct) {GPIOF, 8})
#define ADC3_IN7	&((GPIO_Struct) {GPIOF, 9})
#define ADC3_IN8	&((GPIO_Struct) {GPIOF, 10})
#define ADC3_IN9	&((GPIO_Struct) {GPIOF, 3})
#define ADC3_IN14	&((GPIO_Struct) {GPIOF, 4})
#define ADC3_IN15	&((GPIO_Struct) {GPIOF, 5})

/*** SCU mapping ***/

#define LED1 		&((GPIO_Struct) {GPIOC, 0})
#define LED2		&((GPIO_Struct) {GPIOC, 3})
#define BUTTON		&((GPIO_Struct) {GPIOA, 3})
#define AMP			DAC1
#define ZPT			ADC123_IN0

#endif /* PERIPHERALS_GPIO_MAPPING_H_ */
