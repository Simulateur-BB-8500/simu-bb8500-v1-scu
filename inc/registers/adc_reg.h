/*
 * adc_reg.h
 *
 *  Created on: 19 sept. 2017
 *      Author: Ludo
 */

#ifndef __ADC_REG_H__
#define __ADC_REG_H__

#include "stdint.h"

/*** ADC registers ***/

typedef struct {
	volatile uint32_t SR;    	// ADC status register.
	volatile uint32_t CR1;    	// ADC control register 1.
	volatile uint32_t CR2;    	// ADC control register 2.
	volatile uint32_t SMPR1;    // ADC sample time register 1.
	volatile uint32_t SMPR2;    // ADC sample time register 2.
	volatile uint32_t JOFR1;    // ADC injected channel data offset register 1.
	volatile uint32_t JOFR2;    // ADC injected channel data offset register 2.
	volatile uint32_t JOFR3;    // ADC injected channel data offset register 3.
	volatile uint32_t JOFR4;    // ADC injected channel data offset register 4.
	volatile uint32_t HTR;    	// ADC watchdog higher threshold register.
	volatile uint32_t LTR;    	// ADC watchdog lower threshold register.
	volatile uint32_t SQR1;    	// ADC regular sequence register 1.
	volatile uint32_t SQR2;    	// ADC regular sequence register 2.
	volatile uint32_t SQR3;    	// ADC regular sequence register 3.
	volatile uint32_t JSQR;    	// ADC injected sequence register.
	volatile uint32_t JDR1;   	// ADC injected data register 1.
	volatile uint32_t JDR2;    	// ADC injected data register 2.
	volatile uint32_t JDR3;    	// ADC injected data register 3.
	volatile uint32_t JDR4;    	// ADC injected data register 4.
	volatile uint32_t DR;    	// ADC regular data register.
} ADC_registers_t;

/*** Common ADC registers ***/

typedef struct {
	volatile uint32_t CSR;    	// ADC common status register.
	volatile uint32_t CCR;    	// ADC common control register.
	volatile uint32_t CDR;    	// ADC common regular data register.
} ADC_common_registers_t;

/*** ADC base addresses ***/

#define ADC1	((ADC_registers_t*) ((uint32_t) 0x40012000))
#define ADC2	((ADC_registers_t*) ((uint32_t) 0x40012100))
#define ADC3	((ADC_registers_t*) ((uint32_t) 0x40012200))
#define ADCCR	((ADC_common_registers_t*) ((uint32_t) 0x40012300))

#endif /* __ADC_REG_H__ */
