/*
 * mpinv.h
 *
 *  Created on: 8 avr. 2018
 *      Author: Ludovic
 */

#ifndef SIMULATOR_MPINV_H
#define SIMULATOR_MPINV_H

/*** MPINV functions ***/

void MPINV_Init(void);
void MPINV_SetVoltage(unsigned int new_voltage);
void MPINV_Routine(void);

#endif /* SIMULATOR_MPINV_H */
