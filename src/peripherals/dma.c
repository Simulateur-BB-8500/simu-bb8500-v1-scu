/*
 * dma.c
 *
 *  Created on: 6 apr. 2023
 *      Author: Ludo
 */

#include "dma.h"

#include "adc_reg.h"
#include "dma_reg.h"
#include "lsmcu.h"
#include "rcc_reg.h"
#include "stdint.h"

/*** DMA external global variables ***/

extern LSMCU_context_t lsmcu_ctx;

/*** DMA functions ***/

/*******************************************************************/
void DMA2_STR0_init(void) {
	// Enable peripheral clock.
	RCC -> AHB1ENR |= (0b1 << 22); // DMA2EN='1'.
	// Disable DMA stream before configuration.
	DMA2 -> S0CR &= ~(0b1 << 0);
	// Wait for EN bit to be cleared.
	while (((DMA2 -> S0CR) & (0b1 << 0)) != 0);
	// Clear all flags.
	DMA2 -> LIFCR |= 0x0F7D0F7D;
	DMA2 -> HIFCR |= 0x0F7D0F7D;
	// Channel selection.
	DMA2 -> S0CR &= ~(0b0111 << 25); // Request mapping: DMA1 stream 0 channel 0 = ADC.
	// Stream 0 transfer settings.
	DMA2 -> S0CR &= ~(0b00 << 6); // Peripheral to memory.
	DMA2 -> S0CR |= (0b10 << 13) | (0b10 << 11); // Memory and peripheral size = 32bits (MSIZE='10' and PSIZE='10').
	DMA2 -> S0CR |= (0b1 << 10); // Memory pointer incremented according to MSIZE.
	DMA2 -> S0CR &= ~(0b1 << 9); // Peripheral address fixed (PINC='0').
	DMA2 -> S0CR &= ~(0b1 << 18); // Single-buffer mode.
	DMA2 -> S0CR |= (0b1 << 8); // Circular mode enabled (already set by hardware with double-buffer mode).
	// Set peripheral address.
	DMA2 -> S0PAR = (uint32_t) &(ADC1 -> DR);
	// Set memory address.
	DMA2 -> S0M0AR = (uint32_t) &(lsmcu_ctx.adc_data[0]);
	// Number of data to transfer.
	DMA2 -> S0NDTR = ADC_DATA_INDEX_LAST;
	// Start stream.
	DMA2 -> S0CR |= (0b1 << 0);
}
