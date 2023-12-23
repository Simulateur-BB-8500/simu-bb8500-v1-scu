/*
 * lsagiu.c
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludo
 */

#include "lsagiu.h"

#include "kvb.h"
#include "gpio.h"
#include "lsmcu.h"
#include "mapping.h"
#include "nvic.h"
#include "usart.h"
#include "stdint.h"

/*** LSAGIU local macros ***/

#define LSAGIU_RX_BUFFER_SIZE	32

/*** LSAGIU local structures ***/

/*******************************************************************/
typedef struct {
	volatile uint8_t rx_buf[LSAGIU_RX_BUFFER_SIZE];
	volatile uint32_t rx_write_idx;
	uint32_t rx_read_idx;
} LSAGIU_context_t;

/*** LSAGIU external global variables ***/

extern LSMCU_context_t lsmcu_ctx;

/*** LSAGIU local global variables ***/

static LSAGIU_context_t lsagiu_ctx;

/*** LSAGIU local functions ***/

/*******************************************************************/
static void _LSAGIU_fill_rx_buffer(uint8_t ls_cmd) {
	// Store command.
	lsagiu_ctx.rx_buf[lsagiu_ctx.rx_write_idx] = ls_cmd;
	// Increment read index and manage roll-over.
	lsagiu_ctx.rx_write_idx = ((lsagiu_ctx.rx_write_idx + 1) % LSAGIU_RX_BUFFER_SIZE);
}

/*******************************************************************/
static void _LSAGIU_decode(void) {
	// Read last command.
	uint8_t ls_cmd = lsagiu_ctx.rx_buf[lsagiu_ctx.rx_read_idx];
	if (ls_cmd <= LSMCU_TCH_SPEED_LAST) {
		// Store current speed in global context.
		lsmcu_ctx.speed_kmh = ls_cmd;
	}
	else if (ls_cmd <= LSMCU_SPEED_LIMIT_LAST) {
		// Store speed limit in global context.
		lsmcu_ctx.speed_limit_kmh = LSAGIU_SPEED_LIMIT_FACTOR * (ls_cmd - LSMCU_SPEED_LIMIT_OFFSET);
	}
	// Increment read index and manage roll-over.
	lsagiu_ctx.rx_read_idx = ((lsagiu_ctx.rx_read_idx + 1) % LSAGIU_RX_BUFFER_SIZE);
}

/*** LSAGIU functions ***/

/*******************************************************************/
void LSAGIU_init(void) {
	// Init context.
	uint32_t idx = 0;
	for (idx=0 ; idx<LSAGIU_RX_BUFFER_SIZE ; idx++) lsagiu_ctx.rx_buf[idx] = LSMCU_IN_NOP;
	lsagiu_ctx.rx_write_idx = 0;
	lsagiu_ctx.rx_read_idx = 0;
	// Init USART.
	USART1_init(&_LSAGIU_fill_rx_buffer);
	// Enable USART interrupt.
	NVIC_enable_interrupt(NVIC_INTERRUPT_USART1, NVIC_PRIORITY_USART1);
}

/*******************************************************************/
void LSAGIU_write(LSMCU_output_command_t command) {
	// Local variables.
	uint8_t data = command;
	// Send byte.
	USART1_write(&data, 1);
}

/*******************************************************************/
void LSAGIU_process(void) {
	// LSAGIU routine.
	if (lsagiu_ctx.rx_read_idx != lsagiu_ctx.rx_write_idx) {
		_LSAGIU_decode();
		GPIO_toggle(&GPIO_LED_GREEN);
	}
}
