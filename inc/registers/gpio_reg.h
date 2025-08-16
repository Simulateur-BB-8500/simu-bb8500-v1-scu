/*
 * gpio_reg.h
 *
 *  Created on: 06 sep. 2017
 *      Author: Ludo
 */

#ifndef __GPIO_REG_H__
#define __GPIO_REG_H__

#include "stdint.h"

/*** GPIO REG macros ***/

// Peripherals base address.
#define GPIOA   ((GPIO_registers_t*) ((uint32_t) 0x40020000))
#define GPIOB   ((GPIO_registers_t*) ((uint32_t) 0x40020400))
#define GPIOC   ((GPIO_registers_t*) ((uint32_t) 0x40020800))
#define GPIOD   ((GPIO_registers_t*) ((uint32_t) 0x40020C00))
#define GPIOE   ((GPIO_registers_t*) ((uint32_t) 0x40021000))
#define GPIOF   ((GPIO_registers_t*) ((uint32_t) 0x40021400))
#define GPIOG   ((GPIO_registers_t*) ((uint32_t) 0x40021800))
#define GPIOH   ((GPIO_registers_t*) ((uint32_t) 0x40021C00))
#define GPIOI   ((GPIO_registers_t*) ((uint32_t) 0x40022000))
#define GPIOJ   ((GPIO_registers_t*) ((uint32_t) 0x40022400))
#define GPIOK   ((GPIO_registers_t*) ((uint32_t) 0x40022800))

/*** GPIO REG structures ***/

/*!******************************************************************
 * \enum GPIO_registers_t
 * \brief GPIO registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
} GPIO_registers_t;

#endif /* __GPIO_REG_H__ */
