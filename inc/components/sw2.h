/*
 * sw2.h
 *
 *  Created on: 01 oct. 2017
 *      Author: Ludo
 */

#ifndef __SW2_H__
#define __SW2_H__

#include "gpio.h"
#include "stdint.h"

/*** SW2 structures ***/

/*!******************************************************************
 * \enum SW2_state_t
 * \brief 2-positions switch state.
 *******************************************************************/
typedef enum {
    SW2_OFF = 0,
    SW2_ON
} SW2_state_t;

/*!******************************************************************
 * \enum SW2_internal_state_t
 * \brief 2-positions switch internal state.
 *******************************************************************/
typedef enum {
    SW2_STATE_OFF = 10,
    SW2_STATE_CONFIRM_ON,
    SW2_STATE_ON,
    SW2_STATE_CONFIRM_OFF
} SW2_internal_state_t;

/*!******************************************************************
 * \struct SW2_context_t
 * \brief 2-positions switch context structure.
 *******************************************************************/
typedef struct {
    const GPIO_pin_t* gpio;
    uint8_t active_state; // Depends on switch wiring.
    SW2_internal_state_t internal_state; // Current state in SW2 state machine.
    SW2_state_t state; // State after anti-bouncing (used in higher levels).
    uint32_t debouncing_ms; // Delay before validating ON/OFF state (in ms).
    uint32_t confirm_start_time;
} SW2_context_t;

/*** SW2 functions ***/

/*!******************************************************************
 * \fn void SW2_init(SW2_context_t* sw2, const GPIO_pin_t* gpio, uint8_t active_state, uint32_t debouncing_ms)
 * \brief Init 2-positions switch.
 * \param[in]   sw2: Pointer to the switch context to initialize.
 * \param[in]   gpio: GPIO attached to the switch.
 * \param[in]   active_state: Logic state for which the switch is considered as active.
 * \param[in]   debouncing_ms: Switch debouncing time in milliseconds.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void SW2_init(SW2_context_t* sw2, const GPIO_pin_t* gpio, uint8_t active_state, uint32_t debouncing_ms);

/*!******************************************************************
 * \fn void SW2_update_state(SW2_context_t* sw2)
 * \brief Update 2-positions switch state.
 * \param[in]   sw2: Pointer to the switch context to update.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void SW2_update_state(SW2_context_t* sw2);

#endif /* __SW2_H__ */
