/*
 * usart.c
 *
 *  Created on: 28 sept. 2017
 *      Author: Ludo
 */

#include "usart.h"

#include "gpio.h"
#include "lssgkcu.h"
#include "mapping.h"
#include "nvic.h"
#include "rcc.h"
#include "rcc_reg.h"
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
} USART_Context;

/*** USART local global variables ***/

static USART_Context usart1_ctx;

/*** USART local functions ***/

/* USART INTERRUPT HANDLER
 * @param:	None.
 * @return:	None.
 */
void USART1_InterruptHandler(void) {

	/* TX */
	if (((USART1 -> ISR) & (0b1 << 7)) != 0) { // TXE='1'.
		if ((usart1_ctx.tx_read_idx) != (usart1_ctx.tx_write_idx)) {
			// Send byte.
			USART1 -> TDR = (usart1_ctx.tx_buf)[usart1_ctx.tx_read_idx];
			// Increment read index.
			usart1_ctx.tx_read_idx++;
			if (usart1_ctx.tx_read_idx == USART_TX_BUFFER_SIZE) {
				usart1_ctx.tx_read_idx = 0;
			}
		}
		else {
			// No more bytes.
			USART1 -> CR1 &= ~(0b1 << 7); // TXEIE = '0'.
		}
	}

	/* RX */
	if (((USART1 -> ISR) & (0b1 << 5)) != 0) { // RXNE='1'.
		// Get and store new byte into RX buffer.
		unsigned char rx_byte = USART1 -> RDR;
		LSSGKCU_FillRxBuffer(rx_byte);
	}
}

/* APPEND A NEW BYTE TO TX BUFFER AND MANAGE INDEX ROLL-OVER.
 * @param newbyte:		Byte to store in buffer.
 * @return:				None.
 */
void USART1_FillTxBuffer(unsigned char new_tx_byte) {
	(usart1_ctx.tx_buf)[usart1_ctx.tx_write_idx] = new_tx_byte;
	// Increment index and manage roll-over.
	usart1_ctx.tx_write_idx++;
	if (usart1_ctx.tx_write_idx == USART_TX_BUFFER_SIZE) {
		usart1_ctx.tx_write_idx = 0;
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
void USART1_Init(void) {

	/* Init context */
	unsigned int i = 0;
	for (i=0 ; i<USART_TX_BUFFER_SIZE ; i++) (usart1_ctx.tx_buf)[i] = 0;
	usart1_ctx.tx_read_idx = 0;
	usart1_ctx.tx_write_idx = 0;

	/* Enable peripheral clock */
	RCC -> APB2ENR |= (0b1 << 4);

	/* Configure TX and RX GPIOs */
	GPIO_Configure(&GPIO_USART1_TX, GPIO_MODE_ALTERNATE_FUNCTION, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure(&GPIO_USART1_RX, GPIO_MODE_ALTERNATE_FUNCTION, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);

	/* Configure peripheral */
	// 1 stop bit, 8 data bits, oversampling by 16.
	USART1 -> CR1 = 0; // M='00' and OVER8='0'.
	USART1 -> CR2 = 0;
	USART1 -> CR3 = 0;
	// Baud rate.
	USART1 -> BRR = (RCC_PCLK2_KHZ * 1000) / (BAUD_RATE); // USART clock = PCLK2 (APB2 peripheral).
	// Enable transmitter and receiver.
	USART1 -> CR1 |= (0b1 << 3); // TE='1'.
	USART1 -> CR1 |= (0b1 << 2); // RE='1'.
	// Enable interrupts.
	USART1 -> CR1 |= (0b1 << 5); // RXNEIE='1'.
	// Enable peripheral.
	USART1 -> CR1 |= (0b1 << 0); // UE='1'.
	NVIC_EnableInterrupt(IT_USART1);
}

/* SEND A BYTE THROUGH USART.
 * @param byte:			The byte to send.
 * @param format:		Display format (should be 'Binary', 'Hexadecimal', 'Decimal' or 'ASCII').
 * @return: 			None.
 */
void USART1_SendByte(unsigned char tx_byte, USART_Format format) {
	NVIC_DisableInterrupt(IT_USART1);
	switch (format) {
	unsigned int i;
	unsigned char hundreds, tens, units;
	case USART_FORMAT_BINARY:
		for (i=7 ; i>=0 ; i--) {
			if (tx_byte & (1 << i)) {
				USART1_FillTxBuffer(0x31); // = '1'.
			}
			else {
				USART1_FillTxBuffer(0x30); // = '0'.
			}
		}
		break;
	case USART_FORMAT_HEXADECIMAL:
		USART1_FillTxBuffer(CharToASCII((tx_byte & 0xF0) >> 4));
		USART1_FillTxBuffer(CharToASCII(tx_byte & 0x0F));
		break;
	case USART_FORMAT_DECIMAL:
		// Hundreds.
		hundreds = (tx_byte/100);
		USART1_FillTxBuffer(hundreds+48); // 48 = ASCII offset to reach character '0'.
		// Tens.
		tens = (tx_byte-hundreds*100)/10;
		USART1_FillTxBuffer(tens+48); // 48 = ASCII offset to reach character '0'.
		// Units.
		units = (tx_byte-hundreds*100-tens*10);
		USART1_FillTxBuffer(units+48); // 48 = ASCII offset to reach character '0'.
		break;
	case USART_FORMAT_ASCII:
		// Raw byte.
		USART1_FillTxBuffer(tx_byte);
		break;
	}
	USART1 -> CR1 |= (0b1 << 7); // TXEIE = '1'.
	NVIC_EnableInterrupt(IT_USART1);
}
