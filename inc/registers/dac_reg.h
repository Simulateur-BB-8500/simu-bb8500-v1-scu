/*
 * dac_reg.h
 *
 *  Created on: 18 sept. 2017
 *      Author: Ludo
 */

#ifndef __DAC_REG_H__
#define __DAC_REG_H__

/*** DAC registers ***/

typedef struct {
	volatile unsigned int CR;    		// DAC control register.
	volatile unsigned int SWTRIGR;    	// DAC software trigger register.
	volatile unsigned int DHR12R1;    	// DAC channel 1 12-bits right-aligned data holding register.
	volatile unsigned int DHR12L1;    	// DAC channel 1 12-bits left-aligned data holding register.
	volatile unsigned int DHR8R1;    	// DAC channel 1 8-bits right-aligned data holding register.
	volatile unsigned int DHR12R2;    	// DAC channel 2 12-bits right-aligned data holding register.
	volatile unsigned int DHR12L2;    	// DAC channel 2 12-bits left-aligned data holding register.
	volatile unsigned int DHR8R2;    	// DAC channel 2 8-bits right-aligned data holding register.
	volatile unsigned int DHR12RD;    	// Dual DAC 12-bits right-aligned data holding register.
	volatile unsigned int DHR12LD;    	// Dual DAC 12-bits left-aligned data holding register.
	volatile unsigned int DHR8RD;    	// Dual DAC 8-bits right-aligned data holding register.
	unsigned int DOR1;    				// DAC channel 1 data output register.
	unsigned int DOR2;    				// DAC channel 2 data output register.
	volatile unsigned int SR;    		// DAC status register.
} DAC_registers_t;

/*** DAC base addresses ***/

#define DAC	((DAC_registers_t*) ((unsigned int) 0x40007400))

#endif /* __DAC_REG_H__ */
