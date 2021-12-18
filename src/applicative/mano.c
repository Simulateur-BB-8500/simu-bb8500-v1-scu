/*
 * mano.c
 *
 *  Created on: 3 aug. 2019
 *      Author: Ludo
 */

#include "mano.h"

#include "lsmcu.h"
#include "mapping.h"
#include "stepper.h"
#include "tim.h"

/*** MANO local macros ***/

#define MANO_STEP_IT_PERIOD_MAX		1000	// 1000 * 100µs = 100ms.

/*** MANO external global variables ***/

extern LSMCU_Context lsmcu_ctx;
extern STEPPER_Context stepper_cp;
extern STEPPER_Context stepper_re;
extern STEPPER_Context stepper_cg;
extern STEPPER_Context stepper_cf1;
extern STEPPER_Context stepper_cf2;

/*** MANO local global variables ***/

MANO_Context mano_cp = {&stepper_cp, 100, 3072, 20, 100, 0, 0, 0, 0, 0};
MANO_Context mano_re = {&stepper_re, 100, 3072, 20, 100, 0, 0, 0, 0, 0};
MANO_Context mano_cg = {&stepper_cg, 100, 3072, 20, 100, 0, 0, 0, 0, 0};
MANO_Context mano_cf1 = {&stepper_cf1, 60, 3072, 20, 100, 0, 0, 0, 0, 0};
MANO_Context mano_cf2 = {&stepper_cf2, 60, 3072, 20, 100, 0, 0, 0, 0, 0};

/*** MANO local functions ***/

/* POWER MANOMETERS DRIVERS.
 * @param:	None.
 * @return:	None.
 */
static void MANOS_PowerOn(void) {
	// Turn step motors on.
	GPIO_Write(&GPIO_ZMANOS, 1);
	GPIO_Write(&GPIO_LED_RED, 1);
	// Start timer.
	TIM7_Start();
}

/* POWER MANOMETERS DRIVERS.
 * @param:	None.
 * @return:	None.
 */
static void MANOS_PowerOff(void) {
	// Turn step motors on.
	GPIO_Write(&GPIO_ZMANOS, 0);
	GPIO_Write(&GPIO_LED_RED, 0);
	// Stop timer.
	TIM7_Stop();
}

/* CHECK IF A GIVEN NEEDLE IS MOVING.
 * @param:			None.
 * @eturn moving:	'1' if the manometers needle is currently moving (target not reached), '0' otherwise.
 */
static unsigned char MANO_NeedleIsMoving(MANO_Context* mano) {
	unsigned char moving = 0;
	if (((mano -> mano_stepper) -> stepper_current_step) != (mano -> mano_target_step)) {
		moving = 1;
	}
	return moving;
}

/*** MANO functions ***/

/* MANOMETERS COMMON INIT.
 * @param:	None.
 * @return:	None.
 */
void MANOS_Init(void) {
	// Init GPIOs.
	GPIO_Configure(&GPIO_ZMANOS, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init step motors.
	STEPPER_Init(mano_cp.mano_stepper);
	STEPPER_Init(mano_cg.mano_stepper);
	STEPPER_Init(mano_re.mano_stepper);
	STEPPER_Init(mano_cf1.mano_stepper);
	STEPPER_Init(mano_cf2.mano_stepper);
	// Link to global context.
	lsmcu_ctx.lsmcu_mano_cp = &mano_cp;
	lsmcu_ctx.lsmcu_mano_re = &mano_re;
	lsmcu_ctx.lsmcu_mano_cg = &mano_cg;
	lsmcu_ctx.lsmcu_mano_cf1 = &mano_cf1;
	lsmcu_ctx.lsmcu_mano_cf2 = &mano_cf1;
}

/* CONTROL ZMANOS SIGNAL.
 * @param:	None.
 * @return:	None.
 */
void MANOS_ManagePower(void) {
	// Check all manometers.
	if ((MANO_NeedleIsMoving(&mano_cp) == 0) &&
		(MANO_NeedleIsMoving(&mano_re) == 0) &&
		(MANO_NeedleIsMoving(&mano_cg) == 0) &&
		(MANO_NeedleIsMoving(&mano_cf1) == 0) &&
		(MANO_NeedleIsMoving(&mano_cf2) == 0)) {
		// Turn manometers off.
		MANOS_PowerOff();
	}
	else {
		// Turn manometers on.
		MANOS_PowerOn();
	}
}

/* UPDATE PRESSURE TARGET.
 * @param mano:					Manometer to control.
 * @param pressure_decibars:	New pressure expressed in decibars.
 */
void MANO_SetTarget(MANO_Context* mano, unsigned int pressure_decibars) {
	// Update target step.
	mano -> mano_target_step = ((mano -> mano_pressure_max_steps) * pressure_decibars) / (mano -> mano_pressure_max_decibars);
}

/* GET CURRENT MANOMETER PRESSURE.
 * @param mano:					Manometer to analyse.
 * @return pressure_decibars:	Current pressure displayed by the manometer in decibars.
 */
unsigned int MANO_GetPressure(MANO_Context* mano) {
	unsigned int pressure_decibars = (((mano -> mano_stepper) -> stepper_current_step) * (mano -> mano_pressure_max_decibars)) / (mano -> mano_pressure_max_steps);
	return pressure_decibars;
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
	// Disable movement.
	mano -> mano_enable = 0;
	// Update target to perform inertia.
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
	// Movement feedback loop.
	mano -> mano_step_it_count++;
	unsigned int current_step = ((mano -> mano_stepper) -> stepper_current_step);
	// ChecK if the period was reached.
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
	// Compute next period.
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
