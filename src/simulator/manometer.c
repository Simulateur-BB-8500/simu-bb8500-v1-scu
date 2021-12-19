/*
 * manometer.c
 *
 *  Created on: 3 aug. 2019
 *      Author: Ludo
 */

#include "manometer.h"

#include "lsmcu.h"
#include "mapping.h"
#include "step_motor.h"
#include "tim.h"

/*** MANOMETER local macros ***/

#define MANOMETER_STEP_IT_PERIOD_MAX		1000	// 1000 * 100µs = 100ms.

/*** MANOMETER external global variables ***/

extern STEP_MOTOR_Context step_motor_cp;
extern STEP_MOTOR_Context step_motor_re;
extern STEP_MOTOR_Context step_motor_cg;
extern STEP_MOTOR_Context step_motor_cf1;
extern STEP_MOTOR_Context step_motor_cf2;
extern LSMCU_Context lsmcu_ctx;

/*** MANOMETER local global variables ***/

MANOMETER_Context manometer_cp = {0, &step_motor_cp, 100, 3072, 20, 0, 0, 100, 0, 0};
MANOMETER_Context manometer_re = {0, &step_motor_re, 100, 3072, 20, 0, 0, 100, 0, 0};
MANOMETER_Context manometer_cg = {0, &step_motor_cg, 100, 3072, 20, 0, 0, 100, 0, 0};
MANOMETER_Context manometer_cf1 = {0, &step_motor_cf1, 60, 3072, 20, 0, 0, 100, 0, 0};
MANOMETER_Context manometer_cf2 = {0, &step_motor_cf2, 60, 3072, 20, 0, 0, 100, 0, 0};

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
	if (((manometer -> step_motor) -> step) != (manometer -> step_target)) {
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
	STEP_MOTOR_Init(manometer_cp.step_motor);
	STEP_MOTOR_Init(manometer_cg.step_motor);
	STEP_MOTOR_Init(manometer_re.step_motor);
	STEP_MOTOR_Init(manometer_cf1.step_motor);
	STEP_MOTOR_Init(manometer_cf2.step_motor);
	// Link to global context.
	lsmcu_ctx.manometer_cp = &manometer_cp;
	lsmcu_ctx.manometer_re = &manometer_re;
	lsmcu_ctx.manometer_cg = &manometer_cg;
	lsmcu_ctx.manometer_cf1 = &manometer_cf1;
	lsmcu_ctx.manometer_cf2 = &manometer_cf1;
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
	manometer -> step_target = ((manometer -> pressure_max_steps) * pressure_decibars) / (manometer -> pressure_max_decibars);
}

/* GET CURRENT MANOMETER PRESSURE.
 * @param manometer:			Manometer to analyse.
 * @return pressure_decibars:	Current pressure displayed by the manometer in decibars.
 */
unsigned int MANOMETER_GetPressure(MANOMETER_Context* manometer) {
	unsigned int pressure_decibars = (((manometer -> step_motor) -> step) * (manometer -> pressure_max_decibars)) / (manometer -> pressure_max_steps);
	return pressure_decibars;
}

/* START NEEDLE MOVEMENT.
 * @param manometer:	Manometer to control.
 * @return:		None.
 */
void MANOMETER_NeedleStart(MANOMETER_Context* manometer) {
	// Enable movement.
	manometer -> enable = 1;
	// Store current position as start step.
	manometer -> step_start = (manometer -> step_motor -> step);
}

/* STOP NEEDLE MOVEMENT.
 * @param manometer:	Manometer to control.
 * @return:		None.
 */
void MANOMETER_NeedleStop(MANOMETER_Context* manometer) {
	// Disable movement.
	manometer -> enable = 0;
	// Update target to perform inertia.
	// Up direction.
	if (((manometer -> step_motor) -> step) < (manometer -> step_target)) {
		// Current step < (max - inertia)
		if (((manometer -> step_motor) -> step) < ((manometer -> pressure_max_steps) - (manometer -> needle_inertia_steps))) {
			// New target = current + inertia.
			manometer -> step_target = ((manometer -> step_motor) -> step) + (manometer -> needle_inertia_steps);
		}
	}
	// Down direction.
	if (((manometer -> step_motor) -> step) > (manometer -> step_target)) {
		// Current step > (0 + inertia).
		if (((manometer -> step_motor) -> step) > (manometer -> needle_inertia_steps)) {
			// New target = current - inertia.
			manometer -> step_target = ((manometer -> step_motor) -> step) - (manometer -> needle_inertia_steps);
		}
	}
}

/* MANOMETER MAIN TASK (CALLED BY TIM7 INTERRUPT HANDLER EVERY 100us).
 * @param:	None.
 * @return:	None.
 */
void MANOMETER_NeedleTask(MANOMETER_Context* manometer) {
	// Movement feedback loop.
	manometer -> step_it_count++;
	unsigned int current_step = ((manometer -> step_motor) -> step);
	// ChecK if the period was reached.
	if ((manometer -> step_it_count) >= (manometer -> step_it_period)) {
		// Reset interrupt count.
		manometer -> step_it_count = 0;
		// Up direction.
		if (current_step < (manometer -> step_target)) {
			STEP_MOTOR_Up(manometer -> step_motor);
		}
		// Down direction.
		if (current_step > (manometer -> step_target)) {
			STEP_MOTOR_Down(manometer -> step_motor);
		}
	}
	// Compute next period.
	if (current_step != (manometer -> step_target)) {
		// Get absolute distances between start, target and current steps.
		unsigned int delta_start = 0;
		unsigned int delta_target = 0;
		// Up direction.
		if (current_step < (manometer -> step_target)) {
			delta_start = current_step - (manometer -> step_start);
			delta_target = (manometer -> step_target) - current_step;
		}
		// Down direction.
		if (current_step > (manometer -> step_target)) {
			delta_start = (manometer -> step_start) - current_step;
			delta_target = current_step - (manometer -> step_target);
		}
		// Perform linear equation.
		if (delta_start < (manometer -> needle_inertia_steps)) {
			manometer -> step_it_period = MANOMETER_STEP_IT_PERIOD_MAX - ((MANOMETER_STEP_IT_PERIOD_MAX - (manometer -> step_it_period_min)) * (delta_start)) / (manometer -> needle_inertia_steps);
		}
		else {
			if (delta_target < (manometer -> needle_inertia_steps)) {
				manometer -> step_it_period = MANOMETER_STEP_IT_PERIOD_MAX - ((MANOMETER_STEP_IT_PERIOD_MAX - (manometer -> step_it_period_min)) * (delta_target)) / (manometer -> needle_inertia_steps);
			}
			else {
				// Maximum speed.
				manometer -> step_it_period = (manometer -> step_it_period_min);
			}
		}
	}
}
