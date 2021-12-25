/*
 * scb_reg.h
 *
 *  Created on: 25 déc. 2021
 *      Author: Ludovic
 */

#ifndef SCB_REG_H
#define SCB_REG_H_

/*** SCB registers ***/

typedef struct {
	volatile unsigned int CPUID;	// SCB CPU ID base register.
	volatile unsigned int ICSR;		// SCB interrupt control and state register.
	volatile unsigned int VTOR;		// SCB vector table offset register.
	volatile unsigned int AIRCR;	// SCB application interrupt and reset control register.
	volatile unsigned int SCR;		// SCB system control register.
	volatile unsigned int CCR;		// SCB configuration and control register.
	volatile unsigned int SHPR1;	// SCB system handler priority register 1.
	volatile unsigned int SHPR2;	// SCB system handler priority register 2.
	volatile unsigned int SHPR3;	// SCB system handler priority register 3.
	volatile unsigned int SHCRS;	// SCB system handler control and state register.
	volatile unsigned int CFSR;		// SCB configurable fault status register.
	volatile unsigned int HFSR;		// SCB hard fault status register.
	volatile unsigned int MMAR;		// SCB memory manage fault address register.
	volatile unsigned int BFAR;		// SCB bus fault address register.
} SCB_BaseAddress;

/*** SCB base address ***/

#define SCB		((SCB_BaseAddress*) ((unsigned int) 0xE000ED00))

#endif /* SCB_REG_H */
