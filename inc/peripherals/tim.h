/*
 * tim.h
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludo
 */

#ifndef __TIM_H__
#define __TIM_H__

/*** TIM functions ***/

// Milliseconds count.
void TIM2_init(void);
unsigned int TIM2_get_milliseconds(void);
void TIM2_delay_milliseconds(unsigned ms_to_wait);

// Tachro step timer
void TIM5_init(void);
void TIM5_start(void);
void TIM5_stop(void);
void TIM5_set_delay_microseconds(unsigned int arr_value);
unsigned char TIM5_get_uif_flag(void);
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
void TIM8_set_duty_cycle(unsigned char duty_cycle);
void TIM8_start(void);
void TIM8_stop(void);

#endif /* __TIM_H__ */
