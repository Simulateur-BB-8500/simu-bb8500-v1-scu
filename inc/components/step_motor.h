/*
 * step_motor.h
 *
 *  Created on: 2 aug. 2019
 *      Author: Ludo
 */

#ifndef __STEP_MOTOR_H__
#define __STEP_MOTOR_H__

#include "gpio.h"

/*** STEP_MOTOR structures ***/

typedef struct {
	const GPIO* gpio_command_1;
	const GPIO* gpio_command_2;
	const GPIO* gpio_stop_detection;
	volatile unsigned int step;
} STEP_MOTOR_context_t;

/*** STEP_MOTOR functions ***/

void STEP_MOTOR_init(STEP_MOTOR_context_t* step_motor);
void STEP_MOTOR_up(STEP_MOTOR_context_t* step_motor);
void STEP_MOTOR_down(STEP_MOTOR_context_t* step_motor);

#endif /* __STEP_MOTOR_H__ */
