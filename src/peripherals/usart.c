/*
 * usart.c
 *
 *  Created on: 28 sept. 2017
 *      Author: Ludo
 */

#include "usart.h"

#include "gpio.h"
#include "lssgiu.h"
#include "mapping.h"
#include "nvic.h"
#include "rcc.h"
#include "rcc_reg.h"
#include "usart_reg.h"

/*** USART local macros ***/

#define USART_BAUD_RATE 		9600
#define USART_TIMEOUT_COUNT		100000

/*** USART local functions ***/

/* USART INTERRUPT HANDLER
 * @param:	None.
 * @return:	None.
 */
void __attribute__((optimize("-O0"))) USART1_IRQHandler(void) {
	// RX.
	if (((USART1 -> ISR) & (0b1 << 5)) != 0) { // RXNE='1'.
		// Get and store new byte into RX buffer.
		unsigned char rx_byte = (USART1 -> RDR);
		LSSGIU_FillRxBuffer(rx_byte);
		GPIO_toggle(&GPIO_LED_GREEN);
	}
	// Overrun error interrupt.
	if (((USART1 -> ISR) & (0b1 << 3)) != 0) {
		// Clear ORE flag.
		USART1 -> ICR |= (0b1 << 3);
	}
}

/*** USART functions ***/

/* CONFIGURE USART PERIPHERAL.
 * @param:	None.
 * @return: None.
 */
void USART1_init(void) {
	// Enable peripheral clock.
	RCC -> APB2ENR |= (0b1 << 4);
	// Configure GPIOs.
	GPIO_configure(&GPIO_USART1_TX, GPIO_MODE_ALTERNATE_FUNCTION, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_USART1_RX, GPIO_MODE_ALTERNATE_FUNCTION, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Configure peripheral.
	// 1 stop bit, 8 data bits, oversampling by 16.
	USART1 -> CR1 = 0; // M='00' and OVER8='0'.
	USART1 -> CR2 = 0;
	USART1 -> CR3 = 0;
	USART1 -> CR3 |= (0b1 << 12); // Disable overrun (OVRDIS='1').
	// Baud rate.
	USART1 -> BRR = (RCC_get_clock_frequency(RCC_CLOCK_PCLK2) * 1000) / (USART_BAUD_RATE); // USART clock = PCLK2 (APB2 peripheral).
	// Enable transmitter and receiver.
	USART1 -> CR1 |= (0b1 << 3); // TE='1'.
	USART1 -> CR1 |= (0b1 << 2); // RE='1'.
	USART1 -> CR1 |= (0b1 << 5); // Enable RX interrupt (RXNEIE='1').
	// Enable peripheral.
	USART1 -> CR1 |= (0b1 << 0); // UE='1'.
}

/* SEND A BYTE THROUGH USART.
 * @param tx_byte:	Byte to send.
 * @return: 		None.
 */
void USART1_send_byte(unsigned char tx_byte) {
	// Fill transmit register.
	USART1 -> TDR = tx_byte;
	// Wait for transmission to complete.
	unsigned int loop_count = 0;
	while (((USART1 -> ISR) & (0b1 << 7)) == 0) {
		// Wait for TXE='1' or timeout.
		loop_count++;
		if (loop_count > USART_TIMEOUT_COUNT) break;
	}
}

/* SEND A STRING THROUGH USART.
 * @param tx_string:	String to send.
 * @return: 			None.
 */
void USART1_send_string(char* tx_string) {
	while (*tx_string) {
		USART1_send_byte((unsigned char) *(tx_string++));
	}
}
