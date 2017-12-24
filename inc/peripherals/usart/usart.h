/*
 * usart.h
 *
 *  Created on: 28 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_USART_USART_H_
#define PERIPHERALS_USART_USART_H_

#include "usart_reg.h"

/*** USART structures and enumerations ***/

typedef struct {
	USART_BaseAddress* usartAddress; // 'USART1/2/3/6' or 'UART4/5/7/8'.
	unsigned char txBuffer[256]; // Transmit buffer (only works with size = 256 because indexes are 'unsigned char').
	unsigned char txReadIndex; // Reading index in txBuffer.
	unsigned char txWriteIndex; // Writing index in txBuffer.
	unsigned char rxBuffer[256]; // Receive buffer (only works with size = 256 because indexes are 'unsigned char').
	unsigned char rxReadIndex; // Reading index in rxBuffer.
	unsigned char rxWriteIndex; // Writing index in rxBuffer.
} USART_Struct;

typedef enum ByteDisplayFormat {
	Binary,
	Hexadecimal,
	Decimal,
	ASCII
} ByteDisplayFormat;

/*** USART global variables ***/

static USART_Struct* USART_SGKCU;

/*** USART functions ***/

void USART_Init(USART_Struct* usartStruct, USART_BaseAddress* usart);
void USART_SendByte(USART_Struct* usartStruct, unsigned char byte, ByteDisplayFormat format);
unsigned char USART_GetByte(USART_Struct* usartStruct, unsigned char* byte);

#endif /* PERIPHERALS_USART_USART_H_ */
