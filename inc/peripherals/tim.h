/*
 * tim.h
 *
 *  Created on: 16 sep. 2017
 *      Author: Ludo
 */

#ifndef __TIM_H__
#define __TIM_H__

#include "stdint.h"

/*** TIM structures ***/

/*!******************************************************************
 * \fn TIM_completion_irq_cb_t
 * \brief TIM completion callback.
 *******************************************************************/
typedef void (*TIM_completion_irq_cb_t)(void);

/*** TIM functions ***/

/*!******************************************************************
 * \fn void TIM1_init(uint32_t period_ms)
 * \brief Init TIM1 for ADC trigger.
 * \param[in]   period_ms: Period in milliseconds.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM1_init(uint32_t period_ms);

/*!******************************************************************
 * \fn void TIM2_init(void)
 * \brief Init TIM2 for global MCU base time.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM2_init(void);

/*!******************************************************************
 * \fn uint32_t TIM2_get_milliseconds(void)
 * \brief Get MCU time.
 * \param[in]   none
 * \param[out]  none
 * \retval      Current time in milliseconds.
 *******************************************************************/
uint32_t TIM2_get_milliseconds(void);

/*!******************************************************************
 * \fn void TIM2_delay_milliseconds(uint32_t delay_ms)
 * \brief Perform a delay.
 * \param[in]   delay_ms: Delay to wait in milliseconds.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM2_delay_milliseconds(uint32_t delay_ms);

/*!******************************************************************
 * \fn void TIM5_init(void)
 * \brief Init TIM5 for Tachro phase driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM5_init(void);

/*!******************************************************************
 * \fn void TIM5_start(void)
 * \brief Start TIM5.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM5_start(void);

/*!******************************************************************
 * \fn void TIM5_stop(void)
 * \brief Stop TIM5.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM5_stop(void);

/*!******************************************************************
 * \fn void TIM5_set_delay_microseconds(uint32_t delay_us)
 * \brief Set TIM5 delay.
 * \param[in]   delay_us: Timer period in microseconds.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM5_set_delay_microseconds(uint32_t delay_us);

/*!******************************************************************
 * \fn uint8_t TIM5_get_uif_flag(void)
 * \brief Get TIM5 update interrupt flag.
 * \param[in]   none
 * \param[out]  none
 * \retval      Timer update flag.
 *******************************************************************/
uint8_t TIM5_get_uif_flag(void);

/*!******************************************************************
 * \fn void TIM5_clear_uif_flag(void)
 * \brief Clear TIM5 update interrupt flag.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM5_clear_uif_flag(void);

/*!******************************************************************
 * \fn void TIM6_init(void)
 * \brief Init TIM6 for KVB display sweep.
 * \param[in]   irq_callback: Function to call on interrupt.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM6_init(TIM_completion_irq_cb_t irq_callback);

/*!******************************************************************
 * \fn void TIM6_start(void)
 * \brief Start TIM6.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM6_start(void);

/*!******************************************************************
 * \fn void TIM6_stop(void)
 * \brief Stop TIM6.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM6_stop(void);

/*!******************************************************************
 * \fn void TIM7_init(void)
 * \brief Init TIM7 for manometers step motors driver.
 * \param[in]   period_us: Minimum period in microseconds.
 * \param[in]   irq_callback: Function to call on interrupt.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM7_init(uint32_t period_us, TIM_completion_irq_cb_t irq_callback);

/*!******************************************************************
 * \fn void TIM7_start(void)
 * \brief Start TIM7.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM7_start(void);

/*!******************************************************************
 * \fn void TIM7_stop(void)
 * \brief Stop TIM7.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM7_stop(void);

/*!******************************************************************
 * \fn void TIM8_init(void)
 * \brief Init TIM8 for KVB LVAL PWM control.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM8_init(void);

/*!******************************************************************
 * \fn void TIM8_set_duty_cycle(uint8_t duty_cycle_percent)
 * \brief Set LVAL blinking duty cycle.
 * \param[in]   duty_cycle_percent: Duty cycle in percent.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM8_set_duty_cycle(uint8_t duty_cycle_percent);

/*!******************************************************************
 * \fn void TIM8_start(void)
 * \brief Start TIM8.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM8_start(void);

/*!******************************************************************
 * \fn void TIM8_stop(void)
 * \brief Stop TIM8.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TIM8_stop(void);

#endif /* __TIM_H__ */
