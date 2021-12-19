/*
 * step_motor.h
 *
 *  Created on: 2 aug. 2019
 *      Author: Ludo
 */

#ifndef STEP_MOTOR_H
#define STEP_MOTOR_H

#include "gpio.h"

/*** STEP_MOTOR structures ***/

typedef struct {
	const GPIO* gpio_1;
	const GPIO* gpio_2;
	volatile unsigned int step;
} STEP_MOTOR_Context;

/*** STEP_MOTOR functions ***/

void STEP_MOTOR_Init(STEP_MOTOR_Context* step_motor);
void STEP_MOTOR_Up(STEP_MOTOR_Context* step_motor);
void STEP_MOTOR_Down(STEP_MOTOR_Context* step_motor);

#endif /* STEP_MOTOR_H */
