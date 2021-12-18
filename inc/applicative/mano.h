/*
 * mano.h
 *
 *  Created on: 3 aug. 2019
 *      Author: Ludo
 */

#ifndef MANO_H
#define MANO_H

#include "stepper.h"

/*** MANO structures ***/

typedef struct {
	STEPPER_Context* mano_stepper;
	unsigned int mano_pressure_max_decibars;
	unsigned int mano_pressure_max_steps;
	unsigned int mano_needle_inertia_steps;
	unsigned int mano_step_it_period_min; // Determines the maximum speed of the stepper.
	unsigned int mano_start_step;
	volatile unsigned char mano_enable;
	volatile unsigned int mano_target_step;
	volatile unsigned int mano_step_it_count; // Expressed in hundreds of us (number of timer interrupt calls).
	volatile unsigned int mano_step_it_period; // Expressed in hundreds of us.
} MANO_Context;

/*** MANO functions ***/

void MANOS_Init(void);
void MANOS_ManagePower(void);
void MANO_SetTarget(MANO_Context* mano, unsigned int pressure_decibars);
unsigned int MANO_GetPressure(MANO_Context* mano);
void MANO_StartNeedle(MANO_Context* mano);
void MANO_StopNeedle(MANO_Context* mano);
void MANO_NeedleTask(MANO_Context* mano);

#endif /* APPLICATIVE_MANOS_H_ */
