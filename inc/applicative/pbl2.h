/*
 * pbl2.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludovic
 */

#ifndef PBL2_H
#define PBL2_H

#include "common.h"

/*** PBL2 functions ***/

void PBL2_Init(void);
void PBL2_SetVoltageMv(unsigned int pbl2_voltage_mv);
void PBL2_Task(void);

#endif /* APPLICATIVE_PBL2_H_ */
