/*
 * mpinv.h
 *
 *  Created on: 8 avr. 2018
 *      Author: Ludovic
 */

#ifndef MPINV_H
#define MPINV_H

/*** MPINV functions ***/

void MPINV_Init(void);
void MPINV_SetVoltageMv(unsigned int mpinv_voltage_mv);
void MPINV_Task(void);

#endif /* MPINV_H */
