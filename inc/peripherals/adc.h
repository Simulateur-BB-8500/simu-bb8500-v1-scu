/*
 * adc.h
 *
 *  Created on: 19 sept. 2017
 *      Author: Ludovic
 */

#ifndef ADC_H
#define ADC_H

#include "adc_reg.h"

/*** ADC functions ***/

void ADC1_Init(void);
void ADC1_Task(unsigned char bl_unlocked);

#endif /* ADC_H */
