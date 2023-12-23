/*
 * dac_reg.h
 *
 *  Created on: 18 sep. 2017
 *      Author: Ludo
 */

#ifndef __DAC_REG_H__
#define __DAC_REG_H__

#include "stdint.h"

/*** DAC REG macros ***/

// Peripheral base address.
#define DAC	((DAC_registers_t*) ((uint32_t) 0x40007400))

/*** DAC REG structures ***/

/*!******************************************************************
 * \enum DAC_registers_t
 * \brief DAC registers map.
 *******************************************************************/
typedef struct {
	volatile uint32_t CR;    		// DAC control register.
	volatile uint32_t SWTRIGR;    	// DAC software trigger register.
	volatile uint32_t DHR12R1;    	// DAC channel 1 12-bits right-aligned data holding register.
	volatile uint32_t DHR12L1;    	// DAC channel 1 12-bits left-aligned data holding register.
	volatile uint32_t DHR8R1;    	// DAC channel 1 8-bits right-aligned data holding register.
	volatile uint32_t DHR12R2;    	// DAC channel 2 12-bits right-aligned data holding register.
	volatile uint32_t DHR12L2;    	// DAC channel 2 12-bits left-aligned data holding register.
	volatile uint32_t DHR8R2;    	// DAC channel 2 8-bits right-aligned data holding register.
	volatile uint32_t DHR12RD;    	// Dual DAC 12-bits right-aligned data holding register.
	volatile uint32_t DHR12LD;    	// Dual DAC 12-bits left-aligned data holding register.
	volatile uint32_t DHR8RD;    	// Dual DAC 8-bits right-aligned data holding register.
	volatile uint32_t DOR1;    		// DAC channel 1 data output register.
	volatile uint32_t DOR2;    		// DAC channel 2 data output register.
	volatile uint32_t SR;    		// DAC status register.
} DAC_registers_t;

#endif /* __DAC_REG_H__ */
