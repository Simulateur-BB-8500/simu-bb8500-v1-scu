/*
 * usart.c
 *
 *  Created on: 28 sep. 2017
 *      Author: Ludo
 */

#include "usart.h"

#include "gpio.h"
#include "lsagiu.h"
#include "mapping.h"
#include "nvic.h"
#include "rcc.h"
#include "rcc_reg.h"
#include "usart_reg.h"
#include "stddef.h"
#include "stdint.h"

/*** USART local macros ***/

#define USART_BAUD_RATE 			115200
#define USART_TIMEOUT_COUNT			100000

#define USART_TX_BUFFER_SIZE_BYTES	128

/*** USART local structures ***/

/*******************************************************************/
typedef struct {
	uint8_t tx_buffer[USART_TX_BUFFER_SIZE_BYTES];
	uint8_t tx_buffer_write_idx;
	uint8_t tx_buffer_read_idx;
	USART_rx_irq_cb_t rx_irq_callback;
} USART_context_t;

/*** USART local global variables ***/

static USART_context_t usart1_ctx;

/*** USART local functions ***/

/*******************************************************************/
void USART1_IRQHandler(void) {
	// Local variables.
	uint8_t rx_byte = 0;
	// RX.
	if (((USART1 -> ISR) & (0b1 << 5)) != 0) { // RXNE='1'.
		// Get and store new byte into RX buffer.
		rx_byte = (USART1 -> RDR);
		// Transmit byte to upper layer.
		if (usart1_ctx.rx_irq_callback != NULL) {
			usart1_ctx.rx_irq_callback(rx_byte);
		}
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
	// Local variables.
	uint8_t idx = 0;
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
	// Init context.
	for (idx=0 ; idx<USART_TX_BUFFER_SIZE_BYTES ; idx++) usart1_ctx.tx_buffer[idx] = LSMCU_OUT_NOP;
	usart1_ctx.tx_buffer_write_idx = 0;
	usart1_ctx.tx_buffer_read_idx = 0;
	// Register callback.
	usart1_ctx.rx_irq_callback = irq_callback;
}

/*******************************************************************/
void USART1_write(uint8_t* data, uint8_t data_size_bytes) {
	// Local variables.
	uint8_t idx = 0;
	// Byte loop.
	for (idx=0 ; idx<data_size_bytes ; idx++) {
		// Fill transmit buffer.
		usart1_ctx.tx_buffer[usart1_ctx.tx_buffer_write_idx] = data[idx];
		// Increment write index.
		usart1_ctx.tx_buffer_write_idx = (usart1_ctx.tx_buffer_write_idx + 1) % USART_TX_BUFFER_SIZE_BYTES;
	}
}

/*******************************************************************/
void USART1_process(void) {
	// Check index and flag.
	if ((usart1_ctx.tx_buffer_read_idx != usart1_ctx.tx_buffer_write_idx) && (((USART1 -> ISR) & (0b1 << 7)) != 0)) {
		// Send byte.
		USART1 -> TDR = usart1_ctx.tx_buffer[usart1_ctx.tx_buffer_read_idx];
		// Increment read index.
		usart1_ctx.tx_buffer_read_idx = (usart1_ctx.tx_buffer_read_idx + 1) % USART_TX_BUFFER_SIZE_BYTES;
	}
}
