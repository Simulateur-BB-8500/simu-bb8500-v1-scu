/*
 * flash_reg.h
 *
 *  Created on: 14 jul. 2018
 *      Author: Ludo
 */

#ifndef __FLASH_REG_H__
#define __FLASH_REG_H__

#include "stdint.h"

/*** FLASH interface registers ***/

typedef struct {
	volatile uint32_t ACR;    	// FLASH interface access control register.
	volatile uint32_t KEYR;    	// FLASH interface key register.
	volatile uint32_t OPTKEYR;  // FLASH interface option key register.
	volatile uint32_t SR;    	// FLASH interface status register.
	volatile uint32_t CR;    	// FLASH interface control register.
	volatile uint32_t OPTCR;    // FLASH interface option control register.
	volatile uint32_t OPTCR1;	// FLASH interface option control register.
} FLASH_registers_t;

/*** FLASH interface base address ***/

#define FLASH	((FLASH_registers_t*) ((uint32_t) 0x40023C00))

#endif /* __FLASH_REG_H__ */
