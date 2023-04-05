/*
 * adc.c
 *
 *  Created on: 19 sept. 2017
 *      Author: Ludo
 */

#include "adc.h"

#include "adc_reg.h"
#include "fpb.h"
#include "fd.h"
#include "gpio.h"
#include "lsmcu.h"
#include "mapping.h"
#include "mpinv.h"
#include "pbl2.h"
#include "rcc_reg.h"
#include "stdint.h"
#include "whistle.h"
#include "zpt.h"

/*** ADC local macros ***/

// ADC1 full scale value for 12-bits resolution.
#define ADC_FULL_SCALE		4095
// Channels definition.
#define ADC_CHANNEL_ZPT		0
#define ADC_CHANNEL_PBL2	1
#define ADC_CHANNEL_FPB		2
#define ADC_CHANNEL_AM		3
#define ADC_CHANNEL_FD		5
#define ADC_CHANNEL_MPINV	6
#define ADC_CHANNEL_S		7
#define ADC_CHANNEL_ZLFR	9
#define ADC_CHANNEL_MAX		18

/*** ADC local structures ***/

// ADC internal state machine.
typedef enum {
	ADC_STATE_OFF,
	ADC_STATE_READ_ZPT,
	ADC_STATE_READ_S,
	ADC_STATE_READ_ZLFR,
	ADC_STATE_READ_MPINV,
	ADC_STATE_READ_PBL2,
	ADC_STATE_READ_FPB,
	ADC_STATE_READ_FD,
	ADC_STATE_READ_AM
} ADC_State;

/*** ADC external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** ADC local global variables ***/

static ADC_State adc_state;

/*** ADC local functions ***/

/* SET THE CURRENT CHANNEL OF ADC1.
 * @param channel: 		ADC channel (x for 'ADCChannelx', 17 for 'VREF' or 18 for 'VBAT').
 * @return: 			None.
 */
static void _ADC1_set_channel(uint8_t channel) {
	// Ensure channel ranges between 0 and 18.
	uint8_t local_channel = channel;
	if (local_channel > ADC_CHANNEL_MAX) {
		local_channel = ADC_CHANNEL_MAX;
	}
	// Set channel.
	ADC1 -> SQR3 &= ~(0b11111 << 0);
	ADC1 -> SQR3 |= local_channel;
}

/* START ONE ADC1 CONVERSION.
 * @param:	None.
 * @return: None.
 */
static void _ADC1_start_conversion(void) {
	// Clear EOC flag.
	ADC1 -> SR &= ~(0b1 << 1);
	// Start conversion.
	ADC1 -> CR2 |= (0b1 << 30); // SWSTART='1'.
}

/* RETURNS THE ADC OUTPUT VALUE.
 * @param:	None.
 * @return:	ADC conversion result represented in mV.
 */
static uint32_t _ADC1_get_voltage_mv(void) {
	return ((ADC_VCC_DEFAULT_MV * (ADC1 -> DR)) / (ADC_FULL_SCALE));
}

/*** ADC functions ***/

/* CONFIGURE ADC1 PERIPHERAL.
 * @param:	None.
 * @return: None.
 */
void ADC1_init(void) {
	// Init context */
	adc_state = ADC_STATE_OFF;
	// Enable peripheral clock */
	RCC -> APB2ENR |= (0b1 << 8);
	// Common registers.
	ADCCR -> CCR &= ~(0b1 << 23) ; // Temperature sensor disabled (TSVREFE='0').
	ADCCR -> CCR &= ~(0b1 << 22) ; // Vbat channel disabled (VBATE='0').
	ADCCR -> CCR &= 0xFFFCFFFF; // Prescaler = 2 (ADCPRE='00').
	ADCCR -> CCR &= 0xFFFF2FFF; // DMA disabled (DMA='00').
	ADCCR -> CCR &= 0xFFFFF0FF; // Delay between to sampling phases = 5*T (DELAY='0000').
	ADCCR -> CCR &= 0xFFFFFFE0; // All ADC independent (MULTI='00000').
	// Configure peripheral.
	ADC1 -> CR1 &= ~(0b1 << 8); // Disable scan mode (SCAN='0').
	ADC1 -> CR2 |= (0b1 << 10); // EOC set at the end of each regular conversion (EOCS='1').
	ADC1 -> CR2 &= ~(0b1 << 1); // Single conversion mode (CONT='0').
	ADC1 -> SMPR1 &= 0xF7000000; // Sampling time = 3 cycles (SMPx='000').
	ADC1 -> SMPR2 &= 0xC0000000; // Sampling time = 3 cycles (SMPx='000').
	ADC1 -> CR1 &= ~(0b11 << 24); // Resolution
	ADC1 -> SQR1 &= 0xFF000000; // Regular sequence will always contain 1 channel.
	ADC1 -> SQR2 &= 0xC0000000; // Channel 0 selected by default.
	ADC1 -> SQR3 &= 0xC0000000;
	ADC1 -> CR2 &= ~(0b1 << 11); // // Result in right alignement (ALIGN='0').
	// Enable ADC.
	ADC1 -> CR2 |= (0b1 << 0); // ADON='1'.
}

