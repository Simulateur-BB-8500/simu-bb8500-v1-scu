/*
 * sw4.h
 *
 *  Created on: 26 dec. 2017
 *      Author: Ludo
 */

#ifndef __SW4_H__
#define __SW4_H__

#include "gpio.h"
#include "stdint.h"

/*** SW4 structures ***/

/*!******************************************************************
 * \enum SW4_state_t
 * \brief 4-positions switch state.
 *******************************************************************/
typedef enum {
	SW4_P0 = 0,
	SW4_P1,
	SW4_P2,
	SW4_P3
} SW4_state_t;

/*!******************************************************************
 * \enum SW4_internal_state_t
 * \brief 4-positions switch internal state.
 *******************************************************************/
typedef enum {
	SW4_STATE_P0 = 10,
	SW4_STATE_CONFIRM_P0,
	SW4_STATE_P1,
	SW4_STATE_CONFIRM_P1,
	SW4_STATE_P2,
	SW4_STATE_CONFIRM_P2,
	SW4_STATE_P3,
	SW4_STATE_CONFIRM_P3
} SW4_internal_state_t;

/*!******************************************************************
 * \enum SW4_context_t
 * \brief 4-positions context structure.
 *******************************************************************/
typedef struct {
	uint32_t* adc_data_ptr; // Pointer to the 14-bits ADC data.
	uint32_t voltage_mv; // Voltage in mV.
	SW4_internal_state_t internal_state; // Current state in SW4 state machine.
	SW4_state_t state; // State after anti-bouncing (used in higher levels).
	uint32_t debouncing_ms; // Delay before validating states (in ms).
	uint32_t confirm_start_time;
} SW4_context_t;

/*** SW4 functions ***/

/*!******************************************************************
 * \fn void SW4_init(SW4_context_t* sw4, const GPIO_pin_t* gpio, uint8_t active_state, uint32_t debouncing_ms)
 * \brief Init 4-positions switch.
 * \param[in]  	sw4: Pointer to the switch context to initialize.
 * \param[in]	gpio: GPIO attached to the switch.
 * \param[in]	active_state: Logic state for which the switch is considered as active.
 * \param[in]	debouncing_ms: Switch debouncing time in milliseconds.
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void SW4_init(SW4_context_t* sw4, const GPIO_pin_t* gpio, uint32_t debouncing_ms, uint32_t* adc_data_ptr);

/*!******************************************************************
 * \fn void SW4_update_state(SW4_context_t* sw4)
 * \brief Update 4-positions switch state.
 * \param[in]  	sw4: Pointer to the switch context to update.
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void SW4_update_state(SW4_context_t* sw4);

#endif /* __SW4_H__ */
