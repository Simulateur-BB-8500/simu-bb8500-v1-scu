/*
 * manometer.h
 *
 *  Created on: 3 aug. 2019
 *      Author: Ludo
 */

#ifndef __MANOMETERS_H__
#define __MANOMETERS_H__

#include "mode.h"
#include "step_motor.h"
#include "stdint.h"

/*** MANOMETERS structures ***/

typedef struct {
	STEP_MOTOR_context_t* step_motor;
	uint32_t pressure_limit_mbar; // Maximum operation pressure in millibar.
	uint32_t pressure_max_mbar; // Maximum pressure displayed on the dial in millibar.
	uint32_t pressure_max_steps; // Number of relative steps required to display the maximum pressure.
	volatile uint32_t is_moving;
	volatile uint32_t needle_inertia_steps;
	volatile uint32_t needle_inertia_factor;
	volatile uint32_t step_start;
	volatile uint32_t step_target;
	volatile uint8_t step_target_zero_flag;
	volatile uint32_t step_irq_count;
	volatile uint32_t step_irq_period_min; // Maximum motor speed for a given movement.
	volatile uint32_t step_irq_period;
} MANOMETER_context_t;

/*** MANOMETERS functions ***/

void MANOMETER_init_all(void);
void MANOMETER_manage_power_all(void);
void MANOMETER_set_pressure(MANOMETER_context_t* manometer, uint32_t pressure_mbar, uint32_t speed_mbar_per_second);
uint32_t MANOMETER_get_pressure(MANOMETER_context_t* manometer);
uint8_t MANOMETER_is_pressure_increasing(MANOMETER_context_t* manometer);
void MANOMETER_needle_stop(MANOMETER_context_t* manometer);

#endif /* __MANOMETERS_H__ */
