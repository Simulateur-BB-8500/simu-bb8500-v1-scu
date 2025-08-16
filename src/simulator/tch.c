/*
 * tch.c
 *
 *  Created on: 15 jul. 2018
 *      Author: Ludo
 */

#include "tch.h"

#include "mapping.h"
#include "scu.h"
#include "tim.h"
#include "stdint.h"

/*** TCH local macros ***/

// Speed under which the Tachro is off (not accurate enough).
#define TCH_SPEED_MIN_KMH   5

/*** TCH local structures ***/

/*******************************************************************/
typedef enum {
    TCH_STATE_OFF,
    TCH_STATE_STEP1,
    TCH_STATE_STEP2,
    TCH_STATE_STEP3,
    TCH_STATE_STEP4,
    TCH_STATE_STEP5,
    TCH_STATE_STEP6,
} TCH_state_t;

/*** TCH external global variables ***/

extern SCU_context_t scu_ctx;

/*** TCH local global variables ***/

// step_delay_us[v] = delay between each step in us required to display v km/h.
static const uint32_t tch_step_delay_us[TCH_SPEED_MAX_KMH + 1] = {
    500000, 500000, 500000, 500000, 500000, 314278, 269123, 235314, 209051, 188062, 170904,
    156614, 144530, 134177, 125208, 117363, 110443, 104293, 98793, 93843, 89366,
    85296, 81581, 78177, 75044, 72154, 69477, 66993, 64679, 62520, 60501,
    58608, 56830, 55156, 53579, 52089, 50679, 49344, 48078, 46875, 45730,
    44640, 43601, 42609, 41662, 40755, 39887, 39056, 38258, 37492, 36757,
    36049, 35368, 34713, 34081, 33472, 32885, 32317, 31769, 31239, 30727,
    30231, 29751, 29286, 28835, 28398, 27974, 27562, 27162, 26774, 26397,
    26030, 25673, 25326, 24988, 24659, 24339, 24027, 23722, 23426, 23136,
    22854, 22579, 22310, 22047, 21791, 21540, 21295, 21056, 20822, 20593,
    20369, 20150, 19935, 19725, 19520, 19318, 19121, 18928, 18739, 18553,
    18371, 18193, 18018, 17846, 17678, 17512, 17350, 17191, 17035, 16881,
    16730, 16582, 16437, 16294, 16153, 16015, 15879, 15746, 15614, 15485,
    15358, 15234, 15111, 14990, 14871, 14754, 14638, 14525, 14413, 14303,
    14195, 14088, 13983, 13879, 13777, 13676, 13577, 13480, 13383, 13288,
    13195, 13102, 13011, 12922, 12833, 12746, 12660, 12575, 12491, 12408,
    12326, 12246, 12166, 12088, 12010, 11934, 11858, 11784, 11710, 11637
};
static TCH_state_t tch_state;

/*** TCH local functions ***/

/*******************************************************************/
#define _TCH_inhibit_all_drivers(void) { \
    GPIO_write(&GPIO_TCH_INH_A, 0); \
    GPIO_write(&GPIO_TCH_INH_B, 0); \
    GPIO_write(&GPIO_TCH_INH_C, 0); \
}

/*** TCH functions ***/

