/*
 * fpb.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef __FPB_H__
#define __FPB_H__

/*** FPB functions ***/

void FPB_init(void);
void FPB_set_voltage_mv(unsigned int fpb_voltage_mv);
void FPB_task(void);

#endif /* __FPB_H__ */
