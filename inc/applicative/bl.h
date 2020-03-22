/*
 * bl.h
 *
 *  Created on: 8 apr. 2018
 *      Author: Ludo
 */

#ifndef BL_H
#define BL_H

#include "common.h"

/*** BL functions ***/

void BL_Init(void);
void BL_Task(LSMCU_Context* lsmcu_ctx);

#endif /* BL_H */
