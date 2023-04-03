/*
 * dac.h
 *
 *  Created on: 18 sept. 2017
 *      Author: Ludo
 */

#ifndef __DAC_H__
#define __DAC_H__

#include "types.h"

/*** DAC functions ***/

void DAC_init(void);
void DAC_set_voltage_mv(uint32_t voltage_mv);
uint32_t DAC_get_voltage_mv(void);

#endif /* __DAC_H__ */
