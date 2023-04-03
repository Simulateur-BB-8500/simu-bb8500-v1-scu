/*
 * step_motor.c
 *
 *  Created on: 2 aug. 2019
 *      Author: Ludo
 */

#include "step_motor.h"

#include "gpio.h"
#include "mapping.h"
#include "tim.h"
#include "types.h"

/*** STEP_MOTOR local global variables ***/

STEP_MOTOR_context_t step_motor_cp = {&GPIO_MCP_1, &GPIO_MCP_2, &GPIO_MCP_SD, 0};
STEP_MOTOR_context_t step_motor_re = {&GPIO_MRE_1, &GPIO_MRE_2, &GPIO_MRE_SD, 0};
STEP_MOTOR_context_t step_motor_cg = {&GPIO_MCG_1, &GPIO_MCG_2, &GPIO_MCG_SD, 0};
STEP_MOTOR_context_t step_motor_cf1 = {&GPIO_MCF1_1, &GPIO_MCF1_2, &GPIO_MCF1_SD, 0};
STEP_MOTOR_context_t step_motor_cf2 = {&GPIO_MCF2_1, &GPIO_MCF2_2, &GPIO_MCF2_SD, 0};

/*** STEP_MOTOR local functions ***/

/* CALIBRATE STEP MOTOR WITH STOP DETECTION.
 * @param step_motor:	Step motor to control.
 * @return:				None.
 */
static void _STEP_MOTOR_calibrate(STEP_MOTOR_context_t* step_motor) {
	// Reset step value.
	step_motor -> step = 0xFFFFFFFF;
	// Go to stop detection.
	while (GPIO_read(step_motor -> gpio_stop_detection) != 0) {
		STEP_MOTOR_down(step_motor);
		TIM2_delay_milliseconds(10);
	}
	// Set step to zero.
	(step_motor -> step) = 0;
}

/* CONTROL THE STEP_MOTOR GPIO.
 * @param step_motor:	Step motor to control.
 * @return:				None.
 */
static void _STEP_MOTOR_single_step(STEP_MOTOR_context_t* step_motor) {
	// Convert step to state.
	switch ((step_motor -> step) % 4) {
	// 00.
	case 0:
		GPIO_write((step_motor -> gpio_command_1), 0);
		GPIO_write((step_motor -> gpio_command_2), 0);
		break;
	// 01.
	case 1:
		GPIO_write((step_motor -> gpio_command_1), 0);
		GPIO_write((step_motor -> gpio_command_2), 1);
		break;
	// 11.
	case 2:
		GPIO_write((step_motor -> gpio_command_1), 1);
		GPIO_write((step_motor -> gpio_command_2), 1);
		break;
	// 10.
	case 3:
		GPIO_write((step_motor -> gpio_command_1), 1);
		GPIO_write((step_motor -> gpio_command_2), 0);
		break;
	}
}

/*** STEP_MOTOR functions ***/

/* INIT STEP MOTOR.
 * @param step_motor:	Step motor to init.
 * @return:				None.
 */
void STEP_MOTOR_init(STEP_MOTOR_context_t* step_motor) {
	// Init GPIOs.
	GPIO_configure((step_motor -> gpio_command_1), GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure((step_motor -> gpio_command_2), GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure((step_motor -> gpio_stop_detection), GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_UP);
	// Calibration.
	_STEP_MOTOR_calibrate(step_motor);
}

/* PERFORM A MOTOR STEP UP.
 * @param step_motor:	Step motor to control.
 * @return:				None.
 */
void STEP_MOTOR_up(STEP_MOTOR_context_t* step_motor) {
	// Update and perform step.
	(step_motor -> step)++;
	_STEP_MOTOR_single_step(step_motor);
}

/* PERFORM A MOTOR STEP DOWN.
 * @param step_motor:	Step motor to control.
 * @return:				None.
 */
void STEP_MOTOR_down(STEP_MOTOR_context_t* step_motor) {
	// Check stop detection.
	if (GPIO_read(step_motor -> gpio_stop_detection) == 0) {
		// Recalibrate motor.
		(step_motor -> step) = 0;
	}
	else {
		// Update and perform step.
		if ((step_motor -> step) > 0) {
			(step_motor -> step)--;
		}
		_STEP_MOTOR_single_step(step_motor);
	}
}
