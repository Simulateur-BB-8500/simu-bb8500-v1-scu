/*
 * dac.h
 *
 *  Created on: 18 sept. 2017
 *      Author: Ludo
 */

#ifndef __DAC_H__
#define __DAC_H__

/*** DAC functions ***/

void DAC_init(void);
void DAC_set_voltage_mv(unsigned int voltage_mv);
unsigned int DAC_get_voltage_mv(void);

#endif /* __DAC_H__ */
