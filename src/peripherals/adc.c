/*
 * adc.c
 *
 *  Created on: 19 sept. 2017
 *      Author: Ludovic
 */

#include "adc.h"
#include "adc_reg.h"
#include "gpio.h"
#include "mapping.h"
#include "mpinv.h"
#include "nvic.h"
#include "rcc_reg.h"
#include "zpt.h"

/*** ADC local macros ***/

// ADC1 full scale value for 12-bits resolution.
#define ADC1_FULL_SCALE		4095

/*** ADC local structures ***/

// ADC internal state machine.
typedef enum {
	ADC_STATE_OFF,
	ADC_STATE_READ_MPINV,
	ADC_STATE_READ_ZPT,
} ADC_InternalState;

typedef struct {
	ADC_InternalState internal_state;
	unsigned char conversion_completed;
} ADC_Context;

/*** ADC local global variables ***/

static ADC_Context adc_ctx;

/*** ADC local functions ***/

/* SET THE CURRENT CHANNEL OF ADC1.
 * @param channel: 		ADC channel (x for 'ADCChannelx', 17 for 'VREF' or 18 for 'VBAT').
 * @return: 			None.
 */
void ADC1_SetChannel(unsigned char channel) {
	// Ensure channel ranges between 0 and 18.
	unsigned char local_channel = channel;
	if (local_channel > 18) {
		local_channel = 18;
	}
	// Set channel.
	ADC1 -> SQR3 &= ~(0b11111 << 0);
	ADC1 -> SQR3 |= local_channel;
}

/* START ONE ADC1 CONVERSION.
 * @param:	None.
 * @return: None.
 */
void ADC1_StartConversion(void) {
	adc_ctx.conversion_completed = 0;
	// Start conversion.
	ADC1 -> CR2 |= (0b1 << 30); // SWSTART='1'.
}

/* RETURNS THE ADC OUTPUT VALUE.
 * @param:	None.
 * @return:	ADC conversion result represented in mV.
 */
unsigned int ADC1_GetVoltageMv(void) {
	return ((VCC_MV*(ADC1 -> DR)) / (ADC1_FULL_SCALE));
}

/*** ADC functions ***/

/* CONFIGURE ADC1 PERIPHERAL.
 * @param:	None.
 * @return: None.
 */
void ADC1_Init(void) {

	/* Init context */

	adc_ctx.internal_state = ADC_STATE_OFF;
	adc_ctx.conversion_completed = 0;

	/* Configure analog GPIOs */

	GPIO_Configure(ADC_ZPT_GPIO, Analog, OpenDrain, LowSpeed, NoPullUpNoPullDown);

	/* Enable peripheral clock */

	RCC -> APB2ENR |= (0b1 << 8);

	/* Common registers */

	ADCCR -> CCR &= ~(0b1 << 23) ; // Temperature sensor disabled (TSVREFE='0').
	ADCCR -> CCR &= ~(0b1 << 22) ; // Vbat channel disabled (VBATE='0').
	ADCCR -> CCR &= 0xFFFCFFFF; // Prescaler = 2 (ADCPRE='00').
	ADCCR -> CCR &= 0xFFFF2FFF; // DMA disabled (DMA='00').
	ADCCR -> CCR &= 0xFFFFF0FF; // Delay between to sampling phases = 5*T (DELAY='0000').
	ADCCR -> CCR &= 0xFFFFFFE0; // All ADC independent (MULTI='00000').

	/* Configure peripheral */

	ADC1 -> CR1 &= ~(0b1 << 8); // Disable scan mode (SCAN='0').
	ADC1 -> CR1 |= (0b1 << 5); // Enable end of conversion interrupt (EOCIE='1').
	ADC1 -> CR2 |= (0b1 << 10); // EOC set at the end of each regular conversion (EOCS='1').
	ADC1 -> CR2 &= ~(0b1 << 1); // Single conversion mode (CONT='0').
	ADC1 -> SMPR1 &= 0xF7000000; // Sampling time = 3 cycles (SMPx='000').
	ADC1 -> SMPR2 &= 0xC0000000; // Sampling time = 3 cycles (SMPx='000').
	// Configure resolution.
	ADC1 -> CR1 &= ~(0b11 << 24);
	// Regular sequence will always contain 1 channel.
	ADC1 -> SQR1 &= 0xFF000000;
	// Channel 0 selected by default.
	ADC1 -> SQR2 &= 0xC0000000;
	ADC1 -> SQR3 &= 0xC0000000;
	// Result in right alignement.
	ADC1 -> CR2 &= ~(0b1 << 11); // ALIGN='0'.
	// Enable interrupt.
	NVIC_EnableInterrupt(IT_ADC);
	// Enable ADC.
	ADC1 -> CR2 |= (0b1 << 0); // ADON='1'.
}

/* MAIN ROUTINE OF ADC.
 * @param bl_unlocked:	Indicates if the BL is unlocked ('1') or not ('0').
 * @return:				None.
 */
void ADC1_Routine(unsigned char bl_unlocked) {
	switch (adc_ctx.internal_state) {
	case ADC_STATE_OFF:
		// ADC routine only runs if BL is unlocked.
		if (bl_unlocked == 1) {
			// Next measure is ZPT selector.
			ADC1_SetChannel(8);
			adc_ctx.internal_state = ADC_STATE_READ_MPINV;
			ADC1_StartConversion();
		}
		break;
	case ADC_STATE_READ_MPINV:
		if (adc_ctx.conversion_completed == 1) {
			// Transmit voltage to MPINV module.
			MPINV_SetVoltage(ADC1_GetVoltageMv());
			// Next measure is ZPT selector.
			ADC1_SetChannel(8);
			adc_ctx.internal_state = ADC_STATE_READ_ZPT;
			ADC1_StartConversion();
		}
		break;
	case ADC_STATE_READ_ZPT:
		if (adc_ctx.conversion_completed == 1) {
			// Transmit voltage to ZPT module.
			ZPT_SetVoltage(ADC1_GetVoltageMv());
			// End of routine.
			adc_ctx.internal_state = ADC_STATE_OFF;
		}
		break;
	default:
		// Unknown state.
		break;
	}
}

/* ADCx INTERRUPT HANDLER.
 * @param: 	None.
 * @return: None.
 */
void ADC_InterruptHandler(void) {
	adc_ctx.conversion_completed = 1;
	// Clear EOC flag (since DR register read is performed later).
	ADC1 -> SR &= ~(0b1 << 1);
}
