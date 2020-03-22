/*
 * fpb.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef FPB_H
#define FPB_H

/*** FPB functions ***/

void FPB_Init(void);
void FPB_SetVoltageMv(unsigned int fpb_voltage_mv);
void FPB_Task(void);

#endif /* FPB_H */
