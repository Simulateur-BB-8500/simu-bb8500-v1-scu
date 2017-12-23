/*
 * tim.h
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_TIME_TIM_H_
#define PERIPHERALS_TIME_TIM_H_

#include "masks.h"
#include "tim_reg.h"

/*** TIM #define ***/

#define TIM_MIN_MICROSECONDS 1
#define TIM_MAX_MICROSECONDS 999
#define TIM_MIN_MILLISECONDS 1
#define TIM_MAX_MILLISECONDS 999
#define TIM_MIN_SECONDS 1
#define TIM_MAX_SECONDS 262 // 263*250 > 2^(16)-1.

/*** TIM functions ***/

void TIM_Set(TIM_BaseAddress* TIM, unsigned int duration, TimeUnit unit);
void TIM_Start(TIM_BaseAddress* TIM, boolean reset);
void TIM_Stop(TIM_BaseAddress* TIM, boolean reset);
void TIM_EnableInterrupt(TIM_BaseAddress* TIM);
void TIM_ClearFlag(TIM_BaseAddress* tim);
void TIM_InitMs(void);
unsigned int TIM_GetMs(void);
void TIM_DelayMs(unsigned int msToWait);

#endif /* PERIPHERALS_TIME_TIM_H_ */
