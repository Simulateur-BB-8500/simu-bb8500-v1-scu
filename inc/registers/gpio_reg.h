/*
 * gpio_reg.h
 *
 *  Created on: 6 sept. 2017
 *      Author: Ludo
 */

#ifndef __GPIO_REG_H__
#define __GPIO_REG_H__

#include "types.h"

/*** GPIOx registers ***/

typedef struct {
	volatile uint32_t MODER;    	// GPIO port mode register.
	volatile uint32_t OTYPER;   	// GPIO port output type register.
	volatile uint32_t OSPEEDR;  	// GPIO port output speed register.
	volatile uint32_t PUPDR;    	// GPIO port pull-up/pull-down register.
	volatile uint32_t IDR;      	// GPIO port input data register.
	volatile uint32_t ODR;      	// GPIO port output data register.
	volatile uint32_t BSRR;    	// GPIO port bit set/reset low register.
	volatile uint32_t LCKR;     	// GPIO port configuration lock register.
	volatile uint32_t AFRL;   	// GPIO alternate function low register.
	volatile uint32_t AFRH;   	// GPIO alternate function high register.
} GPIO_registers_t;

/*** GPIOx base addresses ***/

#define GPIOA	((GPIO_registers_t*) ((uint32_t) 0x40020000))
#define GPIOB	((GPIO_registers_t*) ((uint32_t) 0x40020400))
#define GPIOC	((GPIO_registers_t*) ((uint32_t) 0x40020800))
#define GPIOD	((GPIO_registers_t*) ((uint32_t) 0x40020C00))
#define GPIOE	((GPIO_registers_t*) ((uint32_t) 0x40021000))
#define GPIOF	((GPIO_registers_t*) ((uint32_t) 0x40021400))
#define GPIOG	((GPIO_registers_t*) ((uint32_t) 0x40021800))
#define GPIOH	((GPIO_registers_t*) ((uint32_t) 0x40021C00))
#define GPIOI	((GPIO_registers_t*) ((uint32_t) 0x40022000))
#define GPIOJ	((GPIO_registers_t*) ((uint32_t) 0x40022400))
#define GPIOK	((GPIO_registers_t*) ((uint32_t) 0x40022800))

/*** GPIOx #define ***/

#define GPIO_PER_PORT 16 // Each port (A to K) has 16 GPIO.
#define AF_PER_GPIO 16 // Each GPIO has 16 alternate functions.
#define AFRH_OFFSET 8 // Limit between AFRL and AFRH registers.

#endif /* __GPIO_REG_H__ */
