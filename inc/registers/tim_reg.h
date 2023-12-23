/*
 * tim_reg.h
 *
 *  Created on: 16 sep. 2017
 *      Author: Ludo
 */

#ifndef __TIM_REG_H__
#define __TIM_REG_H__

#include "stdint.h"

/*** TIM REG macros ***/

// Peripherals base address.
#define TIM1	((TIM_registers_t*) ((uint32_t) 0x40010000))
#define TIM2	((TIM_registers_t*) ((uint32_t) 0x40000000))
#define TIM3	((TIM_registers_t*) ((uint32_t) 0x40000400))
#define TIM4	((TIM_registers_t*) ((uint32_t) 0x40000800))
#define TIM5	((TIM_registers_t*) ((uint32_t) 0x40000C00))
#define TIM6	((TIM_registers_t*) ((uint32_t) 0x40001000))
#define TIM7	((TIM_registers_t*) ((uint32_t) 0x40001400))
#define TIM8	((TIM_registers_t*) ((uint32_t) 0x40010400))
#define TIM9	((TIM_registers_t*) ((uint32_t) 0x40014000))
#define TIM10	((TIM_registers_t*) ((uint32_t) 0x40014400))
#define TIM11	((TIM_registers_t*) ((uint32_t) 0x40014800))
#define TIM12	((TIM_registers_t*) ((uint32_t) 0x40001800))
#define TIM13	((TIM_registers_t*) ((uint32_t) 0x40001C00))
#define TIM14	((TIM_registers_t*) ((uint32_t) 0x40002000))

/*** TIM REG structures ***/

/*!******************************************************************
 * \enum TIM_registers_t
 * \brief TIM registers map.
 *******************************************************************/
typedef struct {
	volatile uint32_t CR1;    	// Control register 1.
	volatile uint32_t CR2;    	// Control register 2.
	volatile uint32_t SMCR;    	// Slave mode controller register.
	volatile uint32_t DIER;    	// DMA interrupt enable register.
	volatile uint32_t SR;    	// Status register.
	volatile uint32_t EGR;    	// Event generation register.
	volatile uint32_t CCMR1;    // Capture/compare mode register 1.
	volatile uint32_t CCMR2;    // Capture/compare mode register 2.
	volatile uint32_t CCER;    	// Capture/compare enable register.
	volatile uint32_t CNT;    	// Counter register.
	volatile uint32_t PSC;    	// Prescaler register.
	volatile uint32_t ARR;    	// Auto-reload register.
	volatile uint32_t RCR;    	// Repetition counter register.
	volatile uint32_t CCR1;    	// Capture/compare register 1.
	volatile uint32_t CCR2;    	// Capture/compare register 2.
	volatile uint32_t CCR3;    	// Capture/compare register 3.
	volatile uint32_t CCR4;    	// Capture/compare register 4.
	volatile uint32_t BDTR;    	// Break and dead-time register.
	volatile uint32_t DCR;    	// DMA control register.
	volatile uint32_t DMAR;    	// DMA address for full transfer register.
	volatile uint32_t OR;    	// Option register.
	volatile uint32_t CCMR3;	// Output capture/compare mode register 3.
	volatile uint32_t CCR5;    	// Capture/compare register 5.
	volatile uint32_t CCR6;    	// Capture/compare register 6.
} TIM_registers_t;

#endif /* __TIM_REG_H__ */
