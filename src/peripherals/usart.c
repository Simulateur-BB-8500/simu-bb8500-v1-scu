/*
 * usart.c
 *
 *  Created on: 28 sep. 2017
 *      Author: Ludo
 */

#include "usart.h"

#include "gpio.h"
#include "mapping.h"
#include "nvic.h"
#include "rcc.h"
#include "rcc_reg.h"
#include "usart_reg.h"
#include "stddef.h"
#include "stdint.h"

/*** USART local macros ***/

#define USART_BAUD_RATE 		115200
#define USART_TIMEOUT_COUNT		100000

/*** USART local global variables ***/

static USART_rx_irq_cb_t usart1_rx_irq_callback = NULL;

/*** USART local functions ***/

/*******************************************************************/
void __attribute__((optimize("-O0"))) USART1_IRQHandler(void) {
	// RX.
	if (((USART1 -> ISR) & (0b1 << 5)) != 0) { // RXNE='1'.
		// Get and store new byte into RX buffer.
		uint8_t rx_byte = (USART1 -> RDR);
		// Transmit byte to upper layer.
		if ((((USART1 -> CR1) & (0b1 << 5)) != 0) && (usart1_rx_irq_callback != NULL)) {
			usart1_rx_irq_callback(rx_byte);
		}
		// Clear flag.
		USART1 -> RQR |= (0b1 << 3);
	}
	// Overrun error interrupt.
	if (((USART1 -> ISR) & (0b1 << 3)) != 0) {
		// Clear ORE flag.
		USART1 -> ICR |= (0b1 << 3);
	}
}

/*** USART functions ***/

/*******************************************************************/
void USART1_init(USART_rx_irq_cb_t irq_callback) {
	// Enable peripheral clock.
	RCC -> APB2ENR |= (0b1 << 4);
	// Configure peripheral.
	// 1 stop bit, 8 data bits, oversampling by 16.
	USART1 -> CR1 = 0; // M='00' and OVER8='0'.
	USART1 -> CR2 = 0;
	USART1 -> CR3 = 0;
	USART1 -> CR3 |= (0b1 << 12); // Disable overrun (OVRDIS='1').
	// Baud rate.
	USART1 -> BRR = (RCC_get_frequency_hz(RCC_CLOCK_APB2)) / (USART_BAUD_RATE); // USART clock = PCLK2 (APB2 peripheral).
	// Enable transmitter and receiver.
	USART1 -> CR1 |= (0b1 << 3); // TE='1'.
	USART1 -> CR1 |= (0b1 << 2); // RE='1'.
	// Enable interrupt.
	USART1 -> CR1 |= (0b1 << 5); // Enable RX interrupt (RXNEIE='1').
	// Enable peripheral.
	USART1 -> CR1 |= (0b1 << 0); // UE='1'.
	// Configure GPIOs.
	GPIO_configure(&GPIO_USART1_TX, GPIO_MODE_ALTERNATE_FUNCTION, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_UP);
	GPIO_configure(&GPIO_USART1_RX, GPIO_MODE_ALTERNATE_FUNCTION, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_UP);
	// Register callback.
	usart1_rx_irq_callback = irq_callback;
}

/*******************************************************************/
void USART1_write(uint8_t* data, uint8_t data_size_bytes) {
	// Local variables.
	uint8_t idx = 0;
	uint32_t loop_count = 0;
	// Byte loop.
	for (idx=0 ; idx<data_size_bytes ; idx++) {
		// Fill transmit register.
		USART1 -> TDR = data[idx];
		// Wait for transmission to complete.
		while (((USART1 -> ISR) & (0b1 << 7)) == 0) {
			// Wait for TXE='1' or timeout.
			loop_count++;
			if (loop_count > USART_TIMEOUT_COUNT) break;
		}
	}
}
