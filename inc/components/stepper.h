/*
 * stepper.h
 *
 *  Created on: 2 aug. 2019
 *      Author: Ludo
 */

#ifndef STEPPER_H
#define STEPPER_H

#include "gpio.h"

/*** STEPPER structures ***/

typedef struct {
	const GPIO* stepper_cmd1;
	const GPIO* stepper_cmd2;
	volatile unsigned int stepper_current_step;
} STEPPER_Context;

/*** STEPPER global variables ***/

STEPPER_Context stepper_cp;
STEPPER_Context stepper_re;
STEPPER_Context stepper_cg;
STEPPER_Context stepper_cf1;
STEPPER_Context stepper_cf2;

/*** STEPPER functions ***/

void STEPPER_Init(STEPPER_Context* stepper, const GPIO* stepper_cmd1, const GPIO* stepper_cmd2);
void STEPPER_Up(STEPPER_Context* stepper);
void STEPPER_Down(STEPPER_Context* stepper);

#endif /* STEPPER_H */
