/*
 * fd.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef __FD_H__
#define __FD_H__

/*** FD functions ***/

void FD_init(void);
void FD_set_voltage_mv(unsigned int fd_voltage_mv);
void FD_task(void);

#endif /* __FD_H__ */
