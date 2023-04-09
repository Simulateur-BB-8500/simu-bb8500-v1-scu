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
#include "mode.h"
#include "nvic.h"
#include "usart.h"
#ifdef DEBUG
#include "string.h"
#endif
#include "stdint.h"

/*** LSAGIU local macros ***/

#define LSAGIU_RX_BUFFER_SIZE	32

/*** LSAGIU local structures ***/

typedef struct {
	volatile uint8_t rx_buf[LSAGIU_RX_BUFFER_SIZE];
	volatile uint32_t rx_write_idx;
	uint32_t rx_read_idx;
} LSAGIU_Context;

/*** LSAGIU external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** LSAGIU local global variables ***/

static LSAGIU_Context lsagiu_ctx;

/*** LSAGIU local functions ***/

/* DECODE THE CURRENT LSAGIU COMMAND.
 * @param:	None.
 * @return: None.
 */
static void _LSAGIU_decode(void) {
	// Read last command.
	uint8_t ls_cmd = lsagiu_ctx.rx_buf[lsagiu_ctx.rx_read_idx];
	if (ls_cmd <= LSMCU_TCH_SPEED_LAST) {
		// Store current speed in global context.
		lsmcu_ctx.speed_kmh = ls_cmd;
	}
	else if (ls_cmd <= LSMCU_SPEED_LIMIT_LAST) {
		// Store speed limit in global context.
		lsmcu_ctx.speed_limit_kmh = 10 * (ls_cmd - LSMCU_SPEED_LIMIT_OFFSET);
	}
	// Increment read index and manage roll-over.
	lsagiu_ctx.rx_read_idx++;
	if (lsagiu_ctx.rx_read_idx >= LSAGIU_RX_BUFFER_SIZE) {
		lsagiu_ctx.rx_read_idx = 0;
	}
}

/*** LSAGIU functions ***/

/* INIT LSAGIU COMMUNICATION MODULE.
 * @param:	None.
 * @return:	None.
 */
void LSAGIU_Init(void) {
	// Init context.
	uint32_t idx = 0;
	for (idx=0 ; idx<LSAGIU_RX_BUFFER_SIZE ; idx++) lsagiu_ctx.rx_buf[idx] = LSMCU_IN_NOP;
	lsagiu_ctx.rx_write_idx = 0;
	lsagiu_ctx.rx_read_idx = 0;
	// Enable USART interrupt.
	NVIC_enable_interrupt(NVIC_INTERRUPT_USART1);
}

/* FILL LSAGIU BUFFER (CALLED BY USART2 INTERRUPT HANDLER).
 * @param lsagiu_cmd:	The new LSAGIU command to store.
 * @return:			None.
 */
void LSAGIU_FillRxBuffer(uint8_t ls_cmd) {
	lsagiu_ctx.rx_buf[lsagiu_ctx.rx_write_idx] = ls_cmd;
	lsagiu_ctx.rx_write_idx++;
	// Roll-over management.
	if (lsagiu_ctx.rx_write_idx >= LSAGIU_RX_BUFFER_SIZE) {
		lsagiu_ctx.rx_write_idx = 0;
	}
}

/* SEND AN LSAGIU COMMAND TO SGKCU.
 * @param lsagiu_cmd: 	LSAGIU command (byte) to transmit.
 * @return: 			None.
 */
void LSAGIU_Send(uint8_t ls_cmd) {
#ifdef DEBUG
	char str_value[16];
	STRING_value_to_string(ls_cmd, STRING_FORMAT_DECIMAL, 0, str_value);
	USART1_send_string("\nLSAGIU command = ");
	USART1_send_string(str_value);
	USART1_send_string("\n");
#else
	USART1_send_byte(ls_cmd);
#endif
}

/* MAIN ROUTINE OF LSAGIU COMMAND MANAGER.
 * @param:	None.
 * @return:	None.
 */
void LSAGIU_Task(void) {
	// LSAGIU routine.
	if (lsagiu_ctx.rx_read_idx != lsagiu_ctx.rx_write_idx) {
		_LSAGIU_decode();
		GPIO_toggle(&GPIO_LED_GREEN);
	}
}
