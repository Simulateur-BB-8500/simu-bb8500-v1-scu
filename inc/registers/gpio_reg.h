/*
 * gpio_reg.h
 *
 *  Created on: 6 sept. 2017
 *      Author: Ludovic
 */

#ifndef REGISTERS_GPIO_REG_H_
#define REGISTERS_GPIO_REG_H_

/* GPIO registers */

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
} GPIO_Struct;

/* GPIO addresses */

#define GPIOA	((GPIO_Struct*) ((unsigned int) 0x40020000))
#define GPIOB	((GPIO_Struct*) ((unsigned int) 0x40020400))
#define GPIOC	((GPIO_Struct*) ((unsigned int) 0x40020800))
#define GPIOD	((GPIO_Struct*) ((unsigned int) 0x40020C00))
#define GPIOE	((GPIO_Struct*) ((unsigned int) 0x40021000))
#define GPIOF	((GPIO_Struct*) ((unsigned int) 0x40021400))
#define GPIOG	((GPIO_Struct*) ((unsigned int) 0x40021800))
#define GPIOH	((GPIO_Struct*) ((unsigned int) 0x40021C00))
#define GPIOI	((GPIO_Struct*) ((unsigned int) 0x40022000))
#define GPIOJ	((GPIO_Struct*) ((unsigned int) 0x40022400))
#define GPIOK	((GPIO_Struct*) ((unsigned int) 0x40022800))

#endif /* REGISTERS_GPIO_REG_H_ */
