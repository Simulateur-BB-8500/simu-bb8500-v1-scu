/*
 * step_motor.h
 *
 *  Created on: 02 aug. 2019
 *      Author: Ludo
 */

#ifndef __STEP_MOTOR_H__
#define __STEP_MOTOR_H__

#include "gpio.h"
#include "stdint.h"

/*** STEP MOTOR macros ***/

#define STEP_MOTOR_NUMBER_OF_STEPS  2048

/*** STEP MOTOR structures ***/

/*!******************************************************************
 * \enum STEP_MOTOR_context_t
 * \brief Step motor context structure.
 *******************************************************************/
typedef struct {
    const GPIO_pin_t* gpio_command_1;
    const GPIO_pin_t* gpio_command_2;
    const GPIO_pin_t* gpio_stop_detection;
    volatile uint32_t step;
    volatile uint32_t step_zero_offset;
    volatile uint8_t stop_detect_flag;
} STEP_MOTOR_context_t;

/*** STEP_MOTOR functions ***/

/*!******************************************************************
 * \fn void STEP_MOTOR_init(STEP_MOTOR_context_t* step_motor)
 * \brief Init step motor.
 * \param[in]   step_motor: Pointer to the step motor context to initialize.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void STEP_MOTOR_init(STEP_MOTOR_context_t* step_motor);

/*!******************************************************************
 * \fn void STEP_MOTOR_up(STEP_MOTOR_context_t* step_motor)
 * \brief Make an up step.
 * \param[in]   step_motor: Pointer to the step motor to control.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void STEP_MOTOR_up(STEP_MOTOR_context_t* step_motor);

/*!******************************************************************
 * \fn void STEP_MOTOR_down(STEP_MOTOR_context_t* step_motor)
 * \brief Make a down step.
 * \param[in]   step_motor: Pointer to the step motor to control.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void STEP_MOTOR_down(STEP_MOTOR_context_t* step_motor);

#endif /* __STEP_MOTOR_H__ */
