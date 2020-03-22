/*
 * flash_reg.h
 *
 *  Created on: 14 jul. 2018
 *      Author: Ludo
 */

#ifndef FLASH_REG_H
#define FLASH_REG_H

/*** FLASH interface registers ***/

typedef struct {
	volatile unsigned int ACR;    	// FLASH interface access control register.
	volatile unsigned int KEYR;    	// FLASH interface key register.
	volatile unsigned int OPTKEYR;  // FLASH interface option key register.
	volatile unsigned int SR;    	// FLASH interface status register.
	volatile unsigned int CR;    	// FLASH interface control register.
	volatile unsigned int OPTCR;    // FLASH interface option control register.
	volatile unsigned int OPTCR1;	// FLASH interface option control register.
} FLASH_BaseAddress;

/*** FLASH interface base address ***/

#define FLASH	((FLASH_BaseAddress*) ((unsigned int) 0x40023C00))

#endif /* FLASH_REG_H */
