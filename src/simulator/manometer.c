/*
 * manometer.c
 *
 *  Created on: 3 aug. 2019
 *      Author: Ludo
 */

#include "manometer.h"

#include "lsmcu.h"
#include "mapping.h"
#include "stepper.h"
#include "tim.h"

/*** MANOMETER local macros ***/

#define MANOMETER_STEP_IT_PERIOD_MAX		1000	// 1000 * 100µs = 100ms.

/*** MANOMETER external global variables ***/

extern STEPPER_Context stepper_cp;
extern STEPPER_Context stepper_re;
extern STEPPER_Context stepper_cg;
extern STEPPER_Context stepper_cf1;
extern STEPPER_Context stepper_cf2;
extern LSMCU_Context lsmcu_ctx;

/*** MANOMETER local global variables ***/

MANOMETER_Context manometer_cp = {&stepper_cp, 100, 3072, 20, 100, 0, 0, 0, 0, 0};
MANOMETER_Context manometer_re = {&stepper_re, 100, 3072, 20, 100, 0, 0, 0, 0, 0};
MANOMETER_Context manometer_cg = {&stepper_cg, 100, 3072, 20, 100, 0, 0, 0, 0, 0};
MANOMETER_Context manometer_cf1 = {&stepper_cf1, 60, 3072, 20, 100, 0, 0, 0, 0, 0};
MANOMETER_Context manometer_cf2 = {&stepper_cf2, 60, 3072, 20, 100, 0, 0, 0, 0, 0};

/*** MANOMETER local functions ***/

/* POWER MANOMETER DRIVERS.
 * @param:	None.
 * @return:	None.
 */
static void MANOMETER_PowerOnAll(void) {
	// Turn step motors on.
	GPIO_Write(&GPIO_MANOMETER_POWER_ENABLE, 1);
	GPIO_Write(&GPIO_LED_RED, 1);
	// Start timer.
	TIM7_Start();
}

/* POWER MANOMETER DRIVERS.
 * @param:	None.
 * @return:	None.
 */
static void MANOMETER_PowerOffAll(void) {
	// Turn step motors on.
	GPIO_Write(&GPIO_MANOMETER_POWER_ENABLE, 0);
	GPIO_Write(&GPIO_LED_RED, 0);
	// Stop timer.
	TIM7_Stop();
}

/* CHECK IF A GIVEN NEEDLE IS MOVING.
 * @param:			None.
 * @eturn moving:	'1' if the manometer needle is currently moving (target not reached), '0' otherwise.
 */
static unsigned char MANOMETER_NeedleIsMoving(MANOMETER_Context* manometer) {
	unsigned char moving = 0;
	if (((manometer -> manometer_stepper) -> stepper_current_step) != (manometer -> manometer_target_step)) {
		moving = 1;
	}
	return moving;
}

/*** MANOMETER functions ***/

/* MANOMETER COMMON INIT.
 * @param:	None.
 * @return:	None.
 */
