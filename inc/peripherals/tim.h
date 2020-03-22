/*
 * tim.h
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludo
 */

#ifndef TIM_H
#define TIM_H

/*** TIM functions ***/

// Milliseconds count.
void TIM2_Init(void);
unsigned int TIM2_GetMs(void);
void TIM2_DelayMs(unsigned ms_to_wait);

// Tachro step timer
void TIM5_Init(void);
void TIM5_Start(void);
void TIM5_Stop(void);
void TIM5_SetDelayUs(unsigned int arr_value);
unsigned char TIM5_GetUifFlag(void);
void TIM5_ClearUifFlag(void);

// KVB.
void TIM6_Init(void);
void TIM6_Start(void);
void TIM6_Stop(void);

// Manometers.
void TIM7_Init(void);
void TIM7_Start(void);
void TIM7_Stop(void);

// LVAL PWM.
void TIM8_Init(void);
void TIM8_SetDutyCycle(unsigned char duty_cycle);
void TIM8_Start(void);
void TIM8_Stop(void);

#endif /* TIM_H */
