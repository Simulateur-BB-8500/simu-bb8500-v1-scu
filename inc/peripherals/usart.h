/*
 * usart.h
 *
 *  Created on: 28 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_USART_H
#define PERIPHERALS_USART_H

#include "usart_reg.h"

/*** USART enumerations ***/

// Display format.
typedef enum ByteDisplayFormat {
	Binary,
	Hexadecimal,
	Decimal,
	ASCII
} USART_Format;

/*** USART functions ***/

void USART2_Init(void);
void USART2_SendByte(unsigned char tx_byte, USART_Format format);
unsigned char USART2_GetLastByte(unsigned char* last_rx_byte);

#endif /* PERIPHERALS_USART_H */