void MANOMETER_InitAll(void) {
	// Init GPIOs.
	GPIO_Configure(&GPIO_MANOMETER_POWER_ENABLE, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init step motors.
	STEPPER_Init(manometer_cp.manometer_stepper);
	STEPPER_Init(manometer_cg.manometer_stepper);
	STEPPER_Init(manometer_re.manometer_stepper);
	STEPPER_Init(manometer_cf1.manometer_stepper);
	STEPPER_Init(manometer_cf2.manometer_stepper);
	// Link to global context.
	lsmcu_ctx.lsmcu_manometer_cp = &manometer_cp;
	lsmcu_ctx.lsmcu_manometer_re = &manometer_re;
	lsmcu_ctx.lsmcu_manometer_cg = &manometer_cg;
	lsmcu_ctx.lsmcu_manometer_cf1 = &manometer_cf1;
	lsmcu_ctx.lsmcu_manometer_cf2 = &manometer_cf1;
}

/* CONTROL MANOMETER POWER.
 * @param:	None.
 * @return:	None.
 */
void MANOMETER_ManagePowerAll(void) {
	// Check all manometer.
	if ((MANOMETER_NeedleIsMoving(&manometer_cp) == 0) &&
		(MANOMETER_NeedleIsMoving(&manometer_re) == 0) &&
		(MANOMETER_NeedleIsMoving(&manometer_cg) == 0) &&
		(MANOMETER_NeedleIsMoving(&manometer_cf1) == 0) &&
		(MANOMETER_NeedleIsMoving(&manometer_cf2) == 0)) {
		// Turn manometer off.
		MANOMETER_PowerOffAll();
	}
	else {
		// Turn manometer on.
		MANOMETER_PowerOnAll();
	}
}

/* UPDATE PRESSURE TARGET.
 * @param manometer:			Manometer to control.
 * @param pressure_decibars:	New pressure expressed in decibars.
 */
void MANOMETER_SetPressure(MANOMETER_Context* manometer, unsigned int pressure_decibars) {
	// Update target step.
	manometer -> manometer_target_step = ((manometer -> manometer_pressure_max_steps) * pressure_decibars) / (manometer -> manometer_pressure_max_decibars);
}

/* GET CURRENT MANOMETER PRESSURE.
 * @param manometer:			Manometer to analyse.
 * @return pressure_decibars:	Current pressure displayed by the manometer in decibars.
 */
unsigned int MANOMETER_GetPressure(MANOMETER_Context* manometer) {
	unsigned int pressure_decibars = (((manometer -> manometer_stepper) -> stepper_current_step) * (manometer -> manometer_pressure_max_decibars)) / (manometer -> manometer_pressure_max_steps);
	return pressure_decibars;
}

/* START NEEDLE MOVEMENT.
 * @param manometer:	Manometer to control.
 * @return:		None.
 */
void MANOMETER_NeedleStart(MANOMETER_Context* manometer) {
	// Enable movement.
	manometer -> manometer_enable = 1;
	// Store current position as start step.
	manometer -> manometer_start_step = (manometer -> manometer_stepper -> stepper_current_step);
}

/* STOP NEEDLE MOVEMENT.
 * @param manometer:	Manometer to control.
 * @return:		None.
 */
void MANOMETER_NeedleStop(MANOMETER_Context* manometer) {
	// Disable movement.
	manometer -> manometer_enable = 0;
	// Update target to perform inertia.
	// Up direction.
	if (((manometer -> manometer_stepper) -> stepper_current_step) < (manometer -> manometer_target_step)) {
		// Current step < (max - inertia)
		if (((manometer -> manometer_stepper) -> stepper_current_step) < ((manometer -> manometer_pressure_max_steps) - (manometer -> manometer_needle_inertia_steps))) {
			// New target = current + inertia.
			manometer -> manometer_target_step = ((manometer -> manometer_stepper) -> stepper_current_step) + (manometer -> manometer_needle_inertia_steps);
		}
	}
	// Down direction.
	if (((manometer -> manometer_stepper) -> stepper_current_step) > (manometer -> manometer_target_step)) {
		// Current step > (0 + inertia).
		if (((manometer -> manometer_stepper) -> stepper_current_step) > (manometer -> manometer_needle_inertia_steps)) {
			// New target = current - inertia.
			manometer -> manometer_target_step = ((manometer -> manometer_stepper) -> stepper_current_step) - (manometer -> manometer_needle_inertia_steps);
		}
	}
}

/* MANOMETER MAIN TASK (CALLED BY TIM7 INTERRUPT HANDLER EVERY 100us).
 * @param:	None.
 * @return:	None.
 */
void MANOMETER_NeedleTask(MANOMETER_Context* manometer) {
	// Movement feedback loop.
	manometer -> manometer_step_it_count++;
	unsigned int current_step = ((manometer -> manometer_stepper) -> stepper_current_step);
	// ChecK if the period was reached.
	if ((manometer -> manometer_step_it_count) >= (manometer -> manometer_step_it_period)) {
		// Reset interrupt count.
		manometer -> manometer_step_it_count = 0;
		// Up direction.
		if (current_step < (manometer -> manometer_target_step)) {
			STEPPER_Up(manometer -> manometer_stepper);
		}
		// Down direction.
		if (current_step > (manometer -> manometer_target_step)) {
			STEPPER_Down(manometer -> manometer_stepper);
		}
	}
	// Compute next period.
	if (current_step != (manometer -> manometer_target_step)) {
		// Get absolute distances between start, target and current steps.
		unsigned int delta_start = 0;
		unsigned int delta_target = 0;
		// Up direction.
		if (current_step < (manometer -> manometer_target_step)) {
			delta_start = current_step - (manometer -> manometer_start_step);
			delta_target = (manometer -> manometer_target_step) - current_step;
		}
		// Down direction.
		if (current_step > (manometer -> manometer_target_step)) {
			delta_start = (manometer -> manometer_start_step) - current_step;
			delta_target = current_step - (manometer -> manometer_target_step);
		}
		// Perform linear equation.
		if (delta_start < (manometer -> manometer_needle_inertia_steps)) {
			manometer -> manometer_step_it_period = MANOMETER_STEP_IT_PERIOD_MAX - ((MANOMETER_STEP_IT_PERIOD_MAX - (manometer -> manometer_step_it_period_min)) * (delta_start)) / (manometer -> manometer_needle_inertia_steps);
		}
		else {
			if (delta_target < (manometer -> manometer_needle_inertia_steps)) {
				manometer -> manometer_step_it_period = MANOMETER_STEP_IT_PERIOD_MAX - ((MANOMETER_STEP_IT_PERIOD_MAX - (manometer -> manometer_step_it_period_min)) * (delta_target)) / (manometer -> manometer_needle_inertia_steps);
			}
			else {
				// Maximum speed.
				manometer -> manometer_step_it_period = (manometer -> manometer_step_it_period_min);
			}
		}
	}
}
