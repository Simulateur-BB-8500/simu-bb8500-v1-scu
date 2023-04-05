/*
 * zpt.h
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludo
 */

#ifndef __ZPT_H__
#define __ZPT_H__

#include "stdint.h"

/*** ZPT functions ***/

void ZPT_init(void);
void ZPT_set_voltage_mv(uint32_t zpt_voltage_mv);
void ZPT_task(void);

#endif /* __ZPT_H__ */
