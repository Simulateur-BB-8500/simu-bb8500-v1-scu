/*
 * usart.h
 *
 *  Created on: 28 sept. 2017
 *      Author: Ludo
 */

#ifndef __USART_H__
#define __USART_H__

#include "mode.h"

/*** USART functions ***/

void USART1_init(void);
void USART1_send_byte(unsigned char tx_byte);
#ifdef DEBUG
void USART1_send_string(char* tx_string);
#endif

#endif /* __USART_H__ */
