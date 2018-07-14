/*
 * usart.c
 *
 *  Created on: 28 sept. 2017
 *      Author: Ludovic
 */

#include "at.h"
#include "gpio.h"
#include "rcc.h"
#include "mapping.h"
#include "nvic.h"
#include "rcc_reg.h"
#include "usart.h"
#include "usart_reg.h"

/*** USART local macros ***/

// Baud rate.
#define BAUD_RATE 				9600
// Buffer sizes.
#define USART_TX_BUFFER_SIZE	32
#define USART_RX_BUFFER_SIZE	32


/*** USART local structures ***/

typedef struct {
	unsigned char tx_buf[USART_TX_BUFFER_SIZE]; 	// Transmit buffer
	unsigned int tx_read_idx; 						// Reading index in TX buffer.
	unsigned int tx_write_idx; 						// Writing index in TX buffer.
	unsigned char rx_buf[USART_RX_BUFFER_SIZE]; 	// Receive buffer.
	unsigned int rx_idx;							// Current index in RX buffer.
} USART_Context;

/*** USART local global variables ***/

static USART_Context usart2_ctx;

/*** USART local functions ***/

/* APPEND A NEW BYTE TO TX BUFFER AND MANAGE INDEX ROLL-OVER.
 * @param newbyte:		Byte to store in buffer.
 * @return:				None.
 */
void USART2_FillTxBuffer(unsigned char new_tx_byte) {
	(usart2_ctx.tx_buf)[usart2_ctx.tx_write_idx] = new_tx_byte;
	// Increment index and manage roll-over.
	usart2_ctx.tx_write_idx++;
	if (usart2_ctx.tx_write_idx == USART_TX_BUFFER_SIZE) {
		usart2_ctx.tx_write_idx = 0;
	}
}

/* APPEND A NEW BYTE TO RX BUFFER AND MANAGE INDEX ROLL-OVER.
 * @param newbyte:		Byte to store in buffer.
 * @return:				None.
 */
void USART2_FillRxBuffer(unsigned char new_rx_byte) {
	(usart2_ctx.rx_buf)[usart2_ctx.rx_idx] = new_rx_byte;
	// Increment index and manage roll-over.
	usart2_ctx.rx_idx++;
	if (usart2_ctx.rx_idx == USART_RX_BUFFER_SIZE) {
		usart2_ctx.rx_idx = 0;
	}
}

/* CONVERTS A 4-BIT WORD TO THE ASCII CODE OF THE CORRESPONDING HEXADECIMAL CHARACTER.
 * @param n:	The word to converts.
 * @return:		The results of conversion.
 */
unsigned char CharToASCII(unsigned char n) {
	unsigned char result = 0;
	if (n <= 15) {
		result = (n <= 9 ? (char) (n + 48) : (char) (n + 55));
	}
	return result;
}

/* CONFIGURE USART PERIPHERAL.
 * @param:	None.
 * @return: None.
 */
void USART2_Init(void) {

	/* Init context */

	unsigned int i = 0;
	for (i=0 ; i<USART_TX_BUFFER_SIZE ; i++) (usart2_ctx.tx_buf)[i] = 0;
	for (i=0 ; i<USART_RX_BUFFER_SIZE ; i++) (usart2_ctx.rx_buf)[i] = 0;
	usart2_ctx.tx_read_idx = 0;
	usart2_ctx.tx_write_idx = 0;
	usart2_ctx.rx_idx = 0;

	/* Configure TX and RX GPIOs */

	GPIO_Configure(USART_SGKCU_TX_GPIO, AlternateFunction, PushPull, LowSpeed, NoPullUpNoPullDown);
	GPIO_Configure(USART_SGKCU_RX_GPIO, AlternateFunction, OpenDrain, LowSpeed, NoPullUpNoPullDown);

	/* Enable peripheral clock */

	RCC -> APB1ENR |= (0b1 << 17);

	/* Configure peripheral */

	// 1 stop bit, 8 data bits, oversampling by 16.
	USART2 -> CR1 = 0; // M='00' and OVER8='0'.
	USART2 -> CR2 = 0;
	USART2 -> CR3 = 0;
	// Baud rate.
	USART2 -> BRR = (RCC_PCLK1_KHZ*1000)/(BAUD_RATE); // USART clock = PCLK1 (APB peripheral).
	// Enable transmitter and receiver.
	USART2 -> CR1 |= (0b1 << 3); // TE='1'.
	USART2 -> CR1 |= (0b1 << 2); // RE='1'.
	// Enable interrupts.
	USART2 -> CR1 |= (0b1 << 5); // RXNEIE='1'.
	// Enable peripheral.
	USART2 -> CR1 |= (0b1 << 0); // UE='1'.
	NVIC_EnableInterrupt(IT_USART2);
}

