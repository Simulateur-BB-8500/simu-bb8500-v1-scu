/*
 * sgdu.c
 *
 *  Created on: 01 oct. 2017
 *      Author: Ludo
 */

#include "sgdu.h"

#include "kvb.h"
#include "gpio.h"
#include "mapping.h"
#include "nvic.h"
#include "scu.h"
#include "scu_commands.h"
#include "usart.h"
#include "stdint.h"

/*** SGDU local macros ***/

#define SGDU_RX_BUFFER_SIZE     32

/*** SGDU local structures ***/

/*******************************************************************/
typedef struct {
    volatile uint8_t rx_buf[SGDU_RX_BUFFER_SIZE];
    volatile uint8_t rx_write_idx;
    uint8_t rx_read_idx;
} SGDU_context_t;

/*** SGDU external global variables ***/

extern SCU_context_t scu_ctx;

/*** SGDU local global variables ***/

static SGDU_context_t lsagiu_ctx;

/*** SGDU local functions ***/

/*******************************************************************/
static void _SGDU_fill_rx_buffer(uint8_t ls_cmd) {
    // Store command.
    lsagiu_ctx.rx_buf[lsagiu_ctx.rx_write_idx] = ls_cmd;
    // Increment write index.
    lsagiu_ctx.rx_write_idx = (lsagiu_ctx.rx_write_idx + 1) % SGDU_RX_BUFFER_SIZE;
}

/*******************************************************************/
static void _SGDU_decode(void) {
    // Read last command.
    uint8_t ls_cmd = lsagiu_ctx.rx_buf[lsagiu_ctx.rx_read_idx];
    // Decode command.
    if (ls_cmd <= SCU_TCH_SPEED_LAST) {
        // Store current speed in global context.
        scu_ctx.speed_kmh = ls_cmd;
    }
    else if (ls_cmd <= SCU_SPEED_LIMIT_LAST) {
        // Store speed limit in global context.
        scu_ctx.speed_limit_kmh = SCU_SPEED_LIMIT_FACTOR * (ls_cmd - SCU_SPEED_LIMIT_OFFSET);
    }
    // Increment read index.
    lsagiu_ctx.rx_read_idx = (lsagiu_ctx.rx_read_idx + 1) % SGDU_RX_BUFFER_SIZE;
}

/*** SGDU functions ***/

/*******************************************************************/
void SGDU_init(void) {
    // Init context.
    uint32_t idx = 0;
    for (idx = 0; idx < SGDU_RX_BUFFER_SIZE; idx++) {
        lsagiu_ctx.rx_buf[idx] = SCU_IN_NOP;
    }
    lsagiu_ctx.rx_write_idx = 0;
    lsagiu_ctx.rx_read_idx = 0;
    // Init USART.
    USART1_init(&_SGDU_fill_rx_buffer);
    // Enable USART interrupt.
    NVIC_enable_interrupt(NVIC_INTERRUPT_USART1, NVIC_PRIORITY_USART1);
}

/*******************************************************************/
void SGDU_write(SCU_output_command_t command) {
    // Local variables.
    uint8_t data = command;
    // Send byte.
    USART1_write(&data, 1);
}

/*******************************************************************/
void SGDU_process(void) {
    // SGDU routine.
    if (lsagiu_ctx.rx_read_idx != lsagiu_ctx.rx_write_idx) {
        _SGDU_decode();
    }
    USART1_process();
}
