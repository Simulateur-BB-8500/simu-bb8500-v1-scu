/*
 * rcc.h
 *
 *  Created on: 09 sep. 2017
 *      Author: Ludo
 */

#ifndef __RCC_H__
#define __RCC_H__

#include "stdint.h"

/*** RCC macros ***/

//#define RCC_OUTPUT_CLOCK

/*** RCC structures ***/

/*!******************************************************************
 * \enum RCC_clock_t
 * \brief RCC clocks list.
 *******************************************************************/
typedef enum {
    RCC_CLOCK_SYSTEM = 0,
    RCC_CLOCK_APB1,
    RCC_CLOCK_APB2,
    RCC_CLOCK_LAST
} RCC_clock_t;

/*** RCC functions ***/

/*!******************************************************************
 * \fn void RCC_init(void)
 * \brief Init MCU default clock tree.
 * \param[in]   none
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
void RCC_init(void);

/*!******************************************************************
 * \fn uint32_t RCC_get_frequency_hz(RCC_clock_t clock)
 * \brief Get clock frequency.
 * \param[in]   clock: Clock to read.
 * \param[out]  none
 * \retval      Clock frequency in Hz.
 *******************************************************************/
uint32_t RCC_get_frequency_hz(RCC_clock_t clock);

#endif /* __RCC_H__ */
