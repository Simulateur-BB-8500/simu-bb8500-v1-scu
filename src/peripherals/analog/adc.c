/*
 * adc.c
 *
 *  Created on: 19 sept. 2017
 *      Author: Ludovic
 */

#include "adc_reg.h"
#include "mapping.h"
#include "masks.h"
#include "types.h"

/*** ADC #define ***/

#define ADC_SQx_LENGTH 5

/*** ADC functions ***/

/* CONFIGURE ADC COMMON REGISTERS.
 * @param: None.
 * @return: None.
 */
void ADCCR_Init(void) {
	ADCCR -> CCR &= ~BIT_MASK[23] ; // Temperature sensor disabled (TSVREFE = '0').
	ADCCR -> CCR &= ~BIT_MASK[22] ; // Vbat channel disabled (VBATE = '0').
	ADCCR -> CCR &= 0xFFFCFFFF; // Prescaler = 2 (ADCPRE = '00').
	ADCCR -> CCR &= 0xFFFF2FFF; // DMA disabled (DMA = '00').
	ADCCR -> CCR &= 0xFFFFF0FF; // Delay between to sampling phases = 5*T (DELAY = '0000').
	ADCCR -> CCR &= 0xFFFFFFE0; // All ADC independent (MULTI = '00000').
}

/* CONFIGURE AN ADC.
 * @param ADC: ADC address (should be 'ADC1', 'ADC2' or 'ADC3').
 * @param resolution: ADC resolution in bits (should be 'bit6', 'bit8', bit10' or 'bit12').
 * @return: None.
 */
void ADC_Init(ADC_BaseAddress* ADC, ADCResolution resolution) {
	ADC -> CR1 &= ~BIT_MASK[8]; // Disable scan mode (SCAN = '0').
	ADC -> CR1 |= BIT_MASK[5]; // Enable end of conversion interrupt (EOCIE = '1').
	ADC -> CR2 |= BIT_MASK[10]; // EOC set at the end of each regular conversion (EOCS = '1').
	ADC -> CR2 &= ~BIT_MASK[1]; // Single conversion mode (CONT = '0').
	ADC -> SMPR1 &= 0xF7000000; // Sampling time = 3 cycles (SMPx = '000').
	ADC -> SMPR2 &= 0xC0000000; // Sampling time = 3 cycles (SMPx = '000').
	// Configure resolution.
	switch(resolution) {
	case bit6:
		ADC -> CR1 |= BIT_MASK[25];
		ADC -> CR1 |= BIT_MASK[24];
		break;
	case bit8:
		ADC -> CR1 |= BIT_MASK[25];
		ADC -> CR1 &= ~BIT_MASK[24];
		break;
	case bit10:
		ADC -> CR1 &= ~BIT_MASK[25];
		ADC -> CR1 |= BIT_MASK[24];
		break;
	case bit12:
		ADC -> CR1 &= ~BIT_MASK[25];
		ADC -> CR1 &= ~BIT_MASK[24];
		break;
	default:
		break;
	}
	// Regular sequence will always contain 1 channel.
	ADC -> SQR1 &= 0xFF000000;
	// Channel 0 selected by default.
	ADC -> SQR2 &= 0xC0000000;
	ADC -> SQR3 &= 0xC0000000;
	// Result in right alignement.
	ADC -> CR2 &= ~BIT_MASK[11]; // (ALIGN = '0').
	// Enable ADC.
	ADC -> CR2 |= BIT_MASK[0]; // (ADON = '1').
}

/* SET THE CURRENT CHANNEL OF AN ADC.
 * @param ADC: ADC address (should be 'ADC1', 'ADC2' or 'ADC3').
 * @param channel: ADC channel (should be 'ADCChannel0' to 'ADCChannel15', 'NC', 'VREF' or 'VBAT').
 * @return: None.
 */
void ADC_SetChannel(ADC_BaseAddress* ADC, ADCChannel channel) {
	unsigned int i = 0;
	for(i=0 ; i<ADC_SQx_LENGTH ; i++) {
		unsigned int channelMasked = channel & BIT_MASK[i];
		if (channelMasked == 0) {
			// Bit = '0'.
			ADC -> SQR3 &= ~BIT_MASK[i];
		}
		else {
			// Bit = '1'.
			ADC -> SQR3 |= BIT_MASK[i];
		}
	}
}

/* RETURNS THE MAXIMUM VALUE READ BY AN ADC (FOR Vin=Vcc).
 * @param ADC: ADC address (should be 'ADC1', 'ADC2' or 'ADC3').
 * @return result: Maximum ADC output value represented in decimal = 2^(resolution)-1.
 */
unsigned int ADC_GetFullScale(ADC_BaseAddress* ADC) {
	unsigned int result = 0;
	if ((ADC -> CR1 & BIT_MASK[25]) == 0) {
		if ((ADC -> CR1 & BIT_MASK[24]) == 0) {
			// RES = '00'.
			result = bit12;
		}
		else {
			// RES = '01'.
			result = bit10;
		}
	}
	else {
		if ((ADC -> CR1 & BIT_MASK[24]) == 0) {
			// RES = '10'.
			result = bit8;
		}
		else {
			// RES = '11'.
			result = bit6;
		}
	}
	return result;
}

/* START ONE ADC CONVERSION.
 * @param ADC: ADC address (should be 'ADC1', 'ADC2' or 'ADC3').
 * @return: None.
 */
void ADC_StartConversion(ADC_BaseAddress* ADC) {
	ADC -> CR2 |= BIT_MASK[30]; // Start conversion (SWSTART = '1').
}

/* RETURNS THE ADC OUTPUT VALUE.
 * @param ADC: ADC address (should be 'ADC1', 'ADC2' or 'ADC3').
 * @return: ADC conversion result represented in decimal.
 */
unsigned int ADC_GetVoltage(ADC_BaseAddress* ADC) {
	return (ADC -> DR);
}

/* ADC COMMON INTERRUPT HANDLER.
 * @param: None.
 * @return: None.
 */
void ADC_Handler(void) {
	unsigned int v = ADC_GetVoltage(ADC1);
	if (v > (ADC_GetFullScale(ADC1)/2)) {
		GPIO_Write(LED2, HIGH);
	}
	else {
		GPIO_Write(LED2, LOW);
	}
	// Start new conversion.
	ADC_StartConversion(ADC1);
}
