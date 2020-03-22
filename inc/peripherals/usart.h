/*
 * usart.h
 *
 *  Created on: 28 sept. 2017
 *      Author: Ludo
 */

#ifndef USART_H
#define USART_H

/*** USART structures ***/

// Display format.
typedef enum {
	USART_FORMAT_BINARY,
	USART_FORMAT_HEXADECIMAL,
	USART_FORMAT_DECIMAL,
	USART_FORMAT_ASCII
} USART_Format;

/*** USART functions ***/

void USART1_Init(void);
void USART1_SendByte(unsigned char tx_byte, USART_Format format);

#endif /* _USART_H */
