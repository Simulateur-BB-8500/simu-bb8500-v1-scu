/*
 * adc.c
 *
 *  Created on: 19 sep. 2017
 *      Author: Ludo
 */

#include "adc.h"

#include "adc_reg.h"
#include "lsmcu.h"
#include "rcc_reg.h"

/*** ADC local structures ***/

/*******************************************************************/
typedef enum {
	ADC_CHANNEL_ZPT	= 0,
	ADC_CHANNEL_PBL2 = 1,
	ADC_CHANNEL_FPB	= 2,
	ADC_CHANNEL_AM = 3,
	ADC_CHANNEL_FD = 5,
	ADC_CHANNEL_MPINV = 6,
	ADC_CHANNEL_WHISTLE = 7,
	ADC_CHANNEL_ZLFR = 9,
	ADC_CHANNEL_VREFINT = 17,
	ADC_CHANNEL_TMCU = 18,
	ADC_CHANNEL_LAST = 19
} ADC_channel_t;

/*** ADC external global variables ***/

extern LSMCU_context_t lsmcu_ctx;

/*** ADC functions ***/

/*******************************************************************/
void ADC1_init(void) {
	// Local variables.
	uint8_t idx = 0;
	// Reset data.
	for (idx=0 ; idx<ADC_DATA_INDEX_LAST ; idx++) {
		lsmcu_ctx.adc_data[idx] = 0;
	}
	// Enable peripheral clock.
	RCC -> APB2ENR |= (0b1 << 8);
	// Select trigger.
	ADC1 -> CR2 |= (0b01 << 28); // Enable rising edge external trigger.
	ADC1 -> CR2 |= (0b1001 << 24); // Select TIM1_TRGO.
	// Maximum sampling time (SMPx='111').
	ADC1 -> SMPR1 |= 0x07FFFFFF;
	ADC1 -> SMPR2 |= 0x3FFFFFFF;
	// Regular sequence.
	ADC1 -> SQR1 |= ((ADC_DATA_INDEX_LAST - 1) << 20);
	ADC1 -> SQR3 |= (ADC_CHANNEL_ZPT << 0);
	ADC1 -> SQR3 |= (ADC_CHANNEL_PBL2 << 5);
	ADC1 -> SQR3 |= (ADC_CHANNEL_FPB << 10);
	ADC1 -> SQR3 |= (ADC_CHANNEL_AM << 15);
	ADC1 -> SQR3 |= (ADC_CHANNEL_FD << 20);
	ADC1 -> SQR3 |= (ADC_CHANNEL_MPINV << 25);
	ADC1 -> SQR2 |= (ADC_CHANNEL_WHISTLE << 0);
	ADC1 -> SQR2 |= (ADC_CHANNEL_ZLFR << 5);
	// Configure peripheral.
	ADC1 -> CR1 |= (0b1 << 8); // Scan mode.
	ADC1 -> CR2 |= (0b11 << 8); // Enable continuous DMA requests.
	// Enable ADC.
	ADC1 -> CR2 |= (0b1 << 0); // ADON='1'.
}
