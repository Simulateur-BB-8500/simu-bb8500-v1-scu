/*
 * fd.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef FD_H
#define FD_H

/*** FD functions ***/

void FD_Init(void);
void FD_SetVoltageMv(unsigned int fd_voltage_mv);
void FD_Task(void);

#endif /* FD_H */
