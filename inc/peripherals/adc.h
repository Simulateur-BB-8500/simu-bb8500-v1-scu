/*
 * adc.h
 *
 *  Created on: 19 sept. 2017
 *      Author: Ludo
 */

#ifndef ADC_H
#define ADC_H

/*** ADC macros ***/

#define ADC_VCC_DEFAULT_MV	3300

/*** ADC functions ***/

void ADC1_Init(void);
void ADC1_Task(void);

#endif /* ADC_H */