/* MAIN ROUTINE OF ADC.
 * @param:	None.
 * @return:	None.
 */
void ADC1_task(void) {
	uint32_t adc_result_mv = 0;
	switch (adc_state) {
	case ADC_STATE_OFF:
		// Check ZBA.
		if (lsmcu_ctx.zba_closed != 0) {
			// Start first conversion.
			_ADC1_set_channel(ADC_CHANNEL_ZPT);
			_ADC1_start_conversion();
			adc_state = ADC_STATE_READ_ZPT;
		}
		break;
	case ADC_STATE_READ_ZPT:
		// Check end of conversion flag.
		if (((ADC1 -> SR) & (0b1 << 1)) != 0) {
			// Transmit voltage to ZPT module.
			adc_result_mv = _ADC1_get_voltage_mv();
			ZPT_set_voltage_mv(adc_result_mv);
			// Start next conversion.
			_ADC1_set_channel(ADC_CHANNEL_S);
			_ADC1_start_conversion();
			adc_state = ADC_STATE_READ_S;
		}
		break;
	case ADC_STATE_READ_S:
		// Check end of conversion flag.
		if (((ADC1 -> SR) & (0b1 << 1)) != 0) {
			// Transmit voltage to S module.
			adc_result_mv = _ADC1_get_voltage_mv();
			WHISTLE_set_voltage_mv(adc_result_mv);
			// Start next conversion.
			_ADC1_set_channel(ADC_CHANNEL_ZLFR);
			_ADC1_start_conversion();
			adc_state = ADC_STATE_READ_ZLFR;
		}
		break;
	case ADC_STATE_READ_ZLFR:
		// Check end of conversion flag.
		if (((ADC1 -> SR) & (0b1 << 1)) != 0) {
			adc_result_mv = _ADC1_get_voltage_mv();
			// TBD
			// Start next conversion.
			_ADC1_set_channel(ADC_CHANNEL_MPINV);
			_ADC1_start_conversion();
			adc_state = ADC_STATE_READ_MPINV;
		}
		break;
	case ADC_STATE_READ_MPINV:
		// Check end of conversion flag.
		if (((ADC1 -> SR) & (0b1 << 1)) != 0) {
			// Transmit voltage to MPINV module.
			adc_result_mv = _ADC1_get_voltage_mv();
			MPINV_set_voltage_mv(adc_result_mv);
			// Start next conversion.
			_ADC1_set_channel(ADC_CHANNEL_PBL2);
			_ADC1_start_conversion();
			adc_state = ADC_STATE_READ_PBL2;
		}
		break;
	case ADC_STATE_READ_PBL2:
		// Check end of conversion flag.
		if (((ADC1 -> SR) & (0b1 << 1)) != 0) {
			// Transmit voltage to MPINV module.
			adc_result_mv = _ADC1_get_voltage_mv();
			PBL2_set_voltage_mv(adc_result_mv);
			// Start next conversion.
			_ADC1_set_channel(ADC_CHANNEL_FPB);
			_ADC1_start_conversion();
			adc_state = ADC_STATE_READ_FPB;
		}
		break;
	case ADC_STATE_READ_FPB:
		// Check end of conversion flag.
		if (((ADC1 -> SR) & (0b1 << 1)) != 0) {
			// Transmit voltage to FPB module.
			adc_result_mv = _ADC1_get_voltage_mv();
			FPB_set_voltage_mv(adc_result_mv);
			// Start next conversion.
			_ADC1_set_channel(ADC_CHANNEL_FD);
			_ADC1_start_conversion();
			adc_state = ADC_STATE_READ_FD;
		}
		break;
	case ADC_STATE_READ_FD:
		// Check end of conversion flag.
		if (((ADC1 -> SR) & (0b1 << 1)) != 0) {
			// Transmit voltage to FD module.
			adc_result_mv = _ADC1_get_voltage_mv();
			FD_set_voltage_mv(adc_result_mv);
			// Start next conversion.
			_ADC1_set_channel(ADC_CHANNEL_AM);
			_ADC1_start_conversion();
			adc_state = ADC_STATE_READ_AM;
		}
		break;
	case ADC_STATE_READ_AM:
		// End of sequence.
		adc_state = ADC_STATE_OFF;
		break;
	default:
		// Unknown state.
		adc_state = ADC_STATE_OFF;
		break;
	}
}
