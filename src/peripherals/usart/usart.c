/*
 * usart.c
 *
 *  Created on: 28 sept. 2017
 *      Author: Ludovic
 */

#include "at.h"
#include "atCommands.h"
#include "enum.h"
#include "gpio.h"
#include "rcc.h"
#include "mapping.h"
#include "masks.h"
#include "nvic.h"
#include "nvic_reg.h"
#include "rcc_reg.h"
#include "usart.h"
#include "usart_reg.h"

/*** USART internal functions ***/

/* RETURN THE CORRESPONDING INTERRUPT INDEX OF A GIVEN USART.
 * @param USART: 		USART base address (should be 'USART1/2/3/6' or 'UART4/5/7/8').
 * @return itNumber:	The corresponding IT number in NVIC.
 */
unsigned int USART_GetIT(USART_BaseAddress* USART) {
	unsigned int itNumber;
	// Check peripheral address.
	switch ((unsigned int) USART) {
	case ((unsigned int) USART1):
		itNumber = IT_USART1;
		break;
	case ((unsigned int) USART2):
		itNumber = IT_USART2;
		break;
	case ((unsigned int) USART3):
		itNumber = IT_USART3;
		break;
	case ((unsigned int) UART4):
		itNumber = IT_UART4;
		break;
	case ((unsigned int) UART5):
		itNumber = IT_UART5;
		break;
	case ((unsigned int) USART6):
		itNumber = IT_USART6;
		break;
	case ((unsigned int) UART7):
		itNumber = IT_UART7;
		break;
	case ((unsigned int) UART8):
		itNumber = IT_UART8;
		break;
	default:
		break;
	}
	return itNumber;
}

/* ENABLE THE CORRESPONDING CLOCK OF AN USART PERIPHERAL.
 * @param USART: 	USART base address (should be 'USART1/2/3/6' or 'UART4/5/7/8').
 * @return:			None.
 */
void USART_EnableClock(USART_BaseAddress* USART) {
	// Check peripheral address.
	switch ((unsigned int) USART) {
	case ((unsigned int) USART1):
		RCC -> APB2ENR |= BIT_MASK[4]; // USART1EN = '1'.
		break;
	case ((unsigned int) USART2):
		RCC -> APB1ENR |= BIT_MASK[17]; // USART2EN = '1'.
		break;
	case ((unsigned int) USART3):
		RCC -> APB1ENR |= BIT_MASK[18]; // USART3EN = '1'.
		break;
	case ((unsigned int) UART4):
		RCC -> APB1ENR |= BIT_MASK[19]; // UART4EN = '1'.
		break;
	case ((unsigned int) UART5):
		RCC -> APB1ENR |= BIT_MASK[20]; // UART5EN = '1'.
		break;
	case ((unsigned int) USART6):
		RCC -> APB2ENR |= BIT_MASK[5]; // USART6EN = '1'.
		break;
	case ((unsigned int) UART7):
		RCC -> APB1ENR |= BIT_MASK[30]; // UART7EN = '1'.
		break;
	case ((unsigned int) UART8):
		RCC -> APB1ENR |= BIT_MASK[31]; // UART8EN = '1'.
		break;
	default:
		break;
	}
}

/* CONVERTS A 4-BIT WORD TO THE ASCII CODE OF THE CORRESPONDING HEXADECIMAL CHARACTER.
 * @param n:	The word to converts.
 * @return:		The results of conversion.
 */
unsigned char USART_CharToASCII(unsigned char n) {
	return (n <= 9 ? (char) (n + 48) : (char) (n + 55));
}

/*** USART functions ***/

/* CONFIGURE AN USART.
 * @param usartStruct:	USART structure to initialise.
 * @param USART: 		Address of the USART used (should be 'USART1/2/3/6' or 'UART4/5/7/8').
 * @return: 			None.
 */
void USART_Init(USART_Struct* usartStruct, USART_BaseAddress* USART) {
	// Enable peripheral clock.
	USART_EnableClock(USART);
	// Set peripheral address.
	usartStruct -> usartAddress = USART;
	// Init buffers.
	unsigned int i = 0;
	for (i=0 ; i<256 ; i++) {
		(usartStruct -> txBuffer)[i] = 0;
		(usartStruct -> rxBuffer)[i] = 0;
	}
	// Init indexes.
	usartStruct -> txReadIndex = 0;
	usartStruct -> txWriteIndex = 0;
	usartStruct -> rxReadIndex = 0;
	usartStruct -> rxWriteIndex = 0;
	// Init peripheral.
	USART -> CR1 = 0; // 1 stop bit & 8 data bits (M = '00'). Oversampling by 16 (OVER8 = '0').
	USART -> CR2 = 0;
	USART -> CR3 = 0;
	// Baud rate.
	USART -> BRR = SYSCLK/BAUD_RATE; // USART clock = SYSCLK because prescaler = 1.
	// Bidirectionnal.
	USART -> CR1 |= BIT_MASK[3]; // Enable transmitter (TE = '1').
	USART -> CR1 |= BIT_MASK[2]; // Enable receiver (RE = '1').
	// Enable interrupts.
	USART -> CR1 |= BIT_MASK[5]; // RXNEIE = '1'.
	// Enable peripheral.
	USART -> CR1 |= BIT_MASK[0]; // Enable USART (UE = '1').
	NVIC_EnableInterrupt(USART_GetIT(USART));
}

