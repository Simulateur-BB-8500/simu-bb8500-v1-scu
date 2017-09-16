/*
 * tim_reg.h
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludovic
 */

#ifndef REGISTERS_TIM_REG_H_
#define REGISTERS_TIM_REG_H_

/*** TIMx registers ***/

typedef struct {
  volatile unsigned int CR1;    	// TIM1/TIM8 control register 1.
  volatile unsigned int CR2;    	// TIM1/TIM8 control register 2.
  volatile unsigned int SMCR;    	// TIM1/TIM8 slave mode controler register.
  volatile unsigned int DIER;    	// TIM1/TIM8 DMA interrupt enable register.
  volatile unsigned int SR;    		// TIM1/TIM8 status register.
  volatile unsigned int EGR	;    	// TIM1/TIM8 event generation register.
  volatile unsigned int CCMR1;    	// TIM1/TIM8 capture/compare mode register 1.
  volatile unsigned int CCMR2;    	// TIM1/TIM8 capture/compare mode register 2.
  volatile unsigned int CCER;    	// TIM1/TIM8 capture/compare enable register.
  volatile unsigned int CNT;    	// TIM1/TIM8 counter register.
  volatile unsigned int PSC;    	// TIM1/TIM8 prescaler register.
  volatile unsigned int ARR;    	// TIM1/TIM8 auto-reload register.
  volatile unsigned int RCR;    	// TIM1/TIM8 repetition counter register.
  volatile unsigned int CCR1;    	// TIM1/TIM8 capture/compare register 1.
  volatile unsigned int CCR2;    	// TIM1/TIM8 capture/compare register 2.
  volatile unsigned int CCR3;    	// TIM1/TIM8 capture/compare register 3.
  volatile unsigned int CCR4;    	// TIM1/TIM8 capture/compare register 4.
  volatile unsigned int BDTR;    	// TIM1/TIM8 break and dead-time register.
  volatile unsigned int DCR;    	// TIM1/TIM8 DMA control register.
  volatile unsigned int DMAR;    	// TIM1/TIM8 DMA address for full transfer register.
  volatile unsigned int CCMR3O;    	// TIM1/TIM8 output capture/compare mode register 3.
  volatile unsigned int CCR5;    	// TIM1/TIM8 capture/compare register 5.
  volatile unsigned int CCR6;    	// TIM1/TIM8 capture/compare register 6.
} TIM1_TIM8_BaseAddress;

typedef struct {
  volatile unsigned int CR1;    	// TIM2/TIM5 control register 1.
  volatile unsigned int CR2;    	// TIM2/TIM5 control register 2.
  volatile unsigned int SMCR;    	// TIM2/TIM5 slave mode controler register.
  volatile unsigned int DIER;    	// TIM2/TIM5 DMA interrupt enable register.
  volatile unsigned int SR;    		// TIM2/TIM5 status register.
  volatile unsigned int EGR	;    	// TIM2/TIM5 event generation register.
  volatile unsigned int CCMR1;    	// TIM2/TIM5 capture/compare mode register 1.
  volatile unsigned int CCMR2;    	// TIM2/TIM5 capture/compare mode register 2.
  volatile unsigned int CCER;    	// TIM2/TIM5 capture/compare enable register.
  volatile unsigned int CNT;    	// TIM2/TIM5 counter register.
  volatile unsigned int PSC;    	// TIM2/TIM5 prescaler register.
  volatile unsigned int ARR;    	// TIM2/TIM5 auto-reload register.
  unsigned int RESERVED0;  			// Reserved 0x30.
  volatile unsigned int CCR1;    	// TIM2/TIM5 capture/compare register 1.
  volatile unsigned int CCR2;    	// TIM2/TIM5 capture/compare register 2.
  volatile unsigned int CCR3;    	// TIM2/TIM5 capture/compare register 3.
  volatile unsigned int CCR4;    	// TIM2/TIM5 capture/compare register 4.
  unsigned int RESERVED1;  			// Reserved 0x44.
  volatile unsigned int DCR;    	// TIM2/TIM5 DMA control register.
  volatile unsigned int DMAR;    	// TIM2/TIM5 DMA address for full transfer register.
  volatile unsigned int OR;    		// TIM2/TIM5 option register.
} TIM2_TIM5_BaseAddress;

