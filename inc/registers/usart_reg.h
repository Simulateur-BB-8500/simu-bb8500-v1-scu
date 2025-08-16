/*
 * usart_reg.h
 *
 *  Created on: 28 sep. 2017
 *      Author: Ludo
 */

#ifndef __USART_REG_H__
#define __USART_REG_H__

#include "stdint.h"

/*** USART REG macros ***/

// Peripherals base address.
#define USART1  ((USART_registers_t*) ((uint32_t) 0x40011000))
#define USART2  ((USART_registers_t*) ((uint32_t) 0x40004400))
#define USART3  ((USART_registers_t*) ((uint32_t) 0x40004800))
#define UART4   ((USART_registers_t*) ((uint32_t) 0x40004C00))
#define UART5   ((USART_registers_t*) ((uint32_t) 0x40005000))
#define USART6  ((USART_registers_t*) ((uint32_t) 0x40011400))
#define UART7   ((USART_registers_t*) ((uint32_t) 0x40007800))
#define UART8   ((USART_registers_t*) ((uint32_t) 0x40007C00))

/*** USART REG structures ***/

/*!******************************************************************
 * \enum USART_registers_t
 * \brief USART registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t BRR;
    volatile uint32_t GTPR;
    volatile uint32_t RTOR;
    volatile uint32_t RQR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t RDR;
    volatile uint32_t TDR;
} USART_registers_t;

#endif /* __USART_REG_H__ */
