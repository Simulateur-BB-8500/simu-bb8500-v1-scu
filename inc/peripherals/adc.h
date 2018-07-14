/*
 * adc.h
 *
 *  Created on: 19 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_ADC_H
#define PERIPHERALS_ADC_H

#include "adc_reg.h"

/*** ADC functions ***/

void ADC1_Init(void);
void ADC1_Routine(unsigned char bl_unlocked);

#endif /* PERIPHERALS_ADC_H */
