/*
 * mp.c
 *
 *  Created on: 22 mar. 2020
 *      Author: Ludo
 */

#include "mp.h"

#include "gpio.h"
#include "mapping.h"
#include "scu.h"
#include "scu_commands.h"
#include "sgdu.h"
#include "sw2.h"
#include "tim.h"
#include "stdint.h"
#include "usart.h"

/*** MP local macros ***/

#define MP_VARIATOR_SWITCH_PERIOD_TABLE_SIZE    30

/*** MP local structures ***/

/*******************************************************************/
typedef struct {
    // GPIOs.
    SW2_context_t zero;
    SW2_context_t t_more;
    SW2_context_t t_less;
    SW2_context_t t_fast;
    SW2_context_t preparation;
    SW2_context_t f_more;
    SW2_context_t f_less;
    SW2_context_t f_fast;
    SW2_context_t transition;
    // State machine.
    uint8_t zero_on;
    uint8_t t_more_lock;
    uint8_t t_less_lock;
    uint8_t t_fast_on;
    uint8_t preparation_on;
    uint8_t f_more_lock;
    uint8_t f_less_lock;
    uint8_t f_fast_on;
    uint8_t transition_lock;
    // Variator management.
    int8_t variator_step_target;
    uint32_t variator_switch_next_time;
    uint8_t variator_switch_period_idx;
} MP_context_t;

/*** MP external global variables ***/

extern SCU_context_t scu_ctx;

/*** MP local global variables ***/

static const uint32_t MP_VARIATOR_SWITCH_PERIOD_MS[MP_VARIATOR_SWITCH_PERIOD_TABLE_SIZE] = { 300, 300, 300, 300, 300, 600, 300, 300, 600, 300, 600, 300, 300, 300, 300, 600, 300, 300, 300, 300, 300, 300, 300, 600, 300, 600, 300, 300, 300, 300 };
static MP_context_t mp_ctx;

/*** MP local functions ***/

/*******************************************************************/
static void _MP_synchronize(void) {
    // Local variables.
    SCU_output_command_t mp_command = SCU_OUT_NOP;
    // Check target.
    if ((scu_ctx.variator_step != mp_ctx.variator_step_target) && (TIM2_get_milliseconds() > mp_ctx.variator_switch_next_time)) {
        // More direction.
        if (scu_ctx.variator_step < mp_ctx.variator_step_target) {
            // Check current direction.
            if (scu_ctx.variator_step < (MP_VARIATOR_STEP_P - 1)) {
                mp_command = SCU_OUT_MP_F_LESS;
            }
            else if (scu_ctx.variator_step == (MP_VARIATOR_STEP_P - 1)) {
                mp_command = SCU_OUT_MP_P;
            }
            else if (scu_ctx.variator_step == MP_VARIATOR_STEP_P) {
                mp_command = SCU_OUT_MP_0;
            }
            else {
                mp_command = SCU_OUT_MP_T_MORE;
            }
            scu_ctx.variator_step++;
        }
        // Less direction.
        else {
            // Check current direction.
            if (scu_ctx.variator_step > (MP_VARIATOR_STEP_0 + 1)) {
                mp_command = SCU_OUT_MP_T_LESS;
            }
            else if (scu_ctx.variator_step == (MP_VARIATOR_STEP_0 + 1)) {
                mp_command = SCU_OUT_MP_0;
            }
            else if (scu_ctx.variator_step == MP_VARIATOR_STEP_0) {
                mp_command = SCU_OUT_MP_P;
            }
            else {
                mp_command = SCU_OUT_MP_F_MORE;
            }
            scu_ctx.variator_step--;
        }
        // Send command.
        SGDU_write(mp_command);
        // Blink RH light.
        scu_ctx.lsrh_blink_request = 1;
        // Update time.
        mp_ctx.variator_switch_next_time = (TIM2_get_milliseconds() + MP_VARIATOR_SWITCH_PERIOD_MS[mp_ctx.variator_switch_period_idx]);
        mp_ctx.variator_switch_period_idx = (mp_ctx.variator_switch_period_idx + 1) % MP_VARIATOR_SWITCH_PERIOD_TABLE_SIZE;
    }
    // Motors coupling.
    scu_ctx.motors_coupling = (scu_ctx.variator_step > MP_VARIATOR_STEP_MAX_SERIES_COUPLING) ? MP_MOTORS_COUPLING_PARALLEL : MP_MOTORS_COUPLING_SERIES;
}

