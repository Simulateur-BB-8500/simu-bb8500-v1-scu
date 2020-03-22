/*
 * mano.c
 *
 *  Created on: 3 aug. 2019
 *      Author: Ludo
 */

#include "mano.h"

#include "mapping.h"
#include "stepper.h"
#include "tim.h"

/*** MANO local macros ***/

#define MANO_STEP_IT_PERIOD_MAX		1000	// 1000 * 100µs = 100ms.

/*** MANO functions ***/

/* MANOMETERS COMMON INIT.
 * @param:	None.
 * @return:	None.
 */
void MANOS_Init(void) {

	/* Init GPIOs */
	GPIO_Configure(&GPIO_ZMANOS, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);

	/* Start timer */
	TIM7_Start();
}

/* POWER MANOMETERS DRIVERS.
 * @param:	None.
 * @return:	None.
 */
void MANOS_Enable(void) {
	GPIO_Write(&GPIO_ZMANOS, 1);
}

/* POWER MANOMETERS DRIVERS.
 * @param:	None.
 * @return:	None.
 */
void MANOS_Disable(void) {
	GPIO_Write(&GPIO_ZMANOS, 0);
}

/* INIT MANOMETER.
 * @param mano:						Manometer to configure.
 * @param stepper:					Step motor attached to the needle.
 * @param stepper_cmd1:				Step motor GPIO1.
 * @param stepper_cmd2:				Step motor GPIO2.
 * @param pressure_max_decibars:	Maximum pressure displayed on the manometers (in decibars).
 * @param pressure_max_steps:		Number of motor steps required to reach the maximum pressure.
 * @param needle_inertia_steps:		Number of steps used to perform needle inertia.
 * @param needle_speed_max:			Minimum step IT period (expressed in hundreds of µs).
 * @return:							None.
 */
void MANO_Init(MANO_Context* mano, STEPPER_Context* stepper, const GPIO* stepper_cmd1, const GPIO* stepper_cmd2, unsigned int pressure_max_decibars, unsigned int pressure_max_steps, unsigned int needle_inertia_steps, unsigned int needle_speed_max) {

	/* Init GPIOs */
	STEPPER_Init(stepper, stepper_cmd1, stepper_cmd2);

	/* Init context */
	mano -> mano_pressure_max_decibars = pressure_max_decibars;
	mano -> mano_pressure_max_steps = pressure_max_steps;
	mano -> mano_needle_inertia_steps = needle_inertia_steps;
	mano -> mano_enable = 0;
	mano -> mano_stepper = stepper;
	mano -> mano_start_step = 0;
	mano -> mano_target_step = 0;
	mano -> mano_step_it_count = 0;
	mano -> mano_step_it_period = 0;
	mano -> mano_step_it_period_min = needle_speed_max;
}

/* UPDATE PRESSURE TARGET.
 * @param mano:					Manometer to control.
 * @param pressure_decibars:	New pressure expressed in decibars.
 */
void MANO_SetTarget(MANO_Context* mano, unsigned int pressure_decibars) {
	// Update target step.
	mano -> mano_target_step = ((mano -> mano_pressure_max_steps) * pressure_decibars) / (mano -> mano_pressure_max_decibars);
}

/* START NEEDLE MOVEMENT.
 * @param mano:	Manometer to control.
 * @return:		None.
 */
void MANO_StartNeedle(MANO_Context* mano) {
	// Enable movement.
	mano -> mano_enable = 1;
	// Store current position as start step.
	mano -> mano_start_step = (mano -> mano_stepper -> stepper_current_step);
}

/* STOP NEEDLE MOVEMENT.
 * @param mano:	Manometer to control.
 * @return:		None.
 */
void MANO_StopNeedle(MANO_Context* mano) {

	/* Disable movement */
	mano -> mano_enable = 0;

	/* Update target to perform inertia */
	// Up direction.
	if (((mano -> mano_stepper) -> stepper_current_step) < (mano -> mano_target_step)) {
		// Current step < (max - inertia)
		if (((mano -> mano_stepper) -> stepper_current_step) < ((mano -> mano_pressure_max_steps) - (mano -> mano_needle_inertia_steps))) {
			// New target = current + inertia.
			mano -> mano_target_step = ((mano -> mano_stepper) -> stepper_current_step) + (mano -> mano_needle_inertia_steps);
		}
	}
	// Down direction.
	if (((mano -> mano_stepper) -> stepper_current_step) > (mano -> mano_target_step)) {
		// Current step > (0 + inertia).
		if (((mano -> mano_stepper) -> stepper_current_step) > (mano -> mano_needle_inertia_steps)) {
			// New target = current - inertia.
			mano -> mano_target_step = ((mano -> mano_stepper) -> stepper_current_step) - (mano -> mano_needle_inertia_steps);
		}
	}
}

/* MANOMETER MAIN TASK (CALLED BY TIM7 INTERRUPT HANDLER EVERY 100µs).
 * @param:	None.
 * @return:	None.
 */
void MANO_NeedleTask(MANO_Context* mano) {

	/* Movement feedback loop */
	mano -> mano_step_it_count++;
	unsigned int current_step = ((mano -> mano_stepper) -> stepper_current_step);
	// Checl if the period was reached.
	if ((mano -> mano_step_it_count) >= (mano -> mano_step_it_period)) {
		// Reset interrupt count.
		mano -> mano_step_it_count = 0;
		// Up direction.
		if (current_step < (mano -> mano_target_step)) {
			STEPPER_Up(mano -> mano_stepper);
		}
		// Down direction.
		if (current_step > (mano -> mano_target_step)) {
			STEPPER_Down(mano -> mano_stepper);
		}
	}

	/* Compute next period */
	if (current_step != (mano -> mano_target_step)) {
		// Get absolute distances between start, target and current steps.
		unsigned int delta_start = 0;
		unsigned int delta_target = 0;
		// Up direction.
		if (current_step < (mano -> mano_target_step)) {
			delta_start = current_step - (mano -> mano_start_step);
			delta_target = (mano -> mano_target_step) - current_step;
		}
		// Down direction.
		if (current_step > (mano -> mano_target_step)) {
			delta_start = (mano -> mano_start_step) - current_step;
			delta_target = current_step - (mano -> mano_target_step);
		}
		// Perform linear equation.
		if (delta_start < (mano -> mano_needle_inertia_steps)) {
			mano -> mano_step_it_period = MANO_STEP_IT_PERIOD_MAX - ((MANO_STEP_IT_PERIOD_MAX - (mano -> mano_step_it_period_min)) * (delta_start)) / (mano -> mano_needle_inertia_steps);
		}
		else {
			if (delta_target < (mano -> mano_needle_inertia_steps)) {
				mano -> mano_step_it_period = MANO_STEP_IT_PERIOD_MAX - ((MANO_STEP_IT_PERIOD_MAX - (mano -> mano_step_it_period_min)) * (delta_target)) / (mano -> mano_needle_inertia_steps);
			}
			else {
				// Maximum speed.
				mano -> mano_step_it_period = (mano -> mano_step_it_period_min);
			}
		}
	}
}
