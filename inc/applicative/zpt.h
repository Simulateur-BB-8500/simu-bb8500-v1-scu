/*
 * zpt.h
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludovic
 */

#ifndef ZPT_H
#define ZPT_H

/*** ZPT functions ***/

void ZPT_Init(void);
void ZPT_SetVoltageMv(unsigned int zpt_voltage_mv);
void ZPT_Task(void);

#endif /* ZPT_H */
