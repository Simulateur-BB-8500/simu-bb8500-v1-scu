/*
 * tim.h
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludo
 */

#ifndef __TIM_H__
#define __TIM_H__

#include "stdint.h"

/*** TIM functions ***/

// ADC trigger.
void TIM1_init(uint32_t period_ms);

// Milliseconds count.
void TIM2_init(void);
uint32_t TIM2_get_milliseconds(void);
void TIM2_delay_milliseconds(uint32_t delay_ms);

// Tachro step timer
void TIM5_init(void);
void TIM5_start(void);
void TIM5_stop(void);
void TIM5_set_delay_microseconds(uint32_t delay_us);
uint8_t TIM5_get_uif_flag(void);
void TIM5_clear_uif_flag(void);

// KVB.
void TIM6_init(void);
void TIM6_start(void);
void TIM6_stop(void);

// Manometers.
void TIM7_init(uint32_t period_us);
void TIM7_start(void);
void TIM7_stop(void);

// LVAL PWM.
void TIM8_init(void);
void TIM8_set_duty_cycle(uint8_t duty_cycle_percent);
void TIM8_start(void);
void TIM8_stop(void);

#endif /* __TIM_H__ */
