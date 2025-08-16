/*
 * tim_registers.h
 *
 *  Created on: 16 sep. 2017
 *      Author: Ludo
 */

#ifndef __TIM_REGISTERS_H__
#define __TIM_REGISTERS_H__

#include "stdint.h"

/*** TIM REGISTERS macros ***/

// Peripherals base address.
#define TIM1    ((TIM_registers_t*) ((uint32_t) 0x40010000))
#define TIM2    ((TIM_registers_t*) ((uint32_t) 0x40000000))
#define TIM3    ((TIM_registers_t*) ((uint32_t) 0x40000400))
#define TIM4    ((TIM_registers_t*) ((uint32_t) 0x40000800))
#define TIM5    ((TIM_registers_t*) ((uint32_t) 0x40000C00))
#define TIM6    ((TIM_registers_t*) ((uint32_t) 0x40001000))
#define TIM7    ((TIM_registers_t*) ((uint32_t) 0x40001400))
#define TIM8    ((TIM_registers_t*) ((uint32_t) 0x40010400))
#define TIM9    ((TIM_registers_t*) ((uint32_t) 0x40014000))
#define TIM10   ((TIM_registers_t*) ((uint32_t) 0x40014400))
#define TIM11   ((TIM_registers_t*) ((uint32_t) 0x40014800))
#define TIM12   ((TIM_registers_t*) ((uint32_t) 0x40001800))
#define TIM13   ((TIM_registers_t*) ((uint32_t) 0x40001C00))
#define TIM14   ((TIM_registers_t*) ((uint32_t) 0x40002000))

/*** TIM REGISTERS structures ***/

/*!******************************************************************
 * \enum TIM_registers_t
 * \brief TIM registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t BDTR;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
    volatile uint32_t OR;
    volatile uint32_t CCMR3;
    volatile uint32_t CCR5;
    volatile uint32_t CCR6;
} TIM_registers_t;

#endif /* __TIM_REGISTERS_H__ */
