/*
 * adc.c
 *
 *  Created on: 19 sept. 2017
 *      Author: Ludovic
 */

#include "adc_reg.h"
#include "enum.h"
#include "mapping.h"
#include "nvic.h"
#include "rcc_reg.h"
#include "zpt.h"

/*** ADC #define ***/

#define ADC_SQx_LENGTH 5

/*** ADC global variables ***/

static ADC_StateMachine adcState = ADC_OFF;
static unsigned int currentVoltage = 0;
static boolean conversionCompleted = false;

/*** ADC internal functions ***/

/* ENABLE THE CORRESPONDING CLOCK OF AN ADC PERIPHERAL.
 * @param ADC: 	ADC base address (should be 'ADC1', 'ADC2' or 'ADC3').
 * @return:		None.
 */
void ADC_EnableClock(ADC_BaseAddress* ADC) {
	// Check peripheral address.
	switch ((unsigned int) ADC) {
	case ((unsigned int) ADC1):
		RCC -> APB2ENR |= BIT_MASK(8); // (ADC1EN = '1').
		break;
	case ((unsigned int) ADC2):
		RCC -> APB2ENR |= BIT_MASK(9); // (ADC2EN = '1').
		break;
	case ((unsigned int) ADC3):
		RCC -> APB2ENR |= BIT_MASK(10); // (ADC3EN = '1').
		break;
	default:
		break;
	}
}

/*** ADC functions ***/

/* CONFIGURE ADC COMMON REGISTERS.
 * @param: 	None.
 * @return: None.
 */
void ADCCR_Init(void) {
	// Enable interrupt.
	NVIC_EnableInterrupt(IT_ADC);
	// Common registers.
	ADCCR -> CCR &= ~BIT_MASK(23) ; // Temperature sensor disabled (TSVREFE = '0').
	ADCCR -> CCR &= ~BIT_MASK(22) ; // Vbat channel disabled (VBATE = '0').
	ADCCR -> CCR &= 0xFFFCFFFF; // Prescaler = 2 (ADCPRE = '00').
	ADCCR -> CCR &= 0xFFFF2FFF; // DMA disabled (DMA = '00').
	ADCCR -> CCR &= 0xFFFFF0FF; // Delay between to sampling phases = 5*T (DELAY = '0000').
	ADCCR -> CCR &= 0xFFFFFFE0; // All ADC independent (MULTI = '00000').
}

/* CONFIGURE AN ADC.
 * @param ADC: 			ADC address (should be 'ADC1', 'ADC2' or 'ADC3').
 * @param resolution: 	ADC resolution in bits (should be 'bit6', 'bit8', bit10' or 'bit12').
 * @return: 			None.
 */