/*******************************************************************/
void TCH_init(void) {
    // Init INH outputs.
    GPIO_configure(&GPIO_TCH_INH_A, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
    GPIO_configure(&GPIO_TCH_INH_B, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
    GPIO_configure(&GPIO_TCH_INH_C, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
    // Init PWM outputs
    GPIO_configure(&GPIO_TCH_PWM_A, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
    GPIO_configure(&GPIO_TCH_PWM_B, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
    GPIO_configure(&GPIO_TCH_PWM_C, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
    // Init context.
    _TCH_inhibit_all_drivers();
    tch_state = TCH_STATE_OFF;
    // Init global context.
    scu_ctx.speed_kmh = 0;
    // Init phase control timer.
    TIM5_init();
}

/*******************************************************************/
void TCH_process(void) {
    // Perform state machine.
    switch (tch_state) {
    case TCH_STATE_OFF:
        // All outputs off.
        _TCH_inhibit_all_drivers();
        GPIO_write(&GPIO_TCH_PWM_A, 0);
        GPIO_write(&GPIO_TCH_PWM_B, 0);
        GPIO_write(&GPIO_TCH_PWM_C, 0);
        // State evolution.
        if (scu_ctx.speed_kmh >= TCH_SPEED_MIN_KMH) {
            // Start timer and go to first step.
            TIM5_start();
            TIM5_set_delay_microseconds(tch_step_delay_us[scu_ctx.speed_kmh]);
            TIM5_clear_uif_flag();
            tch_state = TCH_STATE_STEP1;
        }
        break;
    case TCH_STATE_STEP1:
        // Set voltage on A-B.
        GPIO_write(&GPIO_TCH_INH_C, 0);
        GPIO_write(&GPIO_TCH_PWM_A, 1);
        GPIO_write(&GPIO_TCH_PWM_B, 0);
        GPIO_write(&GPIO_TCH_PWM_C, 0);
        GPIO_write(&GPIO_TCH_INH_A, 1);
        GPIO_write(&GPIO_TCH_INH_B, 1);
        // State evolution.
        if (TIM5_get_uif_flag() != 0) {
            // Clear flag, update delay and go to next step.
            TIM5_set_delay_microseconds(tch_step_delay_us[scu_ctx.speed_kmh]);
            TIM5_clear_uif_flag();
            tch_state = TCH_STATE_STEP2;
        }
        break;
    case TCH_STATE_STEP2:
        // Set voltage on A-C.
        GPIO_write(&GPIO_TCH_INH_B, 0);
        GPIO_write(&GPIO_TCH_PWM_A, 1);
        GPIO_write(&GPIO_TCH_PWM_B, 0);
        GPIO_write(&GPIO_TCH_PWM_C, 0);
        GPIO_write(&GPIO_TCH_INH_A, 1);
        GPIO_write(&GPIO_TCH_INH_C, 1);
        // State evolution.
        if (TIM5_get_uif_flag() != 0) {
            // Clear flag, update delay and go to next step.
            TIM5_set_delay_microseconds(tch_step_delay_us[scu_ctx.speed_kmh]);
            TIM5_clear_uif_flag();
            tch_state = TCH_STATE_STEP3;
        }
        break;
    case TCH_STATE_STEP3:
        // Set voltage on B-C.
        GPIO_write(&GPIO_TCH_INH_A, 0);
        GPIO_write(&GPIO_TCH_PWM_A, 0);
        GPIO_write(&GPIO_TCH_PWM_B, 1);
        GPIO_write(&GPIO_TCH_PWM_C, 0);
        GPIO_write(&GPIO_TCH_INH_B, 1);
        GPIO_write(&GPIO_TCH_INH_C, 1);
        // State evolution.
        if (TIM5_get_uif_flag() != 0) {
            // Clear flag, update delay and go to next step.
            TIM5_set_delay_microseconds(tch_step_delay_us[scu_ctx.speed_kmh]);
            TIM5_clear_uif_flag();
            tch_state = TCH_STATE_STEP4;
        }
        break;
    case TCH_STATE_STEP4:
        // Set voltage on B-A.
        GPIO_write(&GPIO_TCH_INH_C, 0);
        GPIO_write(&GPIO_TCH_PWM_A, 0);
        GPIO_write(&GPIO_TCH_PWM_B, 1);
        GPIO_write(&GPIO_TCH_PWM_C, 0);
        GPIO_write(&GPIO_TCH_INH_A, 1);
        GPIO_write(&GPIO_TCH_INH_B, 1);
        // State evolution.
        if (TIM5_get_uif_flag() != 0) {
            // Clear flag, update delay and go to next step.
            TIM5_set_delay_microseconds(tch_step_delay_us[scu_ctx.speed_kmh]);
            TIM5_clear_uif_flag();
            tch_state = TCH_STATE_STEP5;
        }
        break;
    case TCH_STATE_STEP5:
        // Set voltage on C-A.
        GPIO_write(&GPIO_TCH_INH_B, 0);
        GPIO_write(&GPIO_TCH_PWM_A, 0);
        GPIO_write(&GPIO_TCH_PWM_B, 0);
        GPIO_write(&GPIO_TCH_PWM_C, 1);
        GPIO_write(&GPIO_TCH_INH_A, 1);
        GPIO_write(&GPIO_TCH_INH_C, 1);
        // State evolution.
        if (TIM5_get_uif_flag() != 0) {
            // Clear flag, update delay and go to next step.
            TIM5_set_delay_microseconds(tch_step_delay_us[scu_ctx.speed_kmh]);
            TIM5_clear_uif_flag();
            tch_state = TCH_STATE_STEP6;
        }
        break;
    case TCH_STATE_STEP6:
        // Set voltage on C-B.
        GPIO_write(&GPIO_TCH_INH_A, 0);
        GPIO_write(&GPIO_TCH_PWM_A, 0);
        GPIO_write(&GPIO_TCH_PWM_B, 0);
        GPIO_write(&GPIO_TCH_PWM_C, 1);
        GPIO_write(&GPIO_TCH_INH_B, 1);
        GPIO_write(&GPIO_TCH_INH_C, 1);
        // Check delay.
        if (TIM5_get_uif_flag() != 0) {
            // Clear flag, update delay and go to next step.
            TIM5_set_delay_microseconds(tch_step_delay_us[scu_ctx.speed_kmh]);
            TIM5_clear_uif_flag();
            tch_state = TCH_STATE_STEP1;
        }
        break;
    default:
        // Unknown state.
        break;
    }
    // Check speed.
    if ((scu_ctx.speed_kmh < TCH_SPEED_MIN_KMH) || (scu_ctx.bl_unlocked == 0)) {
        // Stop timer and switch Tachro off.
        TIM5_stop();
        tch_state = TCH_STATE_OFF;
    }
}
