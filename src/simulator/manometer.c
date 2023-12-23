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
#include "stdint.h"

/*** MANOMETER local macros ***/

#define MANOMETER_STEP_IRQ_PERIOD_US		100

#define MANOMETER_GEAR_G1_Z					36
#define MANOMETER_GEAR_G2_Z					56
#define MANOMETER_GEAR_CENTER_Z				20

#define MANOMETER_PRESSURE_MAX_DEGREES		270
#define MANOMETER_FULL_CIRCLE_DEGREES		360

#define MANOMETER_PRESSURE_MAX_STEP_G1		((STEP_MOTOR_NUMBER_OF_STEPS * MANOMETER_PRESSURE_MAX_DEGREES * MANOMETER_GEAR_CENTER_Z) / (MANOMETER_GEAR_G1_Z * MANOMETER_FULL_CIRCLE_DEGREES))
#define MANOMETER_PRESSURE_MAX_STEP_G2		((STEP_MOTOR_NUMBER_OF_STEPS * MANOMETER_PRESSURE_MAX_DEGREES * MANOMETER_GEAR_CENTER_Z) / (MANOMETER_GEAR_G2_Z * MANOMETER_FULL_CIRCLE_DEGREES))

#define MANOMETER_STEP_IRQ_PER_SECOND		(1000000 / MANOMETER_STEP_IRQ_PERIOD_US)
#define MANOMETER_STEP_IRQ_PERIOD_MIN_US	2000 // Minimum delay between coil pins change.
#define MANOMETER_STEP_IRQ_COUNT_MIN		(MANOMETER_STEP_IRQ_PERIOD_MIN_US / MANOMETER_STEP_IRQ_PERIOD_US)

/*** MANOMETER external global variables ***/

extern STEP_MOTOR_context_t step_motor_cp;
extern STEP_MOTOR_context_t step_motor_re;
extern STEP_MOTOR_context_t step_motor_cg;
extern STEP_MOTOR_context_t step_motor_cf1;
extern STEP_MOTOR_context_t step_motor_cf2;
extern LSMCU_Context lsmcu_ctx;

/*** MANOMETER local global variables ***/

static MANOMETER_context_t manometer_cp =  {&step_motor_cp,  9500, 10000, (MANOMETER_PRESSURE_MAX_STEP_G1 + 0),  0, 0,  0, 0, 0, 0, 0, 0, 0};
static MANOMETER_context_t manometer_re =  {&step_motor_re,  5400, 10000, (MANOMETER_PRESSURE_MAX_STEP_G2 + 20), 0, 30, 3, 0, 0, 0, 0, 0, 0};
static MANOMETER_context_t manometer_cg =  {&step_motor_cg,  5400, 10000, (MANOMETER_PRESSURE_MAX_STEP_G1 + 60), 0, 50, 2, 0, 0, 0, 0, 0, 0};
static MANOMETER_context_t manometer_cf1 = {&step_motor_cf1, 4100, 6000,  (MANOMETER_PRESSURE_MAX_STEP_G1 + 35), 0, 50, 2, 0, 0, 0, 0, 0, 0};
static MANOMETER_context_t manometer_cf2 = {&step_motor_cf2, 4200, 6000,  (MANOMETER_PRESSURE_MAX_STEP_G2 + 30), 0, 30, 3, 0, 0, 0, 0, 0, 0};

/*** MANOMETER local functions ***/

/* MANOMETER MAIN TASK (CALLED BY TIM7 INTERRUPT HANDLER EVERY 100us).
 * @param:	None.
 * @return:	None.
 */