typedef struct {
  volatile unsigned int CR1;    	// TIM3/TIM4 control register 1.
  volatile unsigned int CR2;    	// TIM3/TIM4 control register 2.
  volatile unsigned int SMCR;    	// TIM3/TIM4slave mode controler register.
  volatile unsigned int DIER;    	// TIM3/TIM4 DMA interrupt enable register.
  volatile unsigned int SR;    		// TIM3/TIM4 status register.
  volatile unsigned int EGR	;    	// TIM3/TIM4 event generation register.
  volatile unsigned int CCMR1;    	// TIM3/TIM4 capture/compare mode register 1.
  volatile unsigned int CCMR2;    	// TIM3/TIM4 capture/compare mode register 2.
  volatile unsigned int CCER;    	// TIM3/TIM4 capture/compare enable register.
  volatile unsigned int CNT;    	// TIM3/TIM4 counter register.
  volatile unsigned int PSC;    	// TIM3/TIM4 prescaler register.
  volatile unsigned int ARR;    	// TIM3/TIM4 auto-reload register.
  unsigned int RESERVED0;  			// Reserved 0x30.
  volatile unsigned int CCR1;    	// TIM3/TIM4 capture/compare register 1.
  volatile unsigned int CCR2;    	// TIM3/TIM4 capture/compare register 2.
  volatile unsigned int CCR3;    	// TIM3/TIM4 capture/compare register 3.
  volatile unsigned int CCR4;    	// TIM3/TIM4 capture/compare register 4.
  unsigned int RESERVED1;  			// Reserved 0x44.
  volatile unsigned int DCR;    	// TIM3/TIM4 DMA control register.
  volatile unsigned int DMAR;    	// TIM3/TIM4 DMA address for full transfer register.
} TIM3_TIM4_BaseAddress;

typedef struct {
  volatile unsigned int CR1;    	// TIM6/TIM7 control register 1.
  volatile unsigned int CR2;    	// TIM6/TIM7 control register 2.
  unsigned int RESERVED0;  			// Reserved 0x08.
  volatile unsigned int DIER;    	// TIM6/TIM7 DMA interrupt enable register.
  volatile unsigned int SR;    		// TIM6/TIM7 status register.
  volatile unsigned int EGR	;    	// TIM6/TIM7 event generation register.
  unsigned int RESERVED1;  			// Reserved 0x18.
  unsigned int RESERVED2;  			// Reserved 0x1C.
  unsigned int RESERVED3;  			// Reserved 0x20.
  volatile unsigned int CNT;    	// TIM6/TIM7 counter register.
  volatile unsigned int PSC;    	// TIM6/TIM7 prescaler register.
  volatile unsigned int ARR;    	// TIM6/TIM7 auto-reload register.
} TIM6_TIM7_BaseAddress;

typedef struct {
  volatile unsigned int CR1;    	// TIM9/TIM12 control register 1.
  unsigned int RESERVED0;    		// Reserved 0x04.
  volatile unsigned int SMCR;    	// TIM9/TIM12 slave mode controler register.
  volatile unsigned int DIER;    	// TIM9/TIM12 DMA interrupt enable register.
  volatile unsigned int SR;    		// TIM9/TIM12 status register.
  volatile unsigned int EGR	;    	// TIM9/TIM12 event generation register.
  volatile unsigned int CCMR1;    	// TIM9/TIM12 capture/compare mode register 1.
  unsigned int RESERVED1;    		// Reserved 0x1C.
  volatile unsigned int CCER;    	// TIM9/TIM12 capture/compare enable register.
  volatile unsigned int CNT;    	// TIM9/TIM12 counter register.
  volatile unsigned int PSC;    	// TIM9/TIM12 prescaler register.
  volatile unsigned int ARR;    	// TIM9/TIM12 auto-reload register.
  unsigned int RESERVED2;    		// Reserved 0x30.
  volatile unsigned int CCR1;    	// TIM9/TIM12 capture/compare register 1.
  volatile unsigned int CCR2;    	// TIM9/TIM12 capture/compare register 2.
} TIM9_TIM12_BaseAddress;

