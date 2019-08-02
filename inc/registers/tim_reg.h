/*
 * tim_reg.h
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludovic
 */

#ifndef TIM_REG_H
#define TIM_REG_H

/*** TIMx registers ***/

typedef struct {
	volatile unsigned int CR1;    	// Control register 1.
	volatile unsigned int CR2;    	// Control register 2 (!).
	volatile unsigned int SMCR;    	// Slave mode controler register (!)
	volatile unsigned int DIER;    	// DMA interrupt enable register.
	volatile unsigned int SR;    	// Status register.
	volatile unsigned int EGR;    	// Event generation register.
	volatile unsigned int CCMR1;    // Capture/compare mode register 1 (!).
	volatile unsigned int CCMR2;    // Capture/compare mode register 2 (!).
	volatile unsigned int CCER;    	// Capture/compare enable register (!).
	volatile unsigned int CNT;    	// Counter register.
	volatile unsigned int PSC;    	// Prescaler register.
	volatile unsigned int ARR;    	// Auto-reload register.
	volatile unsigned int RCR;    	// Repetition counter register (!).
	volatile unsigned int CCR1;    	// Capture/compare register 1 (!).
	volatile unsigned int CCR2;    	// Capture/compare register 2 (!).
	volatile unsigned int CCR3;    	// Capture/compare register 3 (!).
	volatile unsigned int CCR4;    	// Capture/compare register 4 (!).
	volatile unsigned int BDTR;    	// Break and dead-time register (!).
	volatile unsigned int DCR;    	// DMA control register (!).
	volatile unsigned int DMAR;    	// DMA address for full transfer register (!).
	volatile unsigned int OR;    	// Option register (!).
	volatile unsigned int CCMR3O;   // Output capture/compare mode register 3 (!).
	volatile unsigned int CCR5;    	// Capture/compare register 5 (!).
	volatile unsigned int CCR6;    	// Capture/compare register 6 (!).
} TIM_BaseAddress;

/* Registers marked (!) are not mapped in all peripherals.
When accessing those registers in a generic function, the timer number has to be check before. */

/*** TIMx base addresses ***/

#define TIM1	((TIM_BaseAddress*) ((unsigned int) 0x40010000))
#define TIM2	((TIM_BaseAddress*) ((unsigned int) 0x40000000))
#define TIM3	((TIM_BaseAddress*) ((unsigned int) 0x40000400))
#define TIM4	((TIM_BaseAddress*) ((unsigned int) 0x40000800))
#define TIM5	((TIM_BaseAddress*) ((unsigned int) 0x40000C00))
#define TIM6	((TIM_BaseAddress*) ((unsigned int) 0x40001000))
#define TIM7	((TIM_BaseAddress*) ((unsigned int) 0x40001400))
#define TIM8	((TIM_BaseAddress*) ((unsigned int) 0x40010400))
#define TIM9	((TIM_BaseAddress*) ((unsigned int) 0x40014000))
#define TIM10	((TIM_BaseAddress*) ((unsigned int) 0x40014400))
#define TIM11	((TIM_BaseAddress*) ((unsigned int) 0x40014800))
#define TIM12	((TIM_BaseAddress*) ((unsigned int) 0x40001800))
#define TIM13	((TIM_BaseAddress*) ((unsigned int) 0x40001C00))
#define TIM14	((TIM_BaseAddress*) ((unsigned int) 0x40002000))

#endif /* TIM_REG_H */
