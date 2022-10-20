/*
 * flash_reg.h
 *
 *  Created on: 14 jul. 2018
 *      Author: Ludo
 */

#ifndef __FLASH_REG_H__
#define __FLASH_REG_H__

/*** FLASH interface registers ***/

typedef struct {
	volatile unsigned int ACR;    	// FLASH interface access control register.
	volatile unsigned int KEYR;    	// FLASH interface key register.
	volatile unsigned int OPTKEYR;  // FLASH interface option key register.
	volatile unsigned int SR;    	// FLASH interface status register.
	volatile unsigned int CR;    	// FLASH interface control register.
	volatile unsigned int OPTCR;    // FLASH interface option control register.
	volatile unsigned int OPTCR1;	// FLASH interface option control register.
} FLASH_base_address_t;

/*** FLASH interface base address ***/

#define FLASH	((FLASH_base_address_t*) ((unsigned int) 0x40023C00))

#endif /* __FLASH_REG_H__ */
