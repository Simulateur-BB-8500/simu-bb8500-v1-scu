/*
 * tim.h
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_TIME_TIM_H_
#define PERIPHERALS_TIME_TIM_H_

#include "enum.h"
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

// Timer 1 and 2 used to count milliseconds since start-up.
void TIM_InitMs(void);
unsigned int TIM_GetMs(void);
void TIM_DelayMs(unsigned int msToWait);
// Generic functions.
void TIM_Init(TIM_BaseAddress* TIM, unsigned int duration, Time_Unit unit, boolean interruptEnable);
void TIM_Start(TIM_BaseAddress* TIM, boolean reset);
void TIM_Stop(TIM_BaseAddress* TIM, boolean reset);


#endif /* PERIPHERALS_TIME_TIM_H_ */