/* SEND A BYTE THROUGH USART.
 * @param byte:			The byte to send.
 * @param format:		Display format (should be 'Binary', 'Hexadecimal', 'Decimal' or 'ASCII').
 * @return: 			None.
 */
void USART2_SendByte(unsigned char tx_byte, USART_Format format) {
	NVIC_DisableInterrupt(IT_USART2);
	switch (format) {
	unsigned int i;
	unsigned char hundreds, tens, units;
	case Binary:
		for (i=7 ; i>=0 ; i--) {
			if (tx_byte & (1 << i)) {
				USART2_FillTxBuffer(0x31); // = '1'.
			}
			else {
				USART2_FillTxBuffer(0x30); // = '0'.
			}
		}
		break;
	case Hexadecimal:
		USART2_FillTxBuffer(CharToASCII((tx_byte & 0xF0) >> 4));
		USART2_FillTxBuffer(CharToASCII(tx_byte & 0x0F));
		break;
	case Decimal:
		// Hundreds.
		hundreds = (tx_byte/100);
		USART2_FillTxBuffer(hundreds+48); // 48 = ASCII offset to reach character '0'.
		// Tens.
		tens = (tx_byte-hundreds*100)/10;
		USART2_FillTxBuffer(tens+48); // 48 = ASCII offset to reach character '0'.
		// Units.
		units = (tx_byte-hundreds*100-tens*10);
		USART2_FillTxBuffer(units+48); // 48 = ASCII offset to reach character '0'.
		break;
	case ASCII:
		// Raw byte.
		USART2_FillTxBuffer(tx_byte);
		break;
	}
	USART2 -> CR1 |= (0b1 << 7); // TXEIE = '1'.
	NVIC_EnableInterrupt(IT_USART2);
}

/* RETURNS THE LAST RECEIVED BYTE.
 * @param byte:			Pointer to char that will contain the last received byte.
 * @return returnCode:	0 if no new char is available ('byte' is not modified), 1 otherwise.
 */
unsigned char USART2_GetLastByte(unsigned char* last_rx_byte) {
	unsigned char returnCode = 0;
	// Get peripheral address.
	NVIC_DisableInterrupt(IT_USART2);
	(*last_rx_byte) = (usart2_ctx.rx_buf)[(usart2_ctx.rx_idx)];
	NVIC_EnableInterrupt(IT_USART2);
	return returnCode;
}

/* USART INTERRUPT HANDLER
 * @param:	None.
 * @return:	None.
 */
void USART2_InterruptHandler(void) {

	/* TX */

	if (((USART2 -> ISR) & (0b1 << 7)) != 0) { // TXE='1'.
		if ((usart2_ctx.tx_read_idx) != (usart2_ctx.tx_write_idx)) {
			// Send byte.
			USART2 -> TDR = (usart2_ctx.tx_buf)[usart2_ctx.tx_read_idx];
			// Increment read index.
			usart2_ctx.tx_read_idx++;
			if (usart2_ctx.tx_read_idx == USART_TX_BUFFER_SIZE) {
				usart2_ctx.tx_read_idx = 0;
			}
		}
		else {
			// No more bytes.
			USART2 -> CR1 &= ~(0b1 << 7); // TXEIE = '0'.
		}
	}

	/* RX */

	if (((USART2 -> ISR) & (0b1 << 5)) != 0) { // RXNE='1'.
		// Get and store new byte into RX buffer.
		unsigned char rx_byte = USART2 -> RDR;
		USART2_FillRxBuffer(rx_byte);
		AT_FillRxBuffer(rx_byte);
	}
}
