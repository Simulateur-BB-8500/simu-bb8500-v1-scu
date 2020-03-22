/*
 * adc.h
 *
 *  Created on: 19 sept. 2017
 *      Author: Ludo
 */

#ifndef ADC_H
#define ADC_H

#include "common.h"

/*** ADC functions ***/

void ADC1_Init(void);
void ADC1_Task(LSMCU_Context* lsmcu_ctx);

#endif /* ADC_H */
