/*
 * scb_registers.h
 *
 *  Created on: 25 dec. 2021
 *      Author: Ludo
 */

#ifndef __SCB_REGISTERS_H__
#define __SCB_REGISTERS_H__

#include "stdint.h"

/*** SCB REGISTERS macros ***/

// Peripheral base address.
#define SCB     ((SCB_registers_t*) ((uint32_t) 0xE000ED00))

/*** SCB REGISTERS structures ***/

/*!******************************************************************
 * \enum SCB_registers_t
 * \brief SCB registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t CPUID;
    volatile uint32_t ICSR;
    volatile uint32_t VTOR;
    volatile uint32_t AIRCR;
    volatile uint32_t SCR;
    volatile uint32_t CCR;
    volatile uint32_t SHPR1;
    volatile uint32_t SHPR2;
    volatile uint32_t SHPR3;
    volatile uint32_t SHCRS;
    volatile uint32_t CFSR;
    volatile uint32_t HFSR;
    volatile uint32_t MMAR;
    volatile uint32_t BFAR;
} SCB_registers_t;

#endif /* __SCB_REGISTERS_H__ */
