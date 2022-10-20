/*
 * gpio_reg.h
 *
 *  Created on: 6 sept. 2017
 *      Author: Ludo
 */

#ifndef __GPIO_REG_H__
#define __GPIO_REG_H__

/*** GPIOx registers ***/

typedef struct {
	volatile unsigned int MODER;    	// GPIO port mode register.
	volatile unsigned int OTYPER;   	// GPIO port output type register.
	volatile unsigned int OSPEEDR;  	// GPIO port output speed register.
	volatile unsigned int PUPDR;    	// GPIO port pull-up/pull-down register.
	volatile unsigned int IDR;      	// GPIO port input data register.
	volatile unsigned int ODR;      	// GPIO port output data register.
	volatile unsigned int BSRR;    	// GPIO port bit set/reset low register.
	volatile unsigned int LCKR;     	// GPIO port configuration lock register.
	volatile unsigned int AFRL;   	// GPIO alternate function low register.
	volatile unsigned int AFRH;   	// GPIO alternate function high register.
} GPIO_base_address_t;

/*** GPIOx base addresses ***/

#define GPIOA	((GPIO_base_address_t*) ((unsigned int) 0x40020000))
#define GPIOB	((GPIO_base_address_t*) ((unsigned int) 0x40020400))
#define GPIOC	((GPIO_base_address_t*) ((unsigned int) 0x40020800))
#define GPIOD	((GPIO_base_address_t*) ((unsigned int) 0x40020C00))
#define GPIOE	((GPIO_base_address_t*) ((unsigned int) 0x40021000))
#define GPIOF	((GPIO_base_address_t*) ((unsigned int) 0x40021400))
#define GPIOG	((GPIO_base_address_t*) ((unsigned int) 0x40021800))
#define GPIOH	((GPIO_base_address_t*) ((unsigned int) 0x40021C00))
#define GPIOI	((GPIO_base_address_t*) ((unsigned int) 0x40022000))
#define GPIOJ	((GPIO_base_address_t*) ((unsigned int) 0x40022400))
#define GPIOK	((GPIO_base_address_t*) ((unsigned int) 0x40022800))

/*** GPIOx #define ***/

#define GPIO_PER_PORT 16 // Each port (A to K) has 16 GPIO.
#define AF_PER_GPIO 16 // Each GPIO has 16 alternate functions.
#define AFRH_OFFSET 8 // Limit between AFRL and AFRH registers.

#endif /* __GPIO_REG_H__ */
