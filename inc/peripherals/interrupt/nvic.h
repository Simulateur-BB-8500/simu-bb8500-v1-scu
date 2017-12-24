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

void NVIC_EnableInterrupt(InterruptVector ITNum);
void NVIC_DisableInterrupt(InterruptVector ITNum);
void NVIC_SetPriority(InterruptVector ITNum, unsigned char priority);

#endif /* PERIPHERALS_INTERRUPT_NVIC_H_ */
