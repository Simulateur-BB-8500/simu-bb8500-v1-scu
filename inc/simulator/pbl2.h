/*
 * pbl2.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef __PBL2_H__
#define __PBL2_H__

#include "stdint.h"

/*** PBL2 functions ***/

void PBL2_init(void);
void PBL2_set_voltage_mv(uint32_t pbl2_voltage_mv);
void PBL2_task(void);

#endif /* APPLICATIVE___PBL2_H___ */
