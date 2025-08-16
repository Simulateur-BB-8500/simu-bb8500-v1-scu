/*
 * nvic_registers.h
 *
 *  Created on: 16 sep. 2017
 *      Author: Ludo
 */

#ifndef __NVIC_REG_H__
#define __NVIC_REG_H__

#include "stdint.h"

/*** NVIC REGISTERS macros ***/

// Peripheral base address.
#define NVIC    ((NVIC_registers_t*) ((uint32_t) 0xE000E100))

/*** NVIC REGISTERS structures ***/

/*!******************************************************************
 * \enum NVIC_registers_t
 * \brief NVIC registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t ISER[8];
    volatile uint32_t RESERVED0[24];
    volatile uint32_t ICER[8];
    volatile uint32_t RESERVED1[24];
    volatile uint32_t ISPR[8];
    volatile uint32_t RESERVED2[24];
    volatile uint32_t ICPR[8];
    volatile uint32_t RESERVED3[24];
    volatile uint32_t IABR[8];
    volatile uint32_t RESERVED4[56];
    volatile uint32_t IPR[60];
    volatile uint32_t RESERVED5[644];
    volatile uint32_t STIR;
} NVIC_registers_t;

#endif /* __NVIC_REG_H__ */
