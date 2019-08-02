/*
 * dac.h
 *
 *  Created on: 18 sept. 2017
 *      Author: Ludovic
 */

#ifndef DAC_H
#define DAC_H

/*** DAC functions ***/

void DAC_Init(void);
void DAC_SetVoltageMv(unsigned char channel, unsigned int voltage_mv);
unsigned int DAC_GetVoltageMv(unsigned char channel);

#endif /* DAC_H */
