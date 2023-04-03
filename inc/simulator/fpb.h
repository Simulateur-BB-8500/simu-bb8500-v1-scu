/*
 * fpb.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef __FPB_H__
#define __FPB_H__

#include "types.h"

/*** FPB functions ***/

void FPB_init(void);
void FPB_set_voltage_mv(uint32_t fpb_voltage_mv);
void FPB_task(void);

#endif /* __FPB_H__ */
