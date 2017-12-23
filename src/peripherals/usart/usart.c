/*
 * usart.c
 *
 *  Created on: 28 sept. 2017
 *      Author: Ludovic
 */

#include "at.h"
#include "atCommands.h"
#include "gpio.h"
#include "rcc.h"
#include "mapping.h"
#include "masks.h"
#include "usart.h"
#include "usart_reg.h"

/* USART #define */

#define USART_CLOCK SYSCLK // Peripheral clock prescaler = 1.

/*** USART functions ***/

/* CONFIGURE AN USART.
 * @param USART: USART address (should be 'USART1/2/3/6' or 'UART4/5/7/8').
 * @return: None.
 */
void USART_Init(USART_BaseAddress* USART) {
	// Init all registers to 0 by default.
	USART -> CR1 = 0;
	USART -> CR2 = 0;
	USART -> CR3 = 0;
	// Baud rate
	USART -> BRR = USART_CLOCK/BAUD_RATE;
	// Enable transmitter (TE = '1').
	USART -> CR1 |= BIT_MASK[3];
	// Enable receiver (RE = '1').
	USART -> CR1 |= BIT_MASK[2];
	// Enable USART (UE = '1').
	USART -> CR1 |= BIT_MASK[0];
	// Interrupts.
	USART -> CR1 |= BIT_MASK[5]; // RXNEIE = '1'.
	USART -> CR1 |= BIT_MASK[7]; // TXEIE = '1'.
	USART -> CR1 |= BIT_MASK[6]; // TCIE = '1'.
}

/* SEND A BYTE THROUGH USART.
 * @param USART: USART address (should be 'USART1/2/3/6' or 'UART4/5/7/8').
 * @param data: byte to send.
 * @return: None.
 */
void USART_Send(USART_BaseAddress* USART, unsigned char data) {
	// Check if transmit data register is empty before new sending (TXE = '1').
	if (((USART -> ISR) & BIT_MASK[7]) != 0) {
		USART -> TDR = data;
	}
}

/* USART2 INTERRUPT HANDLER.
 * @param: None.
 * @return: None.
 */
void USART2_Handler(void) {
	// Check if interrupt was risen by RXNE.
	if (((USART2 -> ISR) & BIT_MASK[5]) != 0) {
		AT_DecodeExecute(USART2 -> RDR); // Decode received byte.
		USART2 -> RQR |= BIT_MASK[3]; // Clear RXNE flag (RXFRQ = '1').
	}
	else {
		// Check if interrupt was risen by TC.
		if (((USART2 -> ISR) & BIT_MASK[6]) != 0) {
			GPIO_Toggle(LED2);
			USART2 -> ICR |= BIT_MASK[6]; // Clear TC flag (TCCF = '1').
		}
	}
}
