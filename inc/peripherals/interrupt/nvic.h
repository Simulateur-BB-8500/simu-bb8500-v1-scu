/*
 * nvic.h
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_INTERRUPT_NVIC_H_
#define PERIPHERALS_INTERRUPT_NVIC_H_

#include "nvic_reg.h"

/*** NVIC functions ***/

void NVIC_EnableInterrupt(Interrupt ITNum);
void NVIC_DisableInterrupt(Interrupt ITNum);
void NVIC_Init(void);

#endif /* PERIPHERALS_INTERRUPT_NVIC_H_ */
