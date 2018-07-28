/*
 * dac.c
 *
 *  Created on: 18 sept. 2017
 *      Author: Ludovic
 */

#include "dac.h"
#include "dac_reg.h"
#include "gpio.h"
#include "mapping.h"
#include "rcc_reg.h"

/*** DAC local macros ***/

// DAC full scale value for 12-bits resolution.
#define DAC_FULL_SCALE 	4095

/*** DAC functions ***/

/* CONFIGURE DAC PERIPHERAL.
 * @param: 	None.
 * @return: None.
 */
void DAC_Init(void) {

	/* Configure analog GPIOs */

	GPIO_Configure(AM_GPIO, Analog, OpenDrain, LowSpeed, NoPullUpNoPullDown);

	/* Enable peripheral clock */

	RCC -> APB1ENR |= (0b1 << 29); // DACEN='1'.

	/* Configure peripheral */

	// Enable channels.
	DAC -> CR |= (0b1 << 0); // EN1='1'.
	//DAC -> CR |= (0b1 << 16); // EN2='1'.
	// Enable output buffer and disable trigger.
	DAC -> CR &= ~(0b11 << 1); // BOFF1='0' and TEN1='0'.
	//DAC -> CR &= ~(0b11 << 17); // BOFF2='0' and TEN2='0'.
}

/* SET DAC OUTPUT VOLTAGE.
 * @param channel: 	DAC channel (0 for 'Channel1' on PA4, 1 for 'Channel2' on PA5).
 * @param voltage: 	Output voltage expressed in mV (between 0 and VCC_MV).
 * @return: 		None.
 */
void DAC_SetVoltageMv(unsigned char channel, unsigned int voltage_mv) {
	// Ensure new voltage is reachable.
	unsigned int real_voltage_mv = voltage_mv;
	if (real_voltage_mv < 0) {
		real_voltage_mv = 0;
	}
	if (real_voltage_mv > VCC_MV) {
		real_voltage_mv = VCC_MV;
	}
	if (channel == 0) {
		DAC -> DHR12R1 = (DAC_FULL_SCALE*real_voltage_mv)/(VCC_MV);
	}
	else {
		DAC -> DHR12R2 = (DAC_FULL_SCALE*real_voltage_mv)/(VCC_MV);
	}
}

/* GET DAC CURRENT OUTPUT VOLTAGE.
 * @param channel: 	DAC channel (0 for 'Channel1' on PA4, 1 for 'Channel2' on PA5).
 * @return voltage:	Current output voltage expressed in mV (between 0 and VCC_MV).
 */
unsigned int DAC_GetVoltageMv(unsigned char channel) {
	unsigned int voltage_mv;
	if (channel == 0) {
		voltage_mv = ((DAC -> DOR1)*VCC_MV)/(DAC_FULL_SCALE);
	}
	else {
		voltage_mv = ((DAC -> DOR2)*VCC_MV)/(DAC_FULL_SCALE);
	}
	return voltage_mv;
}
