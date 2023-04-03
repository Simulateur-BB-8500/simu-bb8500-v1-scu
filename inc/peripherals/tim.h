/*
 * tim.h
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludo
 */

#ifndef __TIM_H__
#define __TIM_H__

#include "types.h"

/*** TIM functions ***/

// Milliseconds count.
void TIM2_init(void);
uint32_t TIM2_get_milliseconds(void);
void TIM2_delay_milliseconds(uint32_t ms_to_wait);

// Tachro step timer
void TIM5_init(void);
void TIM5_start(void);
void TIM5_stop(void);
void TIM5_set_delay_microseconds(uint32_t arr_value);
uint8_t TIM5_get_uif_flag(void);
void TIM5_clear_uif_flag(void);

// KVB.
void TIM6_init(void);
void TIM6_start(void);
void TIM6_stop(void);

// Manometers.
void TIM7_init(void);
void TIM7_start(void);
void TIM7_stop(void);

// LVAL PWM.
void TIM8_init(void);
void TIM8_set_duty_cycle(uint8_t duty_cycle);
void TIM8_start(void);
void TIM8_stop(void);

#endif /* __TIM_H__ */
