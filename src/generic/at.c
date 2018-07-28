/*
 * at.c
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludovic
 */

#include "at.h"
#include "atCommands.h"
#include "kvb.h"
#include "gpio.h"
#include "mapping.h"
#include "tch.h"
#include "usart.h"

/*** AT local macros ***/

#define AT_RX_BUFFER_SIZE	32

/*** AT local structures ***/

typedef struct {
	unsigned char rx_buf[AT_RX_BUFFER_SIZE];
	unsigned int rx_write_idx;
	unsigned int rx_read_idx;
} AT_Context;

/*** AT local global variables ***/

static AT_Context at_ctx;

/*** AT local functions ***/

/* DECODE THE CURRENT AT COMMAND.
 * @param:	None.
 * @return: None.
 */
void AT_DecodeSGKCU(void) {
	unsigned char at_command = at_ctx.rx_buf[at_ctx.rx_read_idx];
	if (at_command <= TCH_SPEED_MAX_KMH) {
		// Transmit speed to Tachro and VACMA modules.
		TCH_SetSpeed(at_command);
	}
	else {
		// Decode AT command.
		switch(at_command) {
		case AT_KVB_LVAL_BLINK:
			GPIO_Write(KVB_LVAL_GPIO, 0);
			KVB_EnableBlinkLVAL(1);
			break;
		case AT_KVB_LVAL_ON:
			KVB_EnableBlinkLVAL(0);
			GPIO_Write(KVB_LVAL_GPIO, 1);
			break;
		case AT_KVB_LVAL_OFF:
			KVB_EnableBlinkLVAL(0);
			GPIO_Write(KVB_LVAL_GPIO, 0);
			break;
		case AT_KVB_LMV_ON:
			GPIO_Write(KVB_LMV_GPIO, 1);
			break;
		case AT_KVB_LMV_OFF:
			GPIO_Write(KVB_LMV_GPIO, 0);
			break;
		case AT_KVB_LFC_ON:
			GPIO_Write(KVB_LFC_GPIO, 1);
			break;
		case AT_KVB_LFC_OFF:
			GPIO_Write(KVB_LFC_GPIO, 0);
			break;
		case AT_KVB_LV_ON:
			GPIO_Write(KVB_LV_GPIO, 1);
			break;
		case AT_KVB_LV_OFF:
			GPIO_Write(KVB_LV_GPIO, 0);
			break;
		case AT_KVB_LFU_ON:
			GPIO_Write(KVB_LFU_GPIO, 1);
			break;
		case AT_KVB_LFU_OFF:
			GPIO_Write(KVB_LFU_GPIO, 0);
			break;
		case AT_KVB_LPS_ON:
			GPIO_Write(KVB_LPS_GPIO, 1);
			break;
		case AT_KVB_LPS_OFF:
			GPIO_Write(KVB_LPS_GPIO, 0);
			break;
		case AT_KVB_LSSF_BLINK:
			GPIO_Write(KVB_LSSF_GPIO, 0);
			KVB_EnableBlinkLSSF(1);
			break;
		case AT_KVB_LSSF_ON:
			KVB_EnableBlinkLSSF(0);
			GPIO_Write(KVB_LSSF_GPIO, 1);
			break;
		case AT_KVB_LSSF_OFF:
			KVB_EnableBlinkLSSF(0);
			GPIO_Write(KVB_LSSF_GPIO, 0);
			break;
		case AT_KVB_YG_OFF:
			KVB_DisplayOff();
			break;
		case AT_KVB_YG_PA400:
			KVB_Display(KVB_PA400_TEXT);
			break;
		case AT_KVB_YG_UC512:
		case AT_KVB_YG_888:
		case AT_KVB_YG_DASH:
		case AT_KVB_G_B:
		case AT_KVB_Y_B:
		case AT_KVB_G_P:
		case AT_KVB_Y_P:
		case AT_KVB_G_L:
		case AT_KVB_Y_L:
		case AT_KVB_G_00:
		case AT_KVB_Y_00:
		case AT_KVB_G_000:
		case AT_KVB_Y_000:
		default:
			// Unknown command.
			break;
		}
	}
}

/*** AT functions ***/

/* INIT ALL AT COMMAND MANAGERS.
 * @param:	None.
 * @return:	None.
 */
void AT_Init(void) {

	/* Init context */

	unsigned int i = 0;
	for(i=0 ; i<AT_RX_BUFFER_SIZE ; i++) at_ctx.rx_buf[i] = 0;
	at_ctx.rx_write_idx = 0;
	at_ctx.rx_read_idx = 0;

	/* Init USART peripheral */

	USART2_Init();
}

/* FILL AT BUFFER (CALLED BY USART2 INTERRUPT HANDLER).
 * @param at_cmd:	The new AT command to store.
 * @return:			None.
 */
void AT_FillRxBuffer(unsigned char at_cmd) {
	at_ctx.rx_buf[at_ctx.rx_write_idx] = at_cmd;
	at_ctx.rx_write_idx++;
	// Roll-over management.
	if (at_ctx.rx_write_idx == AT_RX_BUFFER_SIZE) {
		at_ctx.rx_write_idx = 0;
	}
}

/* SEND AN AT COMMAND TO SGKCU.
 * @param at_cmd: 	AT command (byte) to transmit.
 * @return: 			None.
 */
void AT_Send(unsigned char at_cmd) {
	USART2_SendByte(at_cmd, ASCII);
}

/* MAIN ROUTINE OF AT COMMAND MANAGERS.
 * @param:	None.
 * @return:	None.
 */
void AT_Routine(void) {
	// SGKCU routine.
	if (at_ctx.rx_read_idx != at_ctx.rx_write_idx) {
		AT_DecodeSGKCU();
		// Increment read index and manage roll-over.
		at_ctx.rx_read_idx++;
		if (at_ctx.rx_read_idx == AT_RX_BUFFER_SIZE) {
			at_ctx.rx_read_idx = 0;
		}
	}
}
