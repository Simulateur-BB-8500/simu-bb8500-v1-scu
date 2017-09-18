/*
 * dac.h
 *
 *  Created on: 18 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_ANALOG_DAC_H_
#define PERIPHERALS_ANALOG_DAC_H_

#include "types.h"

/*** DAC #define ***/

#define DAC_FULL_SCALE 4095 // In 12-bits mode.

/*** DAC functions ***/

void DAC_Init(void);
void DAC_SetVoltage(DACChannel channel, unsigned int voltage);
unsigned int DAC_GetVoltage(DACChannel channel);

#endif /* PERIPHERALS_ANALOG_DAC_H_ */