static void __attribute__((optimize("-O0"))) _MANOMETER_needle_task(MANOMETER_context_t* manometer) {
	// Local variables.
	uint32_t current_step = (manometer -> step_motor -> step);
	uint32_t delta_start = 0;
	uint32_t delta_target = 0;
	uint32_t step_irq_period_max = (manometer -> step_irq_period_min) * (manometer -> needle_inertia_factor);
	// Movement feedback loop.
	(manometer -> step_irq_count)++;
	// Check if the period was reached.
	if ((manometer -> step_irq_count) >= (manometer -> step_irq_period)) {
		// Reset interrupt count.
		(manometer -> step_irq_count) = 0;
		// Up direction.
		if (current_step < (manometer -> step_target)) {
			STEP_MOTOR_up(manometer -> step_motor);
			// Compute absolute distances between start, target and current steps.
			delta_start = current_step - (manometer -> step_start);
			delta_target = (manometer -> step_target) - current_step;
		}
		// Down direction.
		if (current_step > (manometer -> step_target)) {
			STEP_MOTOR_down(manometer -> step_motor);
			// Compute absolute distances between start, target and current steps.
			delta_start = (manometer -> step_start) - current_step;
			delta_target = current_step - (manometer -> step_target);
		}
		// Compute next period.
		if (current_step != (manometer -> step_target)) {
			// Perform linear equation.
			if (delta_start < (manometer -> needle_inertia_steps)) {
				(manometer -> step_irq_period) = step_irq_period_max - ((step_irq_period_max - (manometer -> step_irq_period_min)) * (delta_start)) / (manometer -> needle_inertia_steps);
			}
			else {
				if (delta_target < (manometer -> needle_inertia_steps)) {
					(manometer -> step_irq_period) = step_irq_period_max - ((step_irq_period_max - (manometer -> step_irq_period_min)) * (delta_target)) / (manometer -> needle_inertia_steps);
				}
				else {
					// Maximum speed.
					(manometer -> step_irq_period) = (manometer -> step_irq_period_min);
				}
			}
		}
		// Force step down if current step and target are equal but stop detect is not reached.
		if ((current_step == (manometer -> step_target)) && ((manometer -> step_target_zero_flag) != 0) && ((manometer -> step_motor -> stop_detect_flag) == 0)) {
			// Maximum speed.
			(manometer -> step_irq_period) = (manometer -> step_irq_period_min);
			// Step down.
			STEP_MOTOR_down(manometer -> step_motor);
			// Align target value on new step.
			(manometer -> step_target) = (manometer -> step_motor -> step);
		}
		else {
			if (current_step == (manometer -> step_target)) {
				(manometer -> is_moving) = 0;
			}
		}
	}
}

static void _MANOMETER_needle_task_all(void) {
	// Perform manometers needle control.
	_MANOMETER_needle_task(lsmcu_ctx.manometer_cp);
	_MANOMETER_needle_task(lsmcu_ctx.manometer_re);
	_MANOMETER_needle_task(lsmcu_ctx.manometer_cg);
	_MANOMETER_needle_task(lsmcu_ctx.manometer_cf1);
	_MANOMETER_needle_task(lsmcu_ctx.manometer_cf2);
}

/* POWER MANOMETER DRIVERS.
 * @param:	None.
 * @return:	None.
 */
static void _MANOMETER_power_on_all(void) {
	// Turn step motors on.
	GPIO_write(&GPIO_MANOMETER_POWER_ENABLE, 1);
	// Start timer.
	TIM7_start();
}

/* POWER MANOMETER DRIVERS.
 * @param:	None.
 * @return:	None.
 */
static void _MANOMETER_power_off_all(void) {
	// Stop timer.
	TIM7_stop();
	// Turn step motors on.
	GPIO_write(&GPIO_MANOMETER_POWER_ENABLE, 0);
}

/* UPDATE MANOMETER ZERO TARGET FLAG.
 * @param manometer:	Manometer to update.
 * @return:				None.
 */
static void _MANOMETER_update_zero_target_flag(MANOMETER_context_t* manometer) {
	// Update flag.
	(manometer -> step_target_zero_flag) = ((manometer -> step_target) == (manometer -> step_motor -> step_zero_offset)) ? 1 : 0;
}

/*** MANOMETER functions ***/

/* MANOMETER COMMON INIT.
 * @param:	None.
 * @return:	None.
 */
