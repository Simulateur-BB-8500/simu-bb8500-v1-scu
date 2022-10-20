/*
 * adc.h
 *
 *  Created on: 19 sept. 2017
 *      Author: Ludo
 */

#ifndef __ADC_H__
#define __ADC_H__

/*** ADC macros ***/

#define ADC_VCC_DEFAULT_MV	3300

/*** ADC functions ***/

void ADC1_init(void);
void ADC1_task(void);

#endif /* __ADC_H__ */
