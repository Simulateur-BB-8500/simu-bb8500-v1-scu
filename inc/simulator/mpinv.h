/*
 * mpinv.h
 *
 *  Created on: 8 apr. 2018
 *      Author: Ludo
 */

#ifndef __MPINV_H__
#define __MPINV_H__

/*** MPINV functions ***/

void MPINV_init(void);
void MPINV_set_voltage_mv(unsigned int mpinv_voltage_mv);
void MPINV_task(void);

#endif /* __MPINV_H__ */