typedef struct {
  volatile unsigned int CR1;    	// TIM10/TIM11/TIM13/TIM14 control register 1.
  unsigned int RESERVED0;    		// Reserved 0x04.
  volatile unsigned int SMCR;    	// TIM10/TIM11/TIM13/TIM14 slave mode controler register.
  volatile unsigned int DIER;    	// TIM10/TIM11/TIM13/TIM14 DMA interrupt enable register.
  volatile unsigned int SR;    		// TIM10/TIM11/TIM13/TIM14 status register.
  volatile unsigned int EGR	;    	// TIM10/TIM11/TIM13/TIM14 event generation register.
  volatile unsigned int CCMR1;    	// TIM10/TIM11/TIM13/TIM14 capture/compare mode register 1.
  unsigned int RESERVED1;    		// Reserved 0x1C.
  volatile unsigned int CCER;    	// TIM10/TIM11/TIM13/TIM14 capture/compare enable register.
  volatile unsigned int CNT;    	// TIM10/TIM11/TIM13/TIM14 counter register.
  volatile unsigned int PSC;    	// TIM10/TIM11/TIM13/TIM14 prescaler register.
  volatile unsigned int ARR;    	// TIM10/TIM11/TIM13/TIM14 auto-reload register.
  unsigned int RESERVED2;    		// Reserved 0x30.
  volatile unsigned int CCR1;    	// TIM10/TIM11/TIM13/TIM14 capture/compare register 1.
  unsigned int RESERVED3;    		// Reserved 0x38.
  unsigned int RESERVED4;    		// Reserved 0x3C.
  unsigned int RESERVED5;    		// Reserved 0x40.
  unsigned int RESERVED6;    		// Reserved 0x44.
  unsigned int RESERVED7;    		// Reserved 0x48.
  unsigned int RESERVED8;    		// Reserved 0x4C.
  volatile unsigned int OR;    		// TIM10/TIM11/TIM13/TIM14 option register.
} TIM10_TIM11_TIM13_TIM14_BaseAddress;

/*** TIMx base addresses ***/

#define TIM1	((TIM1_TIM8_BaseAddress*) ((unsigned int) 0x40010000))
#define TIM2	((TIM2_TIM5_BaseAddress*) ((unsigned int) 0x40000000))
#define TIM3	((TIM3_TIM4_BaseAddress*) ((unsigned int) 0x40000400))
#define TIM4	((TIM3_TIM4_BaseAddress*) ((unsigned int) 0x40000800))
#define TIM5	((TIM2_TIM5_BaseAddress*) ((unsigned int) 0x40000C00))
#define TIM6	((TIM6_TIM7_BaseAddress*) ((unsigned int) 0x40001000))
#define TIM7	((TIM6_TIM7_BaseAddress*) ((unsigned int) 0x40001400))
#define TIM8	((TIM1_TIM8_BaseAddress*) ((unsigned int) 0x40010400))
#define TIM9	((TIM9_TIM12_BaseAddress*) ((unsigned int) 0x40014000))
#define TIM10	((TIM10_TIM11_TIM13_TIM14_BaseAddress*) ((unsigned int) 0x40014400))
#define TIM11	((TIM10_TIM11_TIM13_TIM14_BaseAddress*) ((unsigned int) 0x40014800))
#define TIM12	((TIM9_TIM12_BaseAddress*) ((unsigned int) 0x40001800))
#define TIM13	((TIM10_TIM11_TIM13_TIM14_BaseAddress*) ((unsigned int) 0x40001C00))
#define TIM14	((TIM10_TIM11_TIM13_TIM14_BaseAddress*) ((unsigned int) 0x40002000))

#endif /* REGISTERS_TIM_REG_H_ */
