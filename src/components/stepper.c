/*
 * stepper.c
 *
 *  Created on: 2 aug. 2019
 *      Author: Ludo
 */

#include "stepper.h"

#include "gpio.h"
#include "mapping.h"

/*** STEPPER local global variables ***/

STEPPER_Context stepper_cp = {&GPIO_MCP_1, &GPIO_MCP_2, 0};
STEPPER_Context stepper_re = {&GPIO_MRE_1, &GPIO_MRE_2, 0};
STEPPER_Context stepper_cg = {&GPIO_MCG_1, &GPIO_MCG_2, 0};
STEPPER_Context stepper_cf1 = {&GPIO_MCF1_1, &GPIO_MCF1_2, 0};
STEPPER_Context stepper_cf2 = {&GPIO_MCF2_1, &GPIO_MCF2_2, 0};

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
 * @param stepper:	Step motor to init.
 * @return:			None.
 */
void STEPPER_Init(STEPPER_Context* stepper) {
	// Init GPIOs.
	GPIO_Configure((stepper -> stepper_cmd1), GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_Configure((stepper -> stepper_cmd2), GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init context.
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
