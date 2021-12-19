/*
 * manometer.h
 *
 *  Created on: 3 aug. 2019
 *      Author: Ludo
 */

#ifndef MANOMETERS_H
#define MANOMETERS_H

#include "stepper.h"

/*** MANOMETERS structures ***/

typedef struct {
	STEPPER_Context* manometer_stepper;
	unsigned int manometer_pressure_max_decibars;
	unsigned int manometer_pressure_max_steps;
	unsigned int manometer_needle_inertia_steps;
	unsigned int manometer_step_it_period_min; // Determines the maximum speed of the stepper.
	unsigned int manometer_start_step;
	volatile unsigned char manometer_enable;
	volatile unsigned int manometer_target_step;
	volatile unsigned int manometer_step_it_count; // Expressed in hundreds of us (number of timer interrupt calls).
	volatile unsigned int manometer_step_it_period; // Expressed in hundreds of us.
} MANOMETER_Context;

/*** MANOMETERS functions ***/

void MANOMETER_InitAll(void);
void MANOMETER_ManagePowerAll(void);
void MANOMETER_SetPressure(MANOMETER_Context* manometer, unsigned int pressure_decibars);
unsigned int MANOMETER_GetPressure(MANOMETER_Context* manometer);
void MANOMETER_NeedleStart(MANOMETER_Context* manometer);
void MANOMETER_NeedleStop(MANOMETER_Context* manometer);
void MANOMETER_NeedleTask(MANOMETER_Context* manometer);

#endif /* MANOMETERS_H */
