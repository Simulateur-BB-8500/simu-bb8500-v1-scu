/*
 * kvb.h
 *
 *  Created on: 26 dec. 2017
 *      Author: Ludo
 */

#ifndef __KVB_H__
#define __KVB_H__

#include "stdint.h"

/*** KVB functions ***/

void KVB_init(void);
void KVB_task(void);

void __attribute__((optimize("-O0"))) KVB_sweep(void);

#endif /* __KVB_H__ */
