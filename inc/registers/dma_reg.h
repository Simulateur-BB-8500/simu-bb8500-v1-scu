/*
 * dma_reg.h
 *
 *  Created on: 06 apr. 2023
 *      Author: Ludo
 */

#ifndef __DMA_REG_H__
#define __DMA_REG_H__

#include "stdint.h"

/*** DMA registers ***/

typedef struct {
	volatile uint32_t LISR;		// DMA low interrupt status register.
	volatile uint32_t HISR;		// DMA high interrupt status register.
	volatile uint32_t LIFCR;	// DMA low interrupt flag clear register.
	volatile uint32_t HIFCR;	// DMA high interrupt flag clear register.
	volatile uint32_t S0CR;		// DMA stream 0 configuration register.
	volatile uint32_t S0NDTR;	// DMA stream 0 number of data register.
	volatile uint32_t S0PAR;	// DMA stream 0 peripheral address register.
	volatile uint32_t S0M0AR;	// DMA stream 0 memory 0 address register.
	volatile uint32_t S0M1AR;	// DMA stream 0 memory 1 address register.
	volatile uint32_t S0FCR;	// DMA stream 0 FIFO control register.
	volatile uint32_t S1CR;		// DMA stream 1 configuration register.
	volatile uint32_t S1NDTR;	// DMA stream 1 number of data register.
	volatile uint32_t S1PAR;	// DMA stream 1 peripheral address register.
	volatile uint32_t S1M0AR;	// DMA stream 1 memory 0 address register.
	volatile uint32_t S1M1AR;	// DMA stream 1 memory 1 address register.
	volatile uint32_t S1FCR;	// DMA stream 1 FIFO control register.
	volatile uint32_t S2CR;		// DMA stream 2 configuration register.
	volatile uint32_t S2NDTR;	// DMA stream 2 number of data register.
	volatile uint32_t S2PAR;	// DMA stream 2 peripheral address register.
	volatile uint32_t S2M0AR;	// DMA stream 2 memory 0 address register.
	volatile uint32_t S2M1AR;	// DMA stream 2 memory 1 address register.
	volatile uint32_t S2FCR;	// DMA stream 2 FIFO control register.
	volatile uint32_t S3CR;		// DMA stream 3 configuration register.
	volatile uint32_t S3NDTR;	// DMA stream 3 number of data register.
	volatile uint32_t S3PAR;	// DMA stream 3 peripheral address register.
	volatile uint32_t S3M0AR;	// DMA stream 3 memory 0 address register.
	volatile uint32_t S3M1AR;	// DMA stream 3 memory 1 address register.
	volatile uint32_t S3FCR;	// DMA stream 3 FIFO control register.
	volatile uint32_t S4CR;		// DMA stream 4 configuration register.
	volatile uint32_t S4NDTR;	// DMA stream 4 number of data register.
	volatile uint32_t S4PAR;	// DMA stream 4 peripheral address register.
	volatile uint32_t S4M0AR;	// DMA stream 4 memory 0 address register.
	volatile uint32_t S4M1AR;	// DMA stream 4 memory 1 address register.
	volatile uint32_t S4FCR;	// DMA stream 4 FIFO control register.
	volatile uint32_t S5CR;		// DMA stream 5 configuration register.
	volatile uint32_t S5NDTR;	// DMA stream 5 number of data register.
	volatile uint32_t S5PAR;	// DMA stream 5 peripheral address register.
	volatile uint32_t S5M0AR;	// DMA stream 5 memory 0 address register.
	volatile uint32_t S5M1AR;	// DMA stream 5 memory 1 address register.
	volatile uint32_t S5FCR;	// DMA stream 5 FIFO control register.
	volatile uint32_t S6CR;		// DMA stream 6 configuration register.
	volatile uint32_t S6NDTR;	// DMA stream 6 number of data register.
	volatile uint32_t S6PAR;	// DMA stream 6 peripheral address register.
	volatile uint32_t S6M0AR;	// DMA stream 6 memory 0 address register.
	volatile uint32_t S6M1AR;	// DMA stream 6 memory 1 address register.
	volatile uint32_t S6FCR;	// DMA stream 6 FIFO control register.
	volatile uint32_t S7CR;		// DMA stream 7 configuration register.
	volatile uint32_t S7NDTR;	// DMA stream 7 number of data register.
	volatile uint32_t S7PAR;	// DMA stream 7 peripheral address register.
	volatile uint32_t S7M0AR;	// DMA stream 7 memory 0 address register.
	volatile uint32_t S7M1AR;	// DMA stream 7 memory 1 address register.
	volatile uint32_t S7FCR;	// DMA stream 7 FIFO control register.
} DMA_registers_t;

/*** DMA registers base address ***/

#define DMA1	((DMA_registers_t*) ((uint32_t) 0x40026000))
#define DMA2	((DMA_registers_t*) ((uint32_t) 0x40026400))

#endif /* __DMA_REG_H__ */
