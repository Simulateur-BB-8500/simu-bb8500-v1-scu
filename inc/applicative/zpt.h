/*
 * zpt.h
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludo
 */

#ifndef ZPT_H
#define ZPT_H

#include "common.h"

/*** ZPT functions ***/

void ZPT_Init(void);
void ZPT_SetVoltageMv(unsigned int zpt_voltage_mv);
void ZPT_Task(LSMCU_Context* lscmu_ctx);

#endif /* ZPT_H */
