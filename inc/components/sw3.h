/*
 * sw3.h
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludo
 */

#ifndef __SW3_H__
#define __SW3_H__

#include "gpio.h"
#include "stdint.h"

/*** 3-poles switch structures ***/

/*!******************************************************************
 * \enum SW3_state_t
 * \brief 3-positions switch state.
 *******************************************************************/
typedef enum {
	SW3_BACK = 0,
	SW3_NEUTRAL,
	SW3_FRONT
} SW3_state_t;

/*!******************************************************************
 * \enum SW3_internal_state_t
 * \brief 3-positions switch internal state.
 *******************************************************************/
typedef enum {
	SW3_STATE_NEUTRAL = 0,
	SW3_STATE_CONFIRM_NEUTRAL,
	SW3_STATE_BACK,
	SW3_STATE_CONFIRM_BACK,
	SW3_STATE_FRONT,
	SW3_STATE_CONFIRM_FRONT
} SW3_internal_state_t;

/*!******************************************************************
 * \struct SW3_context_t
 * \brief 3-positions switch context structure.
 *******************************************************************/
typedef struct {
	uint32_t* adc_data_ptr; // Pointer to the 13-bits ADC data.
	uint32_t voltage_mv; // Voltage in mV.
	SW3_internal_state_t internal_state; // Current state in SW3 internal state machine.
	SW3_state_t state; // State after anti-bouncing (used in higher levels).
	uint32_t debouncing_ms; // Delay before validating states (in ms).
	uint32_t confirm_start_time;
} SW3_context_t;

/*** 3-poles switch functions ***/

/*!******************************************************************
 * \fn void SW3_init(SW3_context_t* sw3, const GPIO_pin_t* gpio, uint8_t active_state, uint32_t debouncing_ms)
 * \brief Init 3-positions switch.
 * \param[in]  	sw3: Pointer to the switch context to initialize.
 * \param[in]	gpio: GPIO attached to the switch.
 * \param[in]	debouncing_ms: Switch debouncing time in milliseconds.
 * \param[in]	adc_data_ptr: Pointer to the corresponding analog data from ADC.
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void SW3_init(SW3_context_t* sw3, const GPIO_pin_t* gpio, uint32_t debouncing_ms, uint32_t* adc_data_ptr);

/*!******************************************************************
 * \fn void SW3_update_state(SW3_context_t* sw3)
 * \brief Update 3-positions switch state.
 * \param[in]  	sw3: Pointer to the switch context to update.
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void SW3_update_state(SW3_context_t* sw3);

#endif /* __SW3_H__ */
