/*
 * nvic.c
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludovic
 */

#include "masks.h"
#include "nvic.h"
#include "nvic_reg.h"

/*** NVIC functions ***/

/* ENABLE AN INTERRUPT LINE.
 * @param ITNum: Interrupt number (0 to 97).
 * @return: None.
 */
void NVIC_EnableInterrupt(Interrupt ITNum) {
	// (ITNum >> 5) selects the proper register (ISER0 to ISER7) acting as a modulo.
	// (ITNum & 0x0000001F) selects the proper bit to set.
	NVIC -> ISER[ITNum >> 5] |= BIT_MASK[(ITNum & 0x0000001F)];
}

/* DISABLE AN INTERRUPT LINE.
 * @param ITNum: Interrupt number (0 to 97).
 * @return: None.
 */
void NVIC_DisableInterrupt(Interrupt ITNum) {
	// (ITNum >> 5) selects the proper register (ISER0 to ISER7) acting as a modulo.
	// (ITNum & 0x0000001F) selects the proper bit to set.
	NVIC -> ICER[ITNum >> 5] |= BIT_MASK[(ITNum & 0x0000001F)];
}

/* CONFIGURE MCU INTERRUPT SYSTEM.
 * @param: None.
 * @return: None.
 */
void NVIC_Init(void) {
	NVIC_EnableInterrupt(TIM6_DAC);
	NVIC_EnableInterrupt(TIM7);
	NVIC_EnableInterrupt(ADC);
}

/* @NOTE:
 * To add an interrupt from a given peripheral:
 * 		1) nvic.c, NVIC_Init(): add 'NVIC_EnableInterrupt(<IT>);'
 * 			where <IT> is the interrupt number in NVIC (specified in 'Interrupt' enumeration, see nvic_reg.h).
 * 		2) Configure and enable interrupts in the proper(s) peripheral register(s).
 * 		3) startup_stm32.s, g_pfnVectors: add '.word <handlerFunction>' at the line corresponding to <IT>.
 * 		4) startup_stm32.s, g_pfnVectors: add '.weak <handlerFunction>
											   .thumb_set <handlerFunction>,Default_Handler'.
		5) Implement 'void <handlerFunction>(void)' in the peripheral source file.
 */
