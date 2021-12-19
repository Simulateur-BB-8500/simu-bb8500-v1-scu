/*
 * whistle.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef WHISTLE_H
#define WHISTLE_H

void WHISTLE_Init(void);
void WHISTLE_SetVoltageMv(unsigned int whistle_voltage_mv);
void WHISTLE_Task(void);

#endif /* WHISTLE_H */
