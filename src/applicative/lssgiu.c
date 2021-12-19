/*
 * lssgiu.c
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludo
 */

#include "lssgiu.h"

#include "kvb.h"
#include "gpio.h"
#include "lsmcu.h"
#include "mapping.h"
#include "tch.h"
#include "usart.h"

/*** LSSGIU local macros ***/

#define LSSGIU_RX_BUFFER_SIZE	32

/*** LSSGIU local structures ***/

typedef struct {
	unsigned char rx_buf[LSSGIU_RX_BUFFER_SIZE];
	unsigned int rx_write_idx;
	unsigned int rx_read_idx;
} LSSGIU_Context;

/*** LSSGIU external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** LSSGIU local global variables ***/

static LSSGIU_Context lssgiu_ctx;

/*** LSSGIU local functions ***/

/* DECODE THE CURRENT LSSGIU COMMAND.
 * @param:	None.
 * @return: None.
 */
void LSSGIU_Decode(void) {
	unsigned char ls_cmd = lssgiu_ctx.rx_buf[lssgiu_ctx.rx_read_idx];
	if (ls_cmd <= TCH_SPEED_MAX_KMH) {
		// Save speed in main context.
		lsmcu_ctx.speed_kmh = ls_cmd;
	}
	else {
		// Decode LSSGIU command.
		switch(ls_cmd) {
		case LSMCU_IN_KVB_LVAL_BLINK:
			GPIO_Write(&GPIO_KVB_LVAL, 0);
			KVB_EnableBlinkLVAL(1);
			break;
		case LSMCU_IN_KVB_LVAL_ON:
			KVB_EnableBlinkLVAL(0);
			GPIO_Write(&GPIO_KVB_LVAL, 1);
			break;
		case LSMCU_IN_KVB_LVAL_OFF:
			KVB_EnableBlinkLVAL(0);
			GPIO_Write(&GPIO_KVB_LVAL, 0);
			break;
		case LSMCU_IN_KVB_LMV_ON:
			GPIO_Write(&GPIO_KVB_LMV, 1);
			break;
		case LSMCU_IN_KVB_LMV_OFF:
			GPIO_Write(&GPIO_KVB_LMV, 0);
			break;
		case LSMCU_IN_KVB_LFC_ON:
			GPIO_Write(&GPIO_KVB_LFC, 1);
			break;
		case LSMCU_IN_KVB_LFC_OFF:
			GPIO_Write(&GPIO_KVB_LFC, 0);
			break;
		case LSMCU_IN_KVB_LV_ON:
			GPIO_Write(&GPIO_KVB_LV, 1);
			break;
		case LSMCU_IN_KVB_LV_OFF:
			GPIO_Write(&GPIO_KVB_LV, 0);
			break;
		case LSMCU_IN_KVB_LFU_ON:
			GPIO_Write(&GPIO_KVB_LFU, 1);
			break;
		case LSMCU_IN_KVB_LFU_OFF:
			GPIO_Write(&GPIO_KVB_LFU, 0);
			break;
		case LSMCU_IN_KVB_LPS_ON:
			GPIO_Write(&GPIO_KVB_LPS, 1);
			break;
		case LSMCU_IN_KVB_LPS_OFF:
			GPIO_Write(&GPIO_KVB_LPS, 0);
			break;
		case LSMCU_IN_KVB_LSSF_BLINK:
			GPIO_Write(&GPIO_KVB_LSSF, 0);
			KVB_EnableBlinkLSSF(1);
			break;
		case LSMCU_IN_KVB_LSSF_ON:
			KVB_EnableBlinkLSSF(0);
			GPIO_Write(&GPIO_KVB_LSSF, 1);
			break;
		case LSMCU_IN_KVB_LSSF_OFF:
			KVB_EnableBlinkLSSF(0);
			GPIO_Write(&GPIO_KVB_LSSF, 0);
			break;
		case LSMCU_IN_KVB_YG_OFF:
			KVB_DisplayOff();
			break;
		case LSMCU_IN_KVB_YG_PA400:
			KVB_Display(KVB_YG_PA400);
			break;
		case LSMCU_IN_KVB_YG_UC512:
			KVB_Display(KVB_YG_UC512);
			break;
		case LSMCU_IN_KVB_YG_888:
			KVB_Display(KVB_YG_888);
			break;
		case LSMCU_IN_KVB_YG_DASH:
			KVB_Display(KVB_YG_DASH);
			break;
		case LSMCU_IN_KVB_G_B:
			KVB_Display(KVB_G_B);
			break;
		case LSMCU_IN_KVB_Y_B:
			KVB_Display(KVB_Y_B);
			break;
		case LSMCU_IN_KVB_G_P:
			KVB_Display(KVB_G_P);
			break;
		case LSMCU_IN_KVB_Y_P:
			KVB_Display(KVB_Y_P);
			break;
		case LSMCU_IN_KVB_G_L:
			KVB_Display(KVB_G_L);
			break;
		case LSMCU_IN_KVB_Y_L:
			KVB_Display(KVB_Y_L);
			break;
		case LSMCU_IN_KVB_G_00:
			KVB_Display(KVB_G_00);
			break;
		case LSMCU_IN_KVB_Y_00:
			KVB_Display(KVB_Y_00);
			break;
		case LSMCU_IN_KVB_G_000:
			KVB_Display(KVB_G_000);
			break;
		case LSMCU_IN_KVB_Y_000:
			KVB_Display(KVB_Y_000);
			break;
		default:
			// Unknown command.
			break;
		}
	}
}

/*** LSSGIU functions ***/

/* INIT LSSGIU COMMUNICATION MODULE.
 * @param:	None.
 * @return:	None.
 */
void LSSGIU_Init(void) {
	// Init context.
	unsigned int i = 0;
	for(i=0 ; i<LSSGIU_RX_BUFFER_SIZE ; i++) lssgiu_ctx.rx_buf[i] = 0;
	lssgiu_ctx.rx_write_idx = 0;
	lssgiu_ctx.rx_read_idx = 0;
}

/* FILL LSSGIU BUFFER (CALLED BY USART2 INTERRUPT HANDLER).
 * @param lssgiu_cmd:	The new LSSGIU command to store.
 * @return:			None.
 */
void LSSGIU_FillRxBuffer(unsigned char ls_cmd) {
	lssgiu_ctx.rx_buf[lssgiu_ctx.rx_write_idx] = ls_cmd;
	lssgiu_ctx.rx_write_idx++;
	// Roll-over management.
	if (lssgiu_ctx.rx_write_idx == LSSGIU_RX_BUFFER_SIZE) {
		lssgiu_ctx.rx_write_idx = 0;
	}
}

/* SEND AN LSSGIU COMMAND TO SGKCU.
 * @param lssgiu_cmd: 	LSSGIU command (byte) to transmit.
 * @return: 			None.
 */
void LSSGIU_Send(unsigned char ls_cmd) {
	USART1_SendByte(ls_cmd, USART_FORMAT_ASCII);
}

/* MAIN ROUTINE OF LSSGIU COMMAND MANAGER.
 * @param:	None.
 * @return:	None.
 */
void LSSGIU_Task(void) {
	// LSSGIU routine.
	if (lssgiu_ctx.rx_read_idx != lssgiu_ctx.rx_write_idx) {
		LSSGIU_Decode();
		// Increment read index and manage roll-over.
		lssgiu_ctx.rx_read_idx++;
		if (lssgiu_ctx.rx_read_idx == LSSGIU_RX_BUFFER_SIZE) {
			lssgiu_ctx.rx_read_idx = 0;
		}
	}
}
