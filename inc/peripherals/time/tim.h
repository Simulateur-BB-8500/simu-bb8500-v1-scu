/*
 * tim.h
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_TIME_TIM_H_
#define PERIPHERALS_TIME_TIM_H_

#include "enum.h"
#include "tim_reg.h"

/*** TIM #define ***/

#define TIM_MIN_MICROSECONDS 1
#define TIM_MAX_MICROSECONDS 999
#define TIM_MIN_MILLISECONDS 1
#define TIM_MAX_MILLISECONDS 999
#define TIM_MIN_SECONDS 1
#define TIM_MAX_SECONDS 262 // 263*250 > 2^(16)-1.

/*** TIM functions ***/

// Generic functions.
void TIM_Init(TIM_BaseAddress* TIM, unsigned int duration, Time_Unit unit, boolean interruptEnable);
void TIM_SetDuration(TIM_BaseAddress* TIM, unsigned int duration, Time_Unit unit);
void TIM_Start(TIM_BaseAddress* TIM, boolean reset);
void TIM_Stop(TIM_BaseAddress* TIM, boolean reset);
void TIM_ClearFlag(TIM_BaseAddress* TIM);
void PWM_Init(TIM_BaseAddress* TIM, unsigned int period, Time_Unit unit);
void PWM_SetDutyCycle(TIM_BaseAddress* TIM, PWM_Channel pwmChannel, unsigned int dutyCycle);
void PWM_Start(TIM_BaseAddress* TIM, boolean reset);
void PWM_Stop(TIM_BaseAddress* TIM, boolean reset);

// Timer 1 and 2 used to count milliseconds since start-up.
void TIM_InitMs(void);
unsigned int TIM_GetMs(void);
void TIM_DelayMs(unsigned int msToWait);

#endif /* PERIPHERALS_TIME_TIM_H_ */
