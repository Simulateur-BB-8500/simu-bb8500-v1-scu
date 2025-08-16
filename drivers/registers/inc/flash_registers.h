/*
 * flash_registers.h
 *
 *  Created on: 14 jul. 2018
 *      Author: Ludo
 */

#ifndef __FLASH_REG_H__
#define __FLASH_REG_H__

#include "stdint.h"

/*** FLASH REGISTERS macros ***/

// Peripheral base address.
#define FLASH   ((FLASH_registers_t*) ((uint32_t) 0x40023C00))

/*** FLASH REGISTERS structures ***/

/*!******************************************************************
 * \enum FLASH_registers_t
 * \brief FLASH registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t ACR;
    volatile uint32_t KEYR;
    volatile uint32_t OPTKEYR;
    volatile uint32_t SR;
    volatile uint32_t CR;
    volatile uint32_t OPTCR;
    volatile uint32_t OPTCR1;
} FLASH_registers_t;

#endif /* __FLASH_REG_H__ */
