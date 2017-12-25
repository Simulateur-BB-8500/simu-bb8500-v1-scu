/*
 * mapping.h
 *
 *  Created on: 14 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_GPIO_MAPPING_H_
#define PERIPHERALS_GPIO_MAPPING_H_

#include "adc.h"
#include "adc_reg.h"
#include "enum.h"
#include "gpio.h"
#include "gpio_reg.h"
#include "rcc.h"

/*** Peripherals mapping ***/

// RCC.
#ifdef OUTPUT_CLOCK
#define MCO1			(&((GPIO_Struct) {GPIOA, 8}))
#define MCO2			(&((GPIO_Struct) {GPIOC, 9}))
#endif
// DAC.
#define DAC1			(&((GPIO_Struct) {GPIOA, 4}))
#define DAC2			(&((GPIO_Struct) {GPIOA, 5}))
// ADC1, ADC2 and ADC3.
#define ADC123_IN0		(&((GPIO_Struct) {GPIOA, 0}))
#define ADC123_IN1		(&((GPIO_Struct) {GPIOA, 1}))
#define ADC123_IN2		(&((GPIO_Struct) {GPIOA, 2}))
#define ADC123_IN3		(&((GPIO_Struct) {GPIOA, 3}))
#define ADC123_IN10		(&((GPIO_Struct) {GPIOC, 0}))
#define ADC123_IN11		(&((GPIO_Struct) {GPIOC, 1}))
#define ADC123_IN12		(&((GPIO_Struct) {GPIOC, 2}))
#define ADC123_IN13		(&((GPIO_Struct) {GPIOC, 3}))
// ADC1 and ADC2.
#define ADC12_IN4		(&((GPIO_Struct) {GPIOA, 4}))
#define ADC12_IN5		(&((GPIO_Struct) {GPIOA, 5}))
#define ADC12_IN6		(&((GPIO_Struct) {GPIOA, 6}))
#define ADC12_IN7		(&((GPIO_Struct) {GPIOA, 7}))
#define ADC12_IN8		(&((GPIO_Struct) {GPIOB, 0}))
#define ADC12_IN9		(&((GPIO_Struct) {GPIOB, 1}))
#define ADC12_IN14		(&((GPIO_Struct) {GPIOC, 4}))
#define ADC12_IN15		(&((GPIO_Struct) {GPIOC, 5}))
// ADC3.
#define ADC3_IN4		(&((GPIO_Struct) {GPIOF, 6}))
#define ADC3_IN5		(&((GPIO_Struct) {GPIOF, 7}))
#define ADC3_IN6		(&((GPIO_Struct) {GPIOF, 8}))
#define ADC3_IN7		(&((GPIO_Struct) {GPIOF, 9}))
#define ADC3_IN8		(&((GPIO_Struct) {GPIOF, 10}))
#define ADC3_IN9		(&((GPIO_Struct) {GPIOF, 3}))
#define ADC3_IN14		(&((GPIO_Struct) {GPIOF, 4}))
#define ADC3_IN15		(&((GPIO_Struct) {GPIOF, 5}))
// USART1.
#define USART1_TX		(&((GPIO_Struct) {GPIOB, 6})) // Can be PA9.
#define USART1_RX		(&((GPIO_Struct) {GPIOA, 10})) // Can be PB7.
#define USART1_CTS		(&((GPIO_Struct) {GPIOA, 11}))
#define USART1_RTS		(&((GPIO_Struct) {GPIOA, 12}))
#define USART1_CK		(&((GPIO_Struct) {GPIOA, 8}))
// USART2.
#define USART2_TX		(&((GPIO_Struct) {GPIOD, 5})) // Can be PA2.
#define USART2_RX		(&((GPIO_Struct) {GPIOD, 6})) // Can be PA3.
#define USART2_CTS		(&((GPIO_Struct) {GPIOA, 0})) // Can be PD3.
#define USART2_RTS		(&((GPIO_Struct) {GPIOA, 1})) // Can be PD4.
#define USART2_CK		(&((GPIO_Struct) {GPIOA, 4}))
// USART3.
#define USART3_TX		(&((GPIO_Struct) {GPIOB, 10})) // Can be PC10 or PD8.
#define USART3_RX		(&((GPIO_Struct) {GPIOB, 11})) // Can be PC11 or PD9.
#define USART3_CTS		(&((GPIO_Struct) {GPIOB, 13})) // Can be PD11.
#define USART3_RTS		(&((GPIO_Struct) {GPIOB, 14})) // Can be PD12.
#define USART3_CK		(&((GPIO_Struct) {GPIOB, 12})) // Can be PC12 or PD10.
// UART4.
#define UART4_TX		(&((GPIO_Struct) {GPIOA, 0})) // Can be PC10.
#define UART4_RX		(&((GPIO_Struct) {GPIOA, 1})) // Can be PC11.
#define UART4_CTS		(&((GPIO_Struct) {GPIOB, 0}))
#define UART4_RTS		(&((GPIO_Struct) {GPIOA, 15}))
// UART5.
#define UART5_TX		(&((GPIO_Struct) {GPIOC, 12}))
#define UART5_RX		(&((GPIO_Struct) {GPIOD, 2}))
#define UART5_CTS		(&((GPIO_Struct) {GPIOC, 9}))
#define UART5_RTS		(&((GPIO_Struct) {GPIOC, 8}))
// USART6.
#define USART6_TX		(&((GPIO_Struct) {GPIOC, 6})) // Can be PG14.
#define USART6_RX		(&((GPIO_Struct) {GPIOC, 7})) // Can be PG9.
#define USART6_CTS		(&((GPIO_Struct) {GPIOG, 13})) // Can be PG15.
#define USART6_RTS		(&((GPIO_Struct) {GPIOG, 8})) // Can be PG12.
#define USART6_CK		(&((GPIO_Struct) {GPIOC, 8})) // Can be PG7.
// UART7.
#define UART7_TX		(&((GPIO_Struct) {GPIOE, 8})) // Can be PF7.
#define UART7_RX		(&((GPIO_Struct) {GPIOE, 7})) // Can be PF6.
#define UART7_CTS		(&((GPIO_Struct) {GPIOE, 10})) // Can be PF9.
#define UART7_RTS		(&((GPIO_Struct) {GPIOE, 9})) // Can be PF8.
// UART8.
#define UART8_TX		(&((GPIO_Struct) {GPIOE, 1}))
#define UART8_RX		(&((GPIO_Struct) {GPIOE, 0}))
#define UART8_CTS		(&((GPIO_Struct) {GPIOD, 14}))
#define UART8_RTS		(&((GPIO_Struct) {GPIOD, 15}))

/*** SCU mapping ***/

// Debug.
#define LED1 			(&((GPIO_Struct) {GPIOC, 0}))
#define LED2			(&((GPIO_Struct) {GPIOC, 3}))
#define LED3			(&((GPIO_Struct) {GPIOD, 7}))
#define BUTTON			(&((GPIO_Struct) {GPIOA, 3}))
// ZPT
#define ZPT				ADC12_IN8
#define ZPT_ADC			(&((ADC_Struct) {ADC1, ADC_Channel8}))
// PBL2
#define PBL2			ADC12_IN9
#define	PBL2_ADC		(&((ADC_Struct) {ADC1, ADC_Channel9}))
// Ampèremètres moteurs.
#define AM_OUTPUT		DAC1
// USART.
#define SGKCU_TX		USART2_TX
#define SGKCU_RX		USART2_RX

#endif /* PERIPHERALS_GPIO_MAPPING_H_ */
