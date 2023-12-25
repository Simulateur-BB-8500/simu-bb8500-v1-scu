/*
 * manometer.h
 *
 *  Created on: 3 aug. 2019
 *      Author: Ludo
 */

#ifndef __MANOMETERS_H__
#define __MANOMETERS_H__

#include "step_motor.h"
#include "stdint.h"

/*** MANOMETERS structures ***/

/*!******************************************************************
 * \enum MANOMETER_context_t
 * \brief Manometer context structure.
 *******************************************************************/
typedef struct {
	STEP_MOTOR_context_t* step_motor;
	uint32_t pressure_limit_mbar; // Maximum operation pressure in millibar.
	uint32_t pressure_max_mbar; // Maximum pressure displayed on the dial in millibar.
	uint32_t pressure_max_steps; // Number of relative steps required to display the maximum pressure.
	uint32_t needle_inertia_mbar; // Needle start and stop inertia effect in millibar.
	uint32_t needle_inertia_factor;
	uint32_t needle_inertia_steps; // Number of relative steps required for inertia.
	volatile uint32_t step_start;
	volatile uint32_t step_target;
	volatile uint32_t step_irq_count;
	volatile uint32_t step_irq_period_min; // Maximum motor speed for a given movement.
	volatile uint32_t step_irq_period;
	volatile uint8_t flag_step_target_zero;
	volatile uint8_t flag_is_moving;
} MANOMETER_context_t;

/*** MANOMETERS functions ***/

/*!******************************************************************
 * \fn void MANOMETER_init(void)
 * \brief Init manometers driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void MANOMETER_init(void);

/*!******************************************************************
 * \fn void MANOMETER_manage_power(void)
 * \brief Manage manometers power supply.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void MANOMETER_manage_power(void);

/*!******************************************************************
 * \fn void MANOMETER_set_pressure(MANOMETER_context_t* manometer, uint32_t pressure_mbar, uint32_t speed_mbar_per_second)
 * \brief Set manometer pressure.
 * \param[in]  	manometer: Pointer to the manometer to control.
 * \param[in]	pressure_mbar: Pressure to set in millibar.
 * \param[in]	speed_mbar_per_second: Manometer needle speed in millibar per second.
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void MANOMETER_set_pressure(MANOMETER_context_t* manometer, uint32_t pressure_mbar, uint32_t speed_mbar_per_second);

/*!******************************************************************
 * \fn uint32_t MANOMETER_get_pressure(MANOMETER_context_t* manometer)
 * \brief Read manometer pressure.
 * \param[in]  	manometer: Pointer to the manometer to read.
 * \param[out] 	none
 * \retval		Current manometer pressure in millibar.
 *******************************************************************/
uint32_t MANOMETER_get_pressure(MANOMETER_context_t* manometer);

/*!******************************************************************
 * \fn uint8_t MANOMETER_is_pressure_increasing(MANOMETER_context_t* manometer)
 * \brief Check is a manometer pressure is currently increasing.
 * \param[in]  	manometer: Pointer to the manometer to read.
 * \param[out] 	none
 * \retval		1 if the pressure is increasing, 0 otherwise.
 *******************************************************************/
uint8_t MANOMETER_is_pressure_increasing(MANOMETER_context_t* manometer);

/*!******************************************************************
 * \fn void MANOMETER_needle_stop(MANOMETER_context_t* manometer)
 * \brief Stop manometer needle.
 * \param[in]  	manometer: Pointer to the manometer to control.
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void MANOMETER_needle_stop(MANOMETER_context_t* manometer);

#endif /* __MANOMETERS_H__ */
