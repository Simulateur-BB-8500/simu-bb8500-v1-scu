/*
 * adc.h
 *
 *  Created on: 19 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_ANALOG_ADC_H_
#define PERIPHERALS_ANALOG_ADC_H_

#include "adc_reg.h"
#include "types.h"

/*** ADC functions ***/

void ADCCR_Init(void);
void ADC_Init(ADC_BaseAddress* ADC, ADC_Resolution resolution);
void ADC_SetChannel(ADC_BaseAddress* ADC, ADC_Channel channel);
void ADC_StartConversion(ADC_BaseAddress* ADC);
unsigned int ADC_GetVoltage(ADC_BaseAddress* ADC);

#endif /* PERIPHERALS_ANALOG_ADC_H_ */
