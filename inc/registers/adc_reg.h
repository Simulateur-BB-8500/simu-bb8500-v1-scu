/*
 * adc_reg.h
 *
 *  Created on: 19 sept. 2017
 *      Author: Ludovic
 */

#ifndef REGISTERS_ADC_REG_H_
#define REGISTERS_ADC_REG_H_

/*** ADC registers ***/

typedef struct {
	volatile unsigned int SR;    	// ADC status register.
	volatile unsigned int CR1;    	// ADC control register 1.
	volatile unsigned int CR2;    	// ADC control register 2.
	volatile unsigned int SMPR1;    // ADC sample time register 1.
	volatile unsigned int SMPR2;    // ADC sample time register 2.
	volatile unsigned int JOFR1;    // ADC injected channel data offset register 1.
	volatile unsigned int JOFR2;    // ADC injected channel data offset register 2.
	volatile unsigned int JOFR3;    // ADC injected channel data offset register 3.
	volatile unsigned int JOFR4;    // ADC injected channel data offset register 4.
	volatile unsigned int HTR;    	// ADC watchdog higher threshold register.
	volatile unsigned int LTR;    	// ADC watchdog lower threshold register.
	volatile unsigned int SQR1;    	// ADC regular sequence register 1.
	volatile unsigned int SQR2;    	// ADC regular sequence register 2.
	volatile unsigned int SQR3;    	// ADC regular sequence register 3.
	volatile unsigned int JSQR;    	// ADC injected sequence register.
	volatile unsigned int JDR1;   	// ADC injected data register 1.
	volatile unsigned int JDR2;    	// ADC injected data register 2.
	volatile unsigned int JDR3;    	// ADC injected data register 3.
	volatile unsigned int JDR4;    	// ADC injected data register 4.
	volatile unsigned int DR;    	// ADC regular data register.
} ADC_BaseAddress;

/*** Common ADC registers ***/

typedef struct {
	volatile unsigned int CSR;    	// ADC common status register.
	volatile unsigned int CCR;    	// ADC common control register.
	volatile unsigned int CDR;    	// ADC common regular data register.
} ADCCR_BaseAddress;

/*** ADC base addresses ***/

#define ADC1	((ADC_BaseAddress*) ((unsigned int) 0x40012000))
#define ADC2	((ADC_BaseAddress*) ((unsigned int) 0x40012100))
#define ADC3	((ADC_BaseAddress*) ((unsigned int) 0x40012200))
#define ADCCR	((ADCCR_BaseAddress*) ((unsigned int) 0x40012300))

#endif /* REGISTERS_ADC_REG_H_ */
