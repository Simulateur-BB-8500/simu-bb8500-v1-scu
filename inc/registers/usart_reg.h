/*
 * usart_reg.h
 *
 *  Created on: 28 sept. 2017
 *      Author: Ludo
 */

#ifndef USART_REG_H
#define USART_REG_H

/*** USARTx registers ***/

typedef struct {
	volatile unsigned int CR1;    	// USART control register 1.
	volatile unsigned int CR2;    	// USART control register 2.
	volatile unsigned int CR3;    	// USART control register 3.
	volatile unsigned int BRR;    	// USART baud rate register.
	volatile unsigned int GTPR;    	// USART guard time and prescaler register.
	volatile unsigned int RTOR;    	// USART receiver timeout register.
	volatile unsigned int RQR;    	// USART request register.
	volatile unsigned int ISR;    	// USART interrupt and status register.
	volatile unsigned int ICR;    	// USART interrupt flag clear register.
	volatile unsigned int RDR;    	// USART receive data register.
	volatile unsigned int TDR;    	// USART transmit data register.
} USART_BaseAddress;

/*** USARTx base addresses ***/

#define USART1	((USART_BaseAddress*) ((unsigned int) 0x40011000))
#define USART2	((USART_BaseAddress*) ((unsigned int) 0x40004400))
#define USART3	((USART_BaseAddress*) ((unsigned int) 0x40004800))
#define UART4	((USART_BaseAddress*) ((unsigned int) 0x40004C00))
#define UART5	((USART_BaseAddress*) ((unsigned int) 0x40005000))
#define USART6	((USART_BaseAddress*) ((unsigned int) 0x40011400))
#define UART7	((USART_BaseAddress*) ((unsigned int) 0x40007800))
#define UART8	((USART_BaseAddress*) ((unsigned int) 0x40007C00))

#endif /* USART_REG_H */
