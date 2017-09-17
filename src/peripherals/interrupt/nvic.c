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
}
