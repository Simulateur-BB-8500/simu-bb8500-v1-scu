/*
 * usart.h
 *
 *  Created on: 28 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_USART_USART_H_
#define PERIPHERALS_USART_USART_H_

#include "usart_reg.h"

/*** USART functions ***/

void USART_Init(USART_BaseAddress* USART);
void USART_Send(USART_BaseAddress* USART, unsigned char data);

#endif /* PERIPHERALS_USART_USART_H_ */
