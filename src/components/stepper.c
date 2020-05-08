/*
 * stepper.c
 *
 *  Created on: 2 aug. 2019
 *      Author: Ludo
 */

#include "stepper.h"

#include "gpio.h"

/*** STEPPER local functions ***/

/* CONTROL THE STEPPER GPIO.
 * @param stepper:	Step motor to control.
 * @return:			None.
 */
void STEPPER_SingleStep(STEPPER_Context* stepper) {
	// Convert step to state.
	switch ((stepper -> stepper_current_step) % 4) {
	// 00.
	case 0:
		GPIO_Write((stepper -> stepper_cmd1), 0);
		GPIO_Write((stepper -> stepper_cmd2), 0);
		break;
	// 01.
	case 1:
		GPIO_Write((stepper -> stepper_cmd1), 0);
		GPIO_Write((stepper -> stepper_cmd2), 1);
		break;
	// 11.
	case 2:
		GPIO_Write((stepper -> stepper_cmd1), 1);
		GPIO_Write((stepper -> stepper_cmd2), 1);
		break;
	// 10.
	case 3:
		GPIO_Write((stepper -> stepper_cmd1), 1);
		GPIO_Write((stepper -> stepper_cmd2), 0);
		break;
	}
}

/*** STEPPER functions ***/

/* INIT STEP MOTOR.
 * @param stepper:		Step motor to control.
 * @param stepper_cmd1:	GPIO attached to the motor pin 1.
 * @param stepper_cmd2:	GPIO attached to the motor pin 2.
 * @return:				None.
 */
void STEPPER_Init(STEPPER_Context* stepper, const GPIO* stepper_cmd1, const GPIO* stepper_cmd2) {
	// Init GPIOs.
	GPIO_Configure((stepper -> stepper_cmd1), GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure((stepper -> stepper_cmd2), GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init context.
	stepper -> stepper_cmd1 = stepper_cmd1;
	stepper -> stepper_cmd2 = stepper_cmd2;
	stepper -> stepper_current_step = 0;
}

/* PERFORM A MOTOR STEP UP.
 * @param:	None.
 * @return:	None.
 */
void STEPPER_Up(STEPPER_Context* stepper) {
	// Update and perform step.
	stepper -> stepper_current_step++;
	STEPPER_SingleStep(stepper);
}

/* PERFORM A MOTOR STEP DOWN.
 * @param:	None.
 * @return:	None.
 */
void STEPPER_Down(STEPPER_Context* stepper) {
	// Update and perform step.
	if ((stepper -> stepper_current_step) > 0) {
		stepper -> stepper_current_step--;
	}
	STEPPER_SingleStep(stepper);
}
