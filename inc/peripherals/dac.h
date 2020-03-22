/*
 * dac.h
 *
 *  Created on: 18 sept. 2017
 *      Author: Ludo
 */

#ifndef DAC_H
#define DAC_H

/*** DAC functions ***/

void DAC_Init(void);
void DAC_SetVoltageMv(unsigned int voltage_mv);
unsigned int DAC_GetVoltageMv(void);

#endif /* DAC_H */
