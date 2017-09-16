/*
 * rcc_reg.h
 *
 *  Created on: 9 sept. 2017
 *      Author: Ludovic
 */

#ifndef REGISTERS_RCC_REG_H_
#define REGISTERS_RCC_REG_H_

/*** RCC registers ***/

typedef struct {
  volatile unsigned int CR;			// RCC clock control register.
  volatile unsigned int PLLCFGR;    // RCC PLL configuration register.
  volatile unsigned int CFGR;      	// RCC clock configuration register.
  volatile unsigned int CIR;       	// RCC clock interrupt register.
  volatile unsigned int AHB1RSTR;  	// RCC AHB1 peripheral reset register.
  volatile unsigned int AHB2RSTR;  	// RCC AHB2 peripheral reset register.
  volatile unsigned int AHB3RSTR;  	// RCC AHB3 peripheral reset register.
  unsigned int RESERVED0; 			// Reserved 0x1C.
  volatile unsigned int APB1RSTR; 	// RCC APB1 peripheral reset register.
  volatile unsigned int APB2RSTR; 	// RCC APB2 peripheral reset register.
  unsigned int RESERVED1; 			// Reserved 0x28.
  unsigned int RESERVED2; 			// Reserved 0x2C.
  volatile unsigned int AHB1ENR;  	// RCC AHB1 peripheral clock register.
  volatile unsigned int AHB2ENR;  	// RCC AHB2 peripheral clock register.
  volatile unsigned int AHB3ENR; 	// RCC AHB3 peripheral clock register.
  unsigned int RESERVED3; 			// Reserved 0x3C.
  volatile unsigned int APB1ENR;  	// RCC APB1 peripheral clock enable register.
  volatile unsigned int APB2ENR;  	// RCC APB2 peripheral clock enable register.
  unsigned int RESERVED4; 			// Reserved 0x48.
  unsigned int RESERVED5; 			// Reserved 0x4C.
  volatile unsigned int AHB1LPENR;	// RCC AHB1 peripheral clock enable in low power mode register.
  volatile unsigned int AHB2LPENR; 	// RCC AHB2 peripheral clock enable in low power mode register.
  volatile unsigned int AHB3LPENR; 	// RCC AHB3 peripheral clock enable in low power mode register.
  unsigned int RESERVED6; 			// Reserved 0x5C.
  volatile unsigned int APB1LPENR;	// RCC APB1 peripheral clock enable in low power mode register.
  volatile unsigned int APB2LPENR; 	// RCC APB2 peripheral clock enable in low power mode register.
  unsigned int RESERVED7; 			// Reserved 0x68.
  unsigned int RESERVED8; 			// Reserved 0x6C.
  volatile unsigned int BDCR;     	// RCC Backup domain control register.
  volatile unsigned int CSR;      	// RCC clock control & status register.
  unsigned int RESERVED9;			// Reserved 0x78.
  unsigned int RESERVED10; 			// Reserved 0x7C.
  volatile unsigned int SSCGR;    	// RCC spread spectrum clock generation register.
  volatile unsigned int PLLI2SCFGR;	// RCC PLLI2S configuration register.
  volatile unsigned int PLLSAICFGR;	// RCC PLLSAI configuration register.
  volatile unsigned int DCKCFGR1;	// RCC Dedicated Clocks configuration register 1.
  volatile unsigned int DCKCFGR2;	// RCC Dedicated Clocks configuration register 2.
} RCC_BaseAddress;

/*** RCC base address ***/

#define RCC		((RCC_BaseAddress*) ((unsigned int) 0x40023800))

#endif /* REGISTERS_RCC_REG_H_ */