/*** MP functions ***/

/*******************************************************************/
void MP_init(void) {
    // Init GPIOs.
    SW2_init(&mp_ctx.zero, &GPIO_MP_0, 1, 100); // MP_0 active high.
    SW2_init(&mp_ctx.t_more, &GPIO_MP_TM, 1, 50); // MP_TP active high.
    SW2_init(&mp_ctx.t_less, &GPIO_MP_TL, 1, 50); // MP_TM active high.
    SW2_init(&mp_ctx.t_fast, &GPIO_MP_TF, 1, 100); // MP_PR active high.
    SW2_init(&mp_ctx.preparation, &GPIO_MP_P, 1, 100); // MP_P active high.
    SW2_init(&mp_ctx.f_more, &GPIO_MP_FM, 1, 50); // MP_FP active high.
    SW2_init(&mp_ctx.f_less, &GPIO_MP_FL, 1, 50); // MP_FM active high.
    SW2_init(&mp_ctx.f_fast, &GPIO_MP_FF, 1, 100); // MP_FR active high.
    SW2_init(&mp_ctx.transition, &GPIO_MP_TR, 0, 100); // MP_TR active low.
    GPIO_configure(&GPIO_MP_SH_ENABLE, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
    // Init context.
    mp_ctx.zero_on = 0;
    mp_ctx.t_more_lock = 0;
    mp_ctx.t_less_lock = 0;
    mp_ctx.preparation_on = 0;
    mp_ctx.f_more_lock = 0;
    mp_ctx.f_less_lock = 0;
    mp_ctx.transition_lock = 0;
    mp_ctx.variator_step_target = 0;
    mp_ctx.variator_switch_next_time = 0;
    mp_ctx.variator_switch_period_idx = 0;
    // Init global context.
    scu_ctx.variator_step = 0;
    scu_ctx.motors_coupling = MP_MOTORS_COUPLING_SERIES;
}

/*******************************************************************/
void MP_process(void) {
    // Local variables.
    int8_t variator_max = ((scu_ctx.motors_coupling) == MP_MOTORS_COUPLING_SERIES) ? MP_VARIATOR_STEP_MAX_SERIES_COUPLING : MP_VARIATOR_STEP_MAX;
    // Update all switches.
    SW2_update_state(&mp_ctx.zero);
    SW2_update_state(&mp_ctx.t_more);
    SW2_update_state(&mp_ctx.t_less);
    SW2_update_state(&mp_ctx.t_fast);
    SW2_update_state(&mp_ctx.preparation);
    SW2_update_state(&mp_ctx.f_more);
    SW2_update_state(&mp_ctx.f_less);
    SW2_update_state(&mp_ctx.f_fast);
    SW2_update_state(&mp_ctx.transition);
    // Check DJ.
    if (scu_ctx.dj_locked != 0) {
        // MP.0
        if (mp_ctx.zero.state == SW2_ON) {
            if (mp_ctx.zero_on == 0) {
                mp_ctx.variator_step_target = MP_VARIATOR_STEP_0;
            }
            mp_ctx.zero_on = 1;
        }
        else {
            if (mp_ctx.zero_on != 0) {
                mp_ctx.variator_step_target = scu_ctx.variator_step;
            }
            mp_ctx.zero_on = 0;
        }
        // MP.TM
        if (mp_ctx.t_more.state == SW2_ON) {
            if ((mp_ctx.t_more_lock == 0) && (mp_ctx.variator_step_target < variator_max)) {
                // Increase step count.
                mp_ctx.variator_step_target++;
            }
            mp_ctx.t_more_lock = 1;
        }
        else {
            mp_ctx.t_more_lock = 0;
        }
        // MP.TL
        if (mp_ctx.t_less.state == SW2_ON) {
            if ((mp_ctx.t_less_lock == 0) && (mp_ctx.variator_step_target > (MP_VARIATOR_STEP_0 + 1))) {
                // Decrease step count.
                mp_ctx.variator_step_target--;
            }
            mp_ctx.t_less_lock = 1;
        }
        else {
            mp_ctx.t_less_lock = 0;
        }
        // MP.PR
        if (mp_ctx.t_fast.state == SW2_ON) {
            if ((mp_ctx.t_fast_on == 0) && (mp_ctx.variator_step_target < variator_max)) {
                // Set target to maximum.
                mp_ctx.variator_step_target = variator_max;
            }
            mp_ctx.t_fast_on = 1;
        }
        else {
            if (mp_ctx.t_fast_on != 0) {
                // Set target to current.
                mp_ctx.variator_step_target = scu_ctx.variator_step;
            }
            mp_ctx.t_fast_on = 0;
        }
        // MP.P
        if (mp_ctx.preparation.state == SW2_ON) {
            if (mp_ctx.preparation_on == 0) {
                mp_ctx.variator_step_target = MP_VARIATOR_STEP_P;
            }
            mp_ctx.preparation_on = 1;
        }
        else {
            if (mp_ctx.preparation_on != 0) {
                mp_ctx.variator_step_target = scu_ctx.variator_step;
            }
            mp_ctx.preparation_on = 0;
        }
        // MP.FM
        if (mp_ctx.f_more.state == SW2_ON) {
            if ((mp_ctx.f_more_lock == 0) && (mp_ctx.variator_step_target > MP_VARIATOR_STEP_MIN)) {
                // Decrease step count.
                mp_ctx.variator_step_target--;
            }
            mp_ctx.f_more_lock = 1;
        }
        else {
            mp_ctx.f_more_lock = 0;
        }
        // MP.FL
        if (mp_ctx.f_less.state == SW2_ON) {
            if ((mp_ctx.f_less_lock == 0) && (mp_ctx.variator_step_target < (MP_VARIATOR_STEP_P - 1))) {
                // Increase step count.
                mp_ctx.variator_step_target++;
            }
            mp_ctx.f_less_lock = 1;
        }
        else {
            mp_ctx.f_less_lock = 0;
        }
        // MP.FR
        if (mp_ctx.f_fast.state == SW2_ON) {
            if (mp_ctx.f_fast_on == 0) {
                // Set target to maximum.
                mp_ctx.variator_step_target = MP_VARIATOR_STEP_MIN;
            }
            mp_ctx.f_fast_on = 1;
        }
        else {
            if (mp_ctx.f_fast_on != 0) {
                // Set target to current.
                mp_ctx.variator_step_target = scu_ctx.variator_step;
            }
            mp_ctx.f_fast_on = 0;
        }
        // MP.TR
        if (mp_ctx.transition.state == SW2_ON) {
            if ((mp_ctx.transition_lock == 0) && (scu_ctx.motors_coupling == MP_MOTORS_COUPLING_SERIES) && (scu_ctx.variator_step == MP_VARIATOR_STEP_MAX_SERIES_COUPLING)) {
                // Increase step count.
                mp_ctx.variator_step_target++;
            }
            mp_ctx.transition_lock = 1;
        }
        else {
            mp_ctx.transition_lock = 0;
        }
    }
    else {
        mp_ctx.variator_step_target = MP_VARIATOR_STEP_0;
    }
    // Shunt management.
    if ((scu_ctx.variator_step == MP_VARIATOR_STEP_MAX_SERIES_COUPLING) || (scu_ctx.variator_step == MP_VARIATOR_STEP_MAX)) {
        GPIO_write(&GPIO_MP_SH_ENABLE, 1);
    }
    else {
        GPIO_write(&GPIO_MP_SH_ENABLE, 0);
    }
    
    // Synchronize variator on target.
    _MP_synchronize();
}
