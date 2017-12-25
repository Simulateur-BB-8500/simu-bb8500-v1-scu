/*
 * dac.c
 *
 *  Created on: 18 sept. 2017
 *      Author: Ludovic
 */

#include "dac.h"
#include "dac_reg.h"
#include "masks.h"
#include "rcc_reg.h"
#include "types.h"

/*** DAC functions ***/

/* CONFIGURES DAC CHANNELS.
 * @param: None.
 * @return: None.
 */
void DAC_Init(void) {
	// Enable DAC clock.
	RCC -> APB1ENR |= BIT_MASK[29]; // (DACEN = '1').
	// Enable channels.
	DAC -> CR |= 0x00010001; // Enable channels 1 and 2 (ENx = '1').
	// Enable output.
	DAC -> CR &= 0xFFF9FFF9; // Enable output buffers (BOFFx = '0') and disable triggers (TENx = '0').
}

/* SET DAC OUTPUT VOLTAGE.
 * @param channel: DAC channel ('Channel1' on PA4 or 'Channel2' on PA5).
 * @param voltage: Output voltage expressed as a 12-bits value.
 * 		0 = 0V.
 * 		DAC_FULL_SCALE = 4095 = 3.3V.
 * @return: None.
 */
void DAC_SetVoltage(DAC_Channel channel, unsigned int voltage) {
	if ((voltage >= 0) && (voltage <= DAC_FULL_SCALE)) {
		switch(channel) {
		case DAC_Channel1:
			DAC -> DHR12R1 = voltage;
			break;
		case DAC_Channel2:
			DAC -> DHR12R1 = voltage;
			break;
		default:
			break;
		}
	}
}

/* GET DAC CURRENT OUTPUT VOLTAGE.
 * @param channel: DAC channel ('Channel1' on PA4 or 'Channel2' on PA5).
 * @return voltage: Current output voltage expressed as a 12-bits value.
 * 		0 = 0V.
 * 		DAC_FULL_SCALE = 4095 = 3.3V.
 */
unsigned int DAC_GetVoltage(DAC_Channel channel) {
	unsigned int voltage;
	switch(channel) {
	case DAC_Channel1:
		voltage = DAC -> DOR1;
		break;
	case DAC_Channel2:
		voltage = DAC -> DOR2;
		break;
	default:
		break;
	}
	return voltage;
}
