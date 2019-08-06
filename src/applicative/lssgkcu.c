/*
 * lssgkcu.c
 *
 *  Crelssgkcued on: 1 oct. 2017
 *      Author: Ludovic
 */

#include "lssgkcu.h"

#include "kvb.h"
#include "gpio.h"
#include "mapping.h"
#include "tch.h"
#include "usart.h"

/*** LSSGKCU local macros ***/

#define LSSGKCU_RX_BUFFER_SIZE	32

/*** LSSGKCU local structures ***/

typedef struct {
	unsigned char rx_buf[LSSGKCU_RX_BUFFER_SIZE];
	unsigned int rx_write_idx;
	unsigned int rx_read_idx;
} LSSGKCU_Context;

/*** LSSGKCU local global variables ***/

static LSSGKCU_Context lssgkcu_ctx;

/*** LSSGKCU local functions ***/

/* DECODE THE CURRENT LSSGKCU COMMAND.
 * @param:	None.
 * @return: None.
 */
void LSSGKCU_Decode(void) {
	unsigned char lssgkcu_command = lssgkcu_ctx.rx_buf[lssgkcu_ctx.rx_read_idx];
	if (lssgkcu_command <= TCH_SPEED_MAX_KMH) {
		// Transmit speed to Tachro and VACMA modules.
		TCH_SetSpeedKmh(lssgkcu_command);
	}
	else {
		// Decode LSSGKCU command.
		switch(lssgkcu_command) {
		case LSSGKCU_KVB_LVAL_BLINK:
			GPIO_Write(&GPIO_KVB_LVAL, 0);
			KVB_EnableBlinkLVAL(1);
			break;
		case LSSGKCU_KVB_LVAL_ON:
			KVB_EnableBlinkLVAL(0);
			GPIO_Write(&GPIO_KVB_LVAL, 1);
			break;
		case LSSGKCU_KVB_LVAL_OFF:
			KVB_EnableBlinkLVAL(0);
			GPIO_Write(&GPIO_KVB_LVAL, 0);
			break;
		case LSSGKCU_KVB_LMV_ON:
			GPIO_Write(&GPIO_KVB_LMV, 1);
			break;
		case LSSGKCU_KVB_LMV_OFF:
			GPIO_Write(&GPIO_KVB_LMV, 0);
			break;
		case LSSGKCU_KVB_LFC_ON:
			GPIO_Write(&GPIO_KVB_LFC, 1);
			break;
		case LSSGKCU_KVB_LFC_OFF:
			GPIO_Write(&GPIO_KVB_LFC, 0);
			break;
		case LSSGKCU_KVB_LV_ON:
			GPIO_Write(&GPIO_KVB_LV, 1);
			break;
		case LSSGKCU_KVB_LV_OFF:
			GPIO_Write(&GPIO_KVB_LV, 0);
			break;
		case LSSGKCU_KVB_LFU_ON:
			GPIO_Write(&GPIO_KVB_LFU, 1);
			break;
		case LSSGKCU_KVB_LFU_OFF:
			GPIO_Write(&GPIO_KVB_LFU, 0);
			break;
		case LSSGKCU_KVB_LPS_ON:
			GPIO_Write(&GPIO_KVB_LPS, 1);
			break;
		case LSSGKCU_KVB_LPS_OFF:
			GPIO_Write(&GPIO_KVB_LPS, 0);
			break;
		case LSSGKCU_KVB_LSSF_BLINK:
			GPIO_Write(&GPIO_KVB_LSSF, 0);
			KVB_EnableBlinkLSSF(1);
			break;
		case LSSGKCU_KVB_LSSF_ON:
			KVB_EnableBlinkLSSF(0);
			GPIO_Write(&GPIO_KVB_LSSF, 1);
			break;
		case LSSGKCU_KVB_LSSF_OFF:
			KVB_EnableBlinkLSSF(0);
			GPIO_Write(&GPIO_KVB_LSSF, 0);
			break;
		case LSSGKCU_KVB_YG_OFF:
			KVB_DisplayOff();
			break;
		case LSSGKCU_KVB_YG_PA400:
			KVB_Display(KVB_PA400_TEXT);
			break;
		case LSSGKCU_KVB_YG_UC512:
			break;
		case LSSGKCU_KVB_YG_888:
			break;
		case LSSGKCU_KVB_YG_DASH:
			break;
		case LSSGKCU_KVB_G_B:
			break;
		case LSSGKCU_KVB_Y_B:
			break;
		case LSSGKCU_KVB_G_P:
			break;
		case LSSGKCU_KVB_Y_P:
			break;
		case LSSGKCU_KVB_G_L:
			break;
		case LSSGKCU_KVB_Y_L:
			break;
		case LSSGKCU_KVB_G_00:
			break;
		case LSSGKCU_KVB_Y_00:
			break;
		case LSSGKCU_KVB_G_000:
			break;
		case LSSGKCU_KVB_Y_000:
			break;
		default:
			// Unknown command.
			break;
		}
	}
}

/*** LSSGKCU functions ***/

/* INIT LSSGKCU COMMUNICATION MODULE.
 * @param:	None.
 * @return:	None.
 */
void LSSGKCU_Init(void) {

	/* Init context */
	unsigned int i = 0;
	for(i=0 ; i<LSSGKCU_RX_BUFFER_SIZE ; i++) lssgkcu_ctx.rx_buf[i] = 0;
	lssgkcu_ctx.rx_write_idx = 0;
	lssgkcu_ctx.rx_read_idx = 0;
}

/* FILL LSSGKCU BUFFER (CALLED BY USART2 INTERRUPT HANDLER).
 * @param lssgkcu_cmd:	The new LSSGKCU command to store.
 * @return:			None.
 */
void LSSGKCU_FillRxBuffer(unsigned char lssgkcu_cmd) {
	lssgkcu_ctx.rx_buf[lssgkcu_ctx.rx_write_idx] = lssgkcu_cmd;
	lssgkcu_ctx.rx_write_idx++;
	// Roll-over management.
	if (lssgkcu_ctx.rx_write_idx == LSSGKCU_RX_BUFFER_SIZE) {
		lssgkcu_ctx.rx_write_idx = 0;
	}
}

/* SEND AN LSSGKCU COMMAND TO SGKCU.
 * @param lssgkcu_cmd: 	LSSGKCU command (byte) to transmit.
 * @return: 			None.
 */
void LSSGKCU_Send(unsigned char lssgkcu_cmd) {
	USART2_SendByte(lssgkcu_cmd, USART_FORMAT_ASCII);
}

/* MAIN ROUTINE OF LSSGKCU COMMAND MANAGER.
 * @param:	None.
 * @return:	None.
 */
void LSSGKCU_Task(void) {
	// LSSGKCU routine.
	if (lssgkcu_ctx.rx_read_idx != lssgkcu_ctx.rx_write_idx) {
		LSSGKCU_Decode();
		// Increment read index and manage roll-over.
		lssgkcu_ctx.rx_read_idx++;
		if (lssgkcu_ctx.rx_read_idx == LSSGKCU_RX_BUFFER_SIZE) {
			lssgkcu_ctx.rx_read_idx = 0;
		}
	}
}
