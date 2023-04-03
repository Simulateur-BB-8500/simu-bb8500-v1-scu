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
#include "types.h"

/*** MANOMETERS structures ***/

typedef struct {
	volatile uint8_t enable;
	STEP_MOTOR_context_t* step_motor;
	uint32_t pressure_limit_decibars; // Maximum operation pressure.
	uint32_t pressure_max_decibars; // Maximum pressure displayed on the dial.
	uint32_t pressure_max_steps; // Number of steps required to display the maximum pressure of the dial.
	uint32_t needle_inertia_steps;
	uint32_t step_start;
	volatile uint32_t step_target;
	uint32_t step_it_period_min; // Determines the maximum speed of the step_motor.
	volatile uint32_t step_it_count; // Expressed in hundreds of us (number of timer interrupt calls).
	volatile uint32_t step_it_period; // Expressed in hundreds of us.
} MANOMETER_context_t;

/*** MANOMETERS functions ***/

void MANOMETER_init_all(void);
void MANOMETER_manage_power_all(void);
void MANOMETER_set_pressure(MANOMETER_context_t* manometer, uint32_t pressure_decibars);
uint32_t MANOMETER_get_pressure(MANOMETER_context_t* manometer);
void MANOMETER_needle_start(MANOMETER_context_t* manometer);
void MANOMETER_needle_stop(MANOMETER_context_t* manometer);
void MANOMETER_needle_task(MANOMETER_context_t* manometer);
#ifdef DEBUG
void MANOMETER_print_data(void);
#endif

#endif /* __MANOMETERS_H__ */
