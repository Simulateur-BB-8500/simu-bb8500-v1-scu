/*
 * dac_registers.h
 *
 *  Created on: 18 sep. 2017
 *      Author: Ludo
 */

#ifndef __DAC_REGISTERS_H__
#define __DAC_REGISTERS_H__

#include "stdint.h"

/*** DAC REGISTERS macros ***/

// Peripheral base address.
#define DAC     ((DAC_registers_t*) ((uint32_t) 0x40007400))

/*** DAC REGISTERS structures ***/

/*!******************************************************************
 * \enum DAC_registers_t
 * \brief DAC registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t SWTRIGR;
    volatile uint32_t DHR12R1;
    volatile uint32_t DHR12L1;
    volatile uint32_t DHR8R1;
    volatile uint32_t DHR12R2;
    volatile uint32_t DHR12L2;
    volatile uint32_t DHR8R2;
    volatile uint32_t DHR12RD;
    volatile uint32_t DHR12LD;
    volatile uint32_t DHR8RD;
    volatile uint32_t DOR1;
    volatile uint32_t DOR2;
    volatile uint32_t SR;
} DAC_registers_t;

#endif /* __DAC_REGISTERS_H__ */
