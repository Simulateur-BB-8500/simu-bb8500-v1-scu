/*
 * manometer.h
 *
 *  Created on: 3 aug. 2019
 *      Author: Ludo
 */

#ifndef MANOMETERS_H
#define MANOMETERS_H

#include "step_motor.h"

/*** MANOMETERS structures ***/

typedef struct {
	volatile unsigned char enable;
	STEP_MOTOR_Context* step_motor;
	unsigned int pressure_limit_decibars; // Maximum operation pressure.
	unsigned int pressure_max_decibars; // Maximum pressure displayed on the dial.
	unsigned int pressure_max_steps; // Number of steps required to display the maximum pressure of the dial.
	unsigned int needle_inertia_steps;
	unsigned int step_start;
	volatile unsigned int step_target;
	unsigned int step_it_period_min; // Determines the maximum speed of the step_motor.
	volatile unsigned int step_it_count; // Expressed in hundreds of us (number of timer interrupt calls).
	volatile unsigned int step_it_period; // Expressed in hundreds of us.
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