void MANOMETER_init_all(void) {
	// Init GPIOs.
	GPIO_configure(&GPIO_MANOMETER_POWER_ENABLE, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init step motors.
	GPIO_write(&GPIO_MANOMETER_POWER_ENABLE, 1);
	STEP_MOTOR_init(manometer_cp.step_motor);
	STEP_MOTOR_init(manometer_re.step_motor);
	STEP_MOTOR_init(manometer_cg.step_motor);
	STEP_MOTOR_init(manometer_cf1.step_motor);
	STEP_MOTOR_init(manometer_cf2.step_motor);
	GPIO_write(&GPIO_MANOMETER_POWER_ENABLE, 0);
	// Link to global context.
	lsmcu_ctx.manometer_cp = &manometer_cp;
	lsmcu_ctx.manometer_re = &manometer_re;
	lsmcu_ctx.manometer_cg = &manometer_cg;
	lsmcu_ctx.manometer_cf1 = &manometer_cf1;
	lsmcu_ctx.manometer_cf2 = &manometer_cf2;
	// Init step motor control timer.
	TIM7_init(MANOMETER_STEP_IRQ_PERIOD_US, &_MANOMETER_needle_task_all);
}

/* CONTROL MANOMETER POWER.
 * @param:	None.
 * @return:	None.
 */
void MANOMETER_manage_power_all(void) {
	// Check all manometer.
	if ((manometer_cp.is_moving  == 0) &&
		(manometer_re.is_moving  == 0) &&
		(manometer_cg.is_moving  == 0) &&
		(manometer_cf1.is_moving == 0) &&
		(manometer_cf2.is_moving == 0)) {
		// Turn manometer off.
		_MANOMETER_power_off_all();
	}
}

/* UPDATE PRESSURE TARGET.
 * @param manometer:				Manometer to control.
 * @param pressure_mbar:			New pressure expressed in millibar.
 * @param speed_mbar_per_second:	Needle speed in millibar per second.
 */
void MANOMETER_set_pressure(MANOMETER_context_t* manometer, uint32_t pressure_mbar, uint32_t speed_mbar_per_second) {
	// Local variables.
	uint32_t new_step_target = 0;
	// Compute new target.
	new_step_target = (manometer -> step_motor -> step_zero_offset) + (((manometer -> pressure_max_steps) * pressure_mbar) / (manometer -> pressure_max_mbar));
	// Only update on target change.
	if ((manometer -> step_target) != new_step_target) {
		// Update target step.
		(manometer -> step_target) = new_step_target;
		// Store current position as start step.
		(manometer -> step_start) = (manometer -> step_motor -> step);
		// Convert speed to IRQ period.
		(manometer -> step_irq_period_min) = ((manometer -> pressure_max_mbar) * MANOMETER_STEP_IRQ_PER_SECOND) / (speed_mbar_per_second * (manometer -> pressure_max_steps));
		// Clamp value.
		if ((manometer -> step_irq_period_min) < MANOMETER_STEP_IRQ_COUNT_MIN) {
			manometer -> step_irq_period_min = MANOMETER_STEP_IRQ_COUNT_MIN;
		}
		// Init step IRQ period to maximum speed for first interrupt.
		(manometer -> step_irq_period) = (manometer -> step_irq_period_min);
		(manometer -> step_irq_count) = manometer -> step_irq_period;
		// Update flags.
		(manometer -> is_moving) = 1;
		_MANOMETER_update_zero_target_flag(manometer);
		// Turn manometers on.
		_MANOMETER_power_on_all();
	}
}

/* GET CURRENT MANOMETER PRESSURE.
 * @param manometer:		Manometer to read.
 * @return pressure_mbar:	Current pressure displayed by the manometer in millibar.
 */
uint32_t MANOMETER_get_pressure(MANOMETER_context_t* manometer) {
	// Convert step to pressure.
	uint32_t pressure_mbar = 0;
	// Check step and offset.
	if ((manometer -> step_motor -> step) > (manometer -> step_motor -> step_zero_offset)) {
		// Compute linear formula.
		pressure_mbar = (((manometer -> step_motor -> step) - (manometer -> step_motor -> step_zero_offset)) * (manometer -> pressure_max_mbar)) / (manometer -> pressure_max_steps);
	}
	return pressure_mbar;
}

/* CHECK IF PRESSURE IS INCREASING.
 * @param manometer:	Manometer to read.
 * @return increasing:	1 if the pressure is currently increasing, 0 otherwise.
 */
uint8_t MANOMETER_is_pressure_increasing(MANOMETER_context_t* manometer) {
	uint8_t increasing = 0;
	if ((manometer -> step_motor -> step) < (manometer -> step_target)) {
		increasing = 1;
	}
	return increasing;
}

/* STOP NEEDLE MOVEMENT.
 * @param manometer:	Manometer to control.
 * @return:				None.
 */
void MANOMETER_needle_stop(MANOMETER_context_t* manometer) {
	// Update target to perform inertia.
	// Up direction.
	if ((manometer -> step_motor -> step) < (manometer -> step_target)) {
		// Current step < (max - inertia)
		if ((manometer -> step_motor -> step) < ((manometer -> step_motor -> step_zero_offset) + (manometer -> pressure_max_steps) - (manometer -> needle_inertia_steps))) {
			// New target = current + inertia.
			(manometer -> step_target) = (manometer -> step_motor -> step) + (manometer -> needle_inertia_steps);
		}
	}
	// Down direction.
	if ((manometer -> step_motor -> step) > (manometer -> step_target)) {
		// Current step > (0 + inertia).
		if ((manometer -> step_motor -> step) > ((manometer -> step_motor -> step_zero_offset) + (manometer -> needle_inertia_steps))) {
			// New target = current - inertia.
			(manometer -> step_target) = (manometer -> step_motor -> step) - (manometer -> needle_inertia_steps);
		}
	}
	// Update zero target flag.
	_MANOMETER_update_zero_target_flag(manometer);
}