void ADC_Init(ADC_BaseAddress* ADC, ADC_Resolution resolution) {
	// Enable peripheral clock.
	ADC_EnableClock(ADC);
	// Configure registers.
	ADC -> CR1 &= ~BIT_MASK(8); // Disable scan mode (SCAN = '0').
	ADC -> CR1 |= BIT_MASK(5); // Enable end of conversion interrupt (EOCIE = '1').
	ADC -> CR2 |= BIT_MASK(10); // EOC set at the end of each regular conversion (EOCS = '1').
	ADC -> CR2 &= ~BIT_MASK(1); // Single conversion mode (CONT = '0').
	ADC -> SMPR1 &= 0xF7000000; // Sampling time = 3 cycles (SMPx = '000').
	ADC -> SMPR2 &= 0xC0000000; // Sampling time = 3 cycles (SMPx = '000').
	// Configure resolution.
	switch(resolution) {
	case bits6:
		ADC -> CR1 |= BIT_MASK(25);
		ADC -> CR1 |= BIT_MASK(24);
		break;
	case bits8:
		ADC -> CR1 |= BIT_MASK(25);
		ADC -> CR1 &= ~BIT_MASK(24);
		break;
	case bits10:
		ADC -> CR1 &= ~BIT_MASK(25);
		ADC -> CR1 |= BIT_MASK(24);
		break;
	case bits12:
		ADC -> CR1 &= ~BIT_MASK(25);
		ADC -> CR1 &= ~BIT_MASK(24);
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
	ADC -> CR2 &= ~BIT_MASK(11); // (ALIGN = '0').
	// Enable ADC.
	ADC -> CR2 |= BIT_MASK(0); // (ADON = '1').
}

/* SET THE CURRENT CHANNEL OF AN ADC.
 * @param ADC: 		ADC address (should be 'ADC1', 'ADC2' or 'ADC3').
 * @param channel: 	ADC channel (should be 'ADCChannel0' to 'ADCChannel15', 'NC', 'VREF' or 'VBAT').
 * @return: 		None.
 */
void ADC_SetChannel(ADC_BaseAddress* ADC, ADC_Channel channel) {
	unsigned int i = 0;
	for(i=0 ; i<ADC_SQx_LENGTH ; i++) {
		unsigned int channelMasked = (channel-ADC_CHANNEL_ENUM_OFFSET) & BIT_MASK(i);
		if (channelMasked == 0) {
			// Bit = '0'.
			ADC -> SQR3 &= ~BIT_MASK(i);
		}
		else {
			// Bit = '1'.
			ADC -> SQR3 |= BIT_MASK(i);
		}
	}
}

/* RETURNS THE MAXIMUM VALUE READ BY AN ADC (FOR Vin=Vcc).
 * @param ADC: 		ADC address (should be 'ADC1', 'ADC2' or 'ADC3').
 * @return result: 	Maximum ADC output value represented in decimal = 2^(resolution)-1.
 */
unsigned int ADC_GetFullScale(ADC_BaseAddress* ADC) {
	unsigned int result = 0;
	if ((ADC -> CR1 & BIT_MASK(25)) == 0) {
		if ((ADC -> CR1 & BIT_MASK(24)) == 0) {
			// RES = '00'.
			result = bits12;
		}
		else {
			// RES = '01'.
			result = bits10;
		}
	}
	else {
		if ((ADC -> CR1 & BIT_MASK(24)) == 0) {
			// RES = '10'.
			result = bits8;
		}
		else {
			// RES = '11'.
			result = bits6;
		}
	}
	return result;
}

/* START ONE ADC CONVERSION.
 * @param ADC: 	ADC address (should be 'ADC1', 'ADC2' or 'ADC3').
 * @return: 	None.
 */
void ADC_StartConversion(ADC_BaseAddress* ADC) {
	conversionCompleted = false;
	ADC -> CR2 |= BIT_MASK(30); // Start conversion (SWSTART = '1').
}

/* RETURNS THE ADC OUTPUT VALUE.
 * @param ADC: 	ADC address (should be 'ADC1', 'ADC2' or 'ADC3').
 * @return:		ADC conversion result represented in decimal.
 */
unsigned int ADC_GetVoltage(ADC_BaseAddress* ADC) {
	return (ADC -> DR);
}

/* MAIN ROUTINE OF ADC.
 * @param blUnlocked:	Indicates if the BL is unlocked ('true') or not ('false').
 * @return:				None.
 */
void ADC_Routine(boolean blUnlocked) {
	switch (adcState) {
	case ADC_OFF:
		// ADC routine only runs if BL is unlocked.
		if (blUnlocked == true) {
			// Next measure is ZPT selector.
			ADC_SetChannel((ZPT_ADC -> adcAddress), (ZPT_ADC -> channel));
			adcState = ADC_ZPT;
			ADC_StartConversion(ZPT_ADC -> adcAddress);
		}
		break;
	case ADC_ZPT:
		if (conversionCompleted == true) {
			// Transmit voltage to ZPT module.
			ZPT_SetVoltage(currentVoltage);
			/* TBC */
			// Next measure is PBL2 selector.
			ADC_SetChannel((PBL2_ADC -> adcAddress), (PBL2_ADC -> channel));
			adcState = ADC_PBL2;
			ADC_StartConversion(PBL2_ADC -> adcAddress);
		}
		break;
	case ADC_PBL2:
		if (conversionCompleted == true) {
			// Transmit voltage to PBL2 module.
			/* TBC */
			// End of routine.
			adcState = ADC_OFF;
		}
		break;
	default:
		// Unknown state.
		break;
	}
}

/* ADC COMMON INTERRUPT HANDLER.
 * @param: 	None.
 * @return: None.
 */
void ADC_Handler(void) {
	currentVoltage = ADC_GetVoltage(ADC1);
	conversionCompleted = true;
}
