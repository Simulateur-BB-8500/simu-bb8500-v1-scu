/*
 * at.h
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_USART_AT_H_
#define PERIPHERALS_USART_AT_H_

/*** AT commands functions ***/

void AT_DecodeSGKCU(unsigned char atCommand);
void AT_SendSGKCU(unsigned char atCommand);

#endif /* PERIPHERALS_USART_AT_H_ */
