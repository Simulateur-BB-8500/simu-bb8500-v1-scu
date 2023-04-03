/*
 * whistle.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef __WHISTLE_H__
#define __WHISTLE_H__

#include "types.h"

void WHISTLE_init(void);
void WHISTLE_set_voltage_mv(uint32_t whistle_voltage_mv);
void WHISTLE_task(void);

#endif /* __WHISTLE_H__ */
