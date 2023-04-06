/*
 * manometer.c
 *
 *  Created on: 3 aug. 2019
 *      Author: Ludo
 */

#include "manometer.h"

#include "lsmcu.h"
#include "mapping.h"
#include "mode.h"
#include "step_motor.h"
#include "tim.h"
#include "stdint.h"
#ifdef DEBUG
#include "string.h"
#include "usart.h"
#endif

/*** MANOMETER local macros ***/

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

static MANOMETER_context_t manometer_cp =  {&step_motor_cp,  9500, 10000, (MANOMETER_PRESSURE_MAX_STEP_G1 + 35), 0,  0, 0, 0, 0, 0, 0};
static MANOMETER_context_t manometer_re =  {&step_motor_re,  5000, 10000, (MANOMETER_PRESSURE_MAX_STEP_G2 + 20), 30, 3, 0, 0, 0, 0, 0};
static MANOMETER_context_t manometer_cg =  {&step_motor_cg,  5400, 10000, (MANOMETER_PRESSURE_MAX_STEP_G1 + 60), 50, 2, 0, 0, 0, 0, 0};
static MANOMETER_context_t manometer_cf1 = {&step_motor_cf1, 4100, 6000,  (MANOMETER_PRESSURE_MAX_STEP_G1 + 35), 50, 2, 0, 0, 0, 0, 0};
static MANOMETER_context_t manometer_cf2 = {&step_motor_cf2, 4200, 6000,  (MANOMETER_PRESSURE_MAX_STEP_G2 + 30), 30, 3, 0, 0, 0, 0, 0};

/*** MANOMETER local functions ***/

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

/* CHECK IF A GIVEN NEEDLE IS MOVING.
 * @param:			None.
 * @eturn moving:	'1' if the manometer needle is currently moving (target not reached), '0' otherwise.
 */
static uint8_t _MANOMETER_needle_is_moving(MANOMETER_context_t* manometer) {
	uint8_t moving = 0;
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
}

/* CONTROL MANOMETER POWER.
 * @param:	None.
 * @return:	None.
 */
void MANOMETER_manage_power_all(void) {
	// Check all manometer.
	if ((_MANOMETER_needle_is_moving(&manometer_cp) == 0) &&
		(_MANOMETER_needle_is_moving(&manometer_re) == 0) &&
		(_MANOMETER_needle_is_moving(&manometer_cg) == 0) &&
		(_MANOMETER_needle_is_moving(&manometer_cf1) == 0) &&
		(_MANOMETER_needle_is_moving(&manometer_cf2) == 0)) {
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
	// Turn manometers on.
	_MANOMETER_power_on_all();
	// Store current position as start step.
	manometer -> step_start = (manometer -> step_motor -> step);
	// Update target step.
	manometer -> step_target = ((manometer -> pressure_max_steps) * pressure_mbar) / (manometer -> pressure_max_mbar);
	// Convert speed to IRQ period.
	manometer -> step_irq_period_min = ((manometer -> pressure_max_mbar) * MANOMETER_STEP_IRQ_PER_SECOND) / (speed_mbar_per_second * (manometer -> pressure_max_steps));
	// Clamp value.
	if ((manometer -> step_irq_period_min) < MANOMETER_STEP_IRQ_COUNT_MIN) {
		manometer -> step_irq_period_min = MANOMETER_STEP_IRQ_COUNT_MIN;
	}
	// Init step IRQ period to maximum speed for first interrupt.
	manometer -> step_irq_period = (manometer -> step_irq_period_min);
	manometer -> step_irq_count = manometer -> step_irq_period;
}

/* GET CURRENT MANOMETER PRESSURE.
 * @param manometer:		Manometer to read.
 * @return pressure_mbar:	Current pressure displayed by the manometer in millibar.
 */
uint32_t MANOMETER_get_pressure(MANOMETER_context_t* manometer) {
	// Convert step to pressure.
	uint32_t pressure_mbar = (((manometer -> step_motor) -> step) * (manometer -> pressure_max_mbar)) / (manometer -> pressure_max_steps);
	return pressure_mbar;
}

/* STOP NEEDLE MOVEMENT.
 * @param manometer:	Manometer to control.
 * @return:				None.
 */
void MANOMETER_needle_stop(MANOMETER_context_t* manometer) {
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
void __attribute__((optimize("-O0"))) MANOMETER_needle_task(MANOMETER_context_t* manometer) {
	// Local variables.
	uint32_t current_step = ((manometer -> step_motor) -> step);
	uint32_t delta_start = 0;
	uint32_t delta_target = 0;
	uint32_t step_irq_period_max = (manometer -> step_irq_period_min) * (manometer -> needle_inertia_factor);
	// Movement feedback loop.
	manometer -> step_irq_count++;
	// ChecK if the period was reached.
	if ((manometer -> step_irq_count) >= (manometer -> step_irq_period)) {
		// Reset interrupt count.
		manometer -> step_irq_count = 0;
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
				manometer -> step_irq_period = step_irq_period_max - ((step_irq_period_max - (manometer -> step_irq_period_min)) * (delta_start)) / (manometer -> needle_inertia_steps);
			}
			else {
				if (delta_target < (manometer -> needle_inertia_steps)) {
					manometer -> step_irq_period = step_irq_period_max - ((step_irq_period_max - (manometer -> step_irq_period_min)) * (delta_target)) / (manometer -> needle_inertia_steps);
				}
				else {
					// Maximum speed.
					manometer -> step_irq_period = (manometer -> step_irq_period_min);
				}
			}
		}
	}
}

#ifdef DEBUG
/* PRINT ALL MANOMETERS PRESSURE.
 * @param:	None.
 * @return:	None.
 */
void MANOMETER_print_data(void) {
	// Local variables.
	char str_value[16];
	// Print all pressure values.
	USART1_send_string("\nCP  = ");
	STRING_value_to_string(MANOMETER_get_pressure(lsmcu_ctx.manometer_cp), STRING_FORMAT_DECIMAL, 0, str_value);
	USART1_send_string(str_value);
	USART1_send_string("\nRE  = ");
	STRING_value_to_string(MANOMETER_get_pressure(lsmcu_ctx.manometer_re), STRING_FORMAT_DECIMAL, 0, str_value);
	USART1_send_string(str_value);
	USART1_send_string("\nCG  = ");
	STRING_value_to_string(MANOMETER_get_pressure(lsmcu_ctx.manometer_cg), STRING_FORMAT_DECIMAL, 0, str_value);
	USART1_send_string(str_value);
	USART1_send_string("\nCF1 = ");
	STRING_value_to_string(MANOMETER_get_pressure(lsmcu_ctx.manometer_cf1), STRING_FORMAT_DECIMAL, 0, str_value);
	USART1_send_string(str_value);
	USART1_send_string("\nCF2 = ");
	STRING_value_to_string(MANOMETER_get_pressure(lsmcu_ctx.manometer_cf2), STRING_FORMAT_DECIMAL, 0, str_value);
	USART1_send_string(str_value);
	USART1_send_string("\n");
}
#endif
