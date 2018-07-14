/*
 * at.h
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludovic
 */

#ifndef GENERIC_AT_H
#define GENERIC_AT_H

/*** AT functions ***/

void AT_Init(void);
void AT_FillRxBuffer(unsigned char at_cmd);
void AT_Send(unsigned char at_cmd);
void AT_Routine(void);

#endif /* GENERIC_AT_H */
