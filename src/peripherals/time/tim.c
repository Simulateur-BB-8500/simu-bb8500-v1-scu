/*
 * tim.c
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludovic
 */

#include "gpio.h"
#include "mapping.h"
#include "mask.h"
#include "tim.h"
#include "tim_reg.h"

/* CONFIGURE TIM6.
 * @param: None.
 * @return: None.
 */
void TIM6_Init(void) {
	TIM6 -> SR &= ~BIT_MASK[0]; // Clear update event flag (SR = '0').
	TIM6 -> PSC = 15999; // TIM6 clock = SYSCLK/(15999+1) = 1kHz.
	TIM6 -> ARR = 1000; // 1000 fronts at 1kHz = 1 second.
	TIM6 -> DIER |= BIT_MASK[0]; // Enable interrupt (UIE = '1').
}

/* START TIM6.
 * @param: None.
 * @return: None.
 */
void TIM6_Start(void) {
	TIM6 -> CR1 |= BIT_MASK[0];
}

/* TIM6 INTERRUPT HANDLE.
 * @param: None.
 * @return: None.
 */
void TIM6_Handler(void) {
	TIM6 -> SR &= ~BIT_MASK[0]; // Clear update event flag.
	GPIO_Toggle(LED);
	TIM6_Start();
}
