/*
 * usart.h
 *
 *  Created on: 28 sept. 2017
 *      Author: Ludo
 */

#ifndef USART_H
#define USART_H

#include "mode.h"

/*** USART functions ***/

void USART1_Init(void);
void USART1_SendByte(unsigned char tx_byte);
#ifdef DEBUG
void USART1_SendString(char* tx_string);
#endif

#endif /* USART_H */
