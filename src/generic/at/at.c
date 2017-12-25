/*
 * at.c
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludovic
 */

#include "at.h"
#include "atCommands.h"
#include "enum.h"
#include "gpio.h"
#include "mapping.h"
#include "usart.h"

/*** AT commands functions ***/

/* DECODE AN AT COMMAND FROM SGKCU.
 * @param atCommand: 	Received AT command (byte).
 * @return: 			None.
 */
void AT_DecodeSGKCU(unsigned char atCommand) {
	if (atCommand <= AT_SPEED_MAX) {
		// Transmit speed to Tachro and VACMA modules.
	}
	else {
		// Decode AT command.
		switch(atCommand) {
		case AT_KVB_LVAL_BLINK:
			GPIO_Write(LED2, HIGH);
			break;
		default:
			// Unknown command.
			break;
		}
	}
}

/* SEND AN AT COMMAND TO SGKCU.
 * @param atCommand: 	AT command (byte) to transmit.
 * @return: 			None.
 */
void AT_SendSGKCU(unsigned char atCommand) {
	USART_SendByte(USART_SGKCU, atCommand, ASCII);
}
