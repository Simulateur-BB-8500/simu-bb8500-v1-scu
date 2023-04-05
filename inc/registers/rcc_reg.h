/*
 * rcc_reg.h
 *
 *  Created on: 9 sept. 2017
 *      Author: Ludo
 */

#ifndef __RCC_REG_H__
#define __RCC_REG_H__

#include "stdint.h"

/*** RCC registers ***/

typedef struct {
	volatile uint32_t CR;			// RCC clock control register.
	volatile uint32_t PLLCFGR;    	// RCC PLL configuration register.
	volatile uint32_t CFGR;      	// RCC clock configuration register.
	volatile uint32_t CIR;       	// RCC clock interrupt register.
	volatile uint32_t AHB1RSTR;  	// RCC AHB1 peripheral reset register.
	volatile uint32_t AHB2RSTR;  	// RCC AHB2 peripheral reset register.
	volatile uint32_t AHB3RSTR;  	// RCC AHB3 peripheral reset register.
	uint32_t RESERVED0; 			// Reserved 0x1C.
	volatile uint32_t APB1RSTR; 	// RCC APB1 peripheral reset register.
	volatile uint32_t APB2RSTR; 	// RCC APB2 peripheral reset register.
	uint32_t RESERVED1; 			// Reserved 0x28.
	uint32_t RESERVED2; 			// Reserved 0x2C.
	volatile uint32_t AHB1ENR;  	// RCC AHB1 peripheral clock register.
	volatile uint32_t AHB2ENR;  	// RCC AHB2 peripheral clock register.
	volatile uint32_t AHB3ENR; 		// RCC AHB3 peripheral clock register.
	uint32_t RESERVED3; 			// Reserved 0x3C.
	volatile uint32_t APB1ENR;  	// RCC APB1 peripheral clock enable register.
	volatile uint32_t APB2ENR;  	// RCC APB2 peripheral clock enable register.
	uint32_t RESERVED4; 			// Reserved 0x48.
	uint32_t RESERVED5; 			// Reserved 0x4C.
	volatile uint32_t AHB1LPENR;	// RCC AHB1 peripheral clock enable in low power mode register.
	volatile uint32_t AHB2LPENR; 	// RCC AHB2 peripheral clock enable in low power mode register.
	volatile uint32_t AHB3LPENR; 	// RCC AHB3 peripheral clock enable in low power mode register.
	uint32_t RESERVED6; 			// Reserved 0x5C.
	volatile uint32_t APB1LPENR;	// RCC APB1 peripheral clock enable in low power mode register.
	volatile uint32_t APB2LPENR; 	// RCC APB2 peripheral clock enable in low power mode register.
	uint32_t RESERVED7; 			// Reserved 0x68.
	uint32_t RESERVED8; 			// Reserved 0x6C.
	volatile uint32_t BDCR;     	// RCC Backup domain control register.
	volatile uint32_t CSR;      	// RCC clock control & status register.
	uint32_t RESERVED9;				// Reserved 0x78.
	uint32_t RESERVED10; 			// Reserved 0x7C.
	volatile uint32_t SSCGR;    	// RCC spread spectrum clock generation register.
	volatile uint32_t PLLI2SCFGR;	// RCC PLLI2S configuration register.
	volatile uint32_t PLLSAICFGR;	// RCC PLLSAI configuration register.
	volatile uint32_t DKCFGR1;		// RCC Dedicated Clocks configuration register 1.
	volatile uint32_t DKCFGR2;		// RCC Dedicated Clocks configuration register 2.
} RCC_registers_t;

/*** RCC base address ***/

#define RCC		((RCC_registers_t*) ((uint32_t) 0x40023800))

#endif /* __RCC_REG_H__ */