/* SENDS A BYTE THROUGH USART.
 * @param usartStruct: 	USART structure used to send.
 * @param byte:			The byte to send.
 * @param format:		Display format (should be 'Binary', 'Hexadecimal', 'Decimal' or 'ASCII').
 * @return: 			None.
 */
void USART_SendByte(USART_Struct* usartStruct, unsigned char byte, ByteDisplayFormat format) {
	USART_BaseAddress* USART = usartStruct -> usartAddress;
	NVIC_DisableInterrupt(USART_GetIT(USART));
	switch (format) {
	int i;
	case Binary:
		for (i = 7; i >= 0; i--) {
			if (((usartStruct -> txWriteIndex) + 1) != (usartStruct -> txReadIndex)) {
				if (byte & (1 << i)) {
					(usartStruct -> txBuffer)[(usartStruct -> txWriteIndex)++] = 0x31; // = '1'.
				}
				else {
					(usartStruct -> txBuffer)[(usartStruct -> txWriteIndex)++] = 0x30; // = '0'.
				}
			}
			else {
				// No more space.
			}
		}
		break;
	case Hexadecimal:
		(usartStruct -> txBuffer)[(usartStruct -> txWriteIndex)++] = USART_CharToASCII((byte & 0xF0) >> 4);
		(usartStruct -> txBuffer)[(usartStruct -> txWriteIndex)++] = USART_CharToASCII(byte & 0x0F);
		break;
	case Decimal:
		if (((usartStruct -> txWriteIndex) + 1) != (usartStruct -> txReadIndex)) {
			// Hundreds.
			unsigned char hundreds = (byte/100);
			(usartStruct -> txBuffer)[(usartStruct -> txWriteIndex)++] = hundreds+48; // 48 = ASCII offset to reach character '0'.
			// Tens.
			unsigned char tens = (byte-hundreds*100)/10;
			(usartStruct -> txBuffer)[(usartStruct -> txWriteIndex)++] = tens+48; // 48 = ASCII offset to reach character '0'.
			// Units.
			unsigned char units = (byte-hundreds*100-tens*10);
			(usartStruct -> txBuffer)[(usartStruct -> txWriteIndex)++] = units+48; // 48 = ASCII offset to reach character '0'.
		}
		else {
			// No more space.
		}
		break;
	case ASCII:
		if (((usartStruct -> txWriteIndex) + 1) != (usartStruct -> txReadIndex)) {
			// Raw byte.
			(usartStruct -> txBuffer)[(usartStruct -> txWriteIndex)++] = byte;
		}
		else {
			// No more space.
		}
		break;
	}
	NVIC_EnableInterrupt(USART_GetIT(USART));
	USART -> CR1 |= BIT_MASK[7]; // TXEIE = '1'.
}

/* RETURNS THE LAST RECEIVED BYTE.
 * @param usartStruct:	USART structure used to receive.
 * @param byte:			Pointer to char that will contain the last received byte.
 * @return returnCode:	0 if no new char is available ('byte' is not modified), 1 otherwise.
 */
unsigned char USART_GetByte(USART_Struct* usartStruct, unsigned char* byte) {
	unsigned char returnCode = 0;
	// Get peripheral address.
	USART_BaseAddress* USART = usartStruct -> usartAddress;
	NVIC_DisableInterrupt(USART_GetIT(USART));
	if ((usartStruct -> rxReadIndex) != (usartStruct -> rxWriteIndex)) {
		returnCode = 1;
		(*byte) = (usartStruct -> rxBuffer)[(usartStruct -> rxReadIndex)++];
	}
	NVIC_EnableInterrupt(USART_GetIT(USART));
	return returnCode;
}

/* USART SGKCU INTERRUPT HANDLER.
 * @param: 	None.
 * @return: None.
 */
void USART_SGKCU_Handler(void) {
	// Get perpheral address.
	USART_BaseAddress* USART = USART_SGKCU -> usartAddress;
	// Interrupt was risen by TXE.
	if (((USART -> ISR) & BIT_MASK[7]) != 0) {
		if ((USART_SGKCU -> txReadIndex) != (USART_SGKCU -> txWriteIndex)) {
			// Send byte.
			USART -> TDR = (USART_SGKCU -> txBuffer)[(USART_SGKCU -> txReadIndex)++];
		}
		else {
			// No more bytes.
			USART -> CR1 &= ~BIT_MASK[7]; // TXEIE = '0'.
		}
	}
	// Interrupt was risen by RXNE.
	if (((USART -> ISR) & BIT_MASK[5]) != 0) {
		unsigned char rxByte = USART -> RDR;
		// Decode received byte.
		AT_DecodeSGKCU(rxByte);
		// Store new byte into RX buffer.
		(USART_SGKCU -> rxBuffer)[(USART_SGKCU -> rxWriteIndex)++] = rxByte;
		if ((USART_SGKCU -> rxWriteIndex) == (USART_SGKCU -> rxReadIndex)) {
			(USART_SGKCU -> rxWriteIndex)--;
		}
	}
}
