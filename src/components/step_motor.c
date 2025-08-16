/*
 * step_motor.c
 *
 *  Created on: 02 aug. 2019
 *      Author: Ludo
 */

#include "step_motor.h"

#include "gpio.h"
#include "mapping.h"
#include "tim.h"
#include "stdint.h"

/*** STEP MOTOR local macros ***/

#define STEP_MOTOR_STEP_MIN_VALUE               0x00000000
#define STEP_MOTOR_STEP_MAX_VALUE               0xFFFFFFFF
#define STEP_MOTOR_STEP_INITIAL_ZERO_OFFSET     0x80000000 // Must be an multiple of 4 to be aligned with initial GPIO state '00'.

/*** STEP_MOTOR local global variables ***/

STEP_MOTOR_context_t step_motor_cp = { &GPIO_MCP_1, &GPIO_MCP_2, &GPIO_MCP_SD, STEP_MOTOR_STEP_INITIAL_ZERO_OFFSET, STEP_MOTOR_STEP_INITIAL_ZERO_OFFSET, 0 };
STEP_MOTOR_context_t step_motor_re = { &GPIO_MRE_1, &GPIO_MRE_2, &GPIO_MRE_SD, STEP_MOTOR_STEP_INITIAL_ZERO_OFFSET, STEP_MOTOR_STEP_INITIAL_ZERO_OFFSET, 0 };
STEP_MOTOR_context_t step_motor_cg = { &GPIO_MCG_1, &GPIO_MCG_2, &GPIO_MCG_SD, STEP_MOTOR_STEP_INITIAL_ZERO_OFFSET, STEP_MOTOR_STEP_INITIAL_ZERO_OFFSET, 0 };
STEP_MOTOR_context_t step_motor_cf1 = { &GPIO_MCF1_1, &GPIO_MCF1_2, &GPIO_MCF1_SD, STEP_MOTOR_STEP_INITIAL_ZERO_OFFSET, STEP_MOTOR_STEP_INITIAL_ZERO_OFFSET, 0 };
STEP_MOTOR_context_t step_motor_cf2 = { &GPIO_MCF2_1, &GPIO_MCF2_2, &GPIO_MCF2_SD, STEP_MOTOR_STEP_INITIAL_ZERO_OFFSET, STEP_MOTOR_STEP_INITIAL_ZERO_OFFSET, 0 };

/*** STEP_MOTOR local functions ***/

/*******************************************************************/
static void _STEP_MOTOR_single_step(STEP_MOTOR_context_t* step_motor) {
    // Convert step to state.
    switch ((step_motor->step) % 4) {
    // 00.
    case 0:
        GPIO_write((step_motor->gpio_command_1), 0);
        GPIO_write((step_motor->gpio_command_2), 0);
        break;
        // 01.
    case 1:
        GPIO_write((step_motor->gpio_command_1), 1);
        GPIO_write((step_motor->gpio_command_2), 0);
        break;
        // 11.
    case 2:
        GPIO_write((step_motor->gpio_command_1), 1);
        GPIO_write((step_motor->gpio_command_2), 1);
        break;
        // 10.
    case 3:
        GPIO_write((step_motor->gpio_command_1), 0);
        GPIO_write((step_motor->gpio_command_2), 1);
        break;
    }
}

/*******************************************************************/
void _STEP_MOTOR_check_stop_detect(STEP_MOTOR_context_t* step_motor) {
    // Check GPIO.
    if (GPIO_read(step_motor->gpio_stop_detection) == 0) {
        // Update offset and flag.
        (step_motor->step_zero_offset) = (step_motor->step);
        (step_motor->stop_detect_flag) = 1;
    }
    else {
        (step_motor->stop_detect_flag) = 0;
    }
}

/*******************************************************************/
static void _STEP_MOTOR_calibrate(STEP_MOTOR_context_t* step_motor) {
    // Go to stop detection.
    while ((step_motor->stop_detect_flag) == 0) {
        // Step down.
        STEP_MOTOR_down(step_motor);
        TIM2_delay_milliseconds(3);
        // Update state.
        _STEP_MOTOR_check_stop_detect(step_motor);
    }
}

/*** STEP_MOTOR functions ***/

/*******************************************************************/
void STEP_MOTOR_init(STEP_MOTOR_context_t* step_motor) {
    // Init GPIOs.
    GPIO_configure((step_motor->gpio_command_1), GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
    GPIO_configure((step_motor->gpio_command_2), GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
    GPIO_configure((step_motor->gpio_stop_detection), GPIO_MODE_INPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_UP);
    // Calibration.
    _STEP_MOTOR_calibrate(step_motor);
}

/*******************************************************************/
void STEP_MOTOR_up(STEP_MOTOR_context_t* step_motor) {
    // Update and perform step.
    if ((step_motor->step) < STEP_MOTOR_STEP_MAX_VALUE) {
        (step_motor->step)++;
        _STEP_MOTOR_single_step(step_motor);
    }
}

/*******************************************************************/
void STEP_MOTOR_down(STEP_MOTOR_context_t* step_motor) {
    // Check stop detection.
    _STEP_MOTOR_check_stop_detect(step_motor);
    // Perform step only if motor is not on stop position.
    if ((step_motor->stop_detect_flag) == 0) {
        // Update and perform step.
        if ((step_motor->step) > STEP_MOTOR_STEP_MIN_VALUE) {
            (step_motor->step)--;
            _STEP_MOTOR_single_step(step_motor);
        }
    }
}
