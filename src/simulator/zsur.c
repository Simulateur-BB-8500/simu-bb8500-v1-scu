/*
 * zsur.c
 *
 *  Created on: 27 dec. 2021
 *      Author: Ludo
 */

#include "zsur.h"

#include "gpio.h"
#include "manometer.h"
#include "mapping.h"
#include "scu.h"
#include "sw2.h"
#include "stdint.h"

/*** ZSUR local macros ***/

#define ZSUR_CG_PRESSURE_MIN_MBAR           4900
#define ZSUR_CG_RE_SPEED_MBAR_PER_SECOND    300

#define LSUR_CG_PRESSURE_THRESHOLD_MBAR     5300

/*** ZSUR local structures ***/

/*******************************************************************/
typedef struct {
    SW2_context_t sw2;
    SW2_state_t previous_state;
} ZSUR_context_t;

/*** ZSUR external global variables ***/

extern SCU_context_t scu_ctx;

/*** ZSUR local global variables ***/

static ZSUR_context_t zsur_ctx;

/*** ZSUR functions ***/

/*******************************************************************/
void ZSUR_init(void) {
    // Init GPIOs.
    SW2_init(&zsur_ctx.sw2, &GPIO_ZSUR, 0, 100); // ZSUR active low.
    GPIO_configure(&GPIO_LSUR, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
}

/*******************************************************************/
void ZSUR_process(void) {
    // Update switch state.
    SW2_update_state(&zsur_ctx.sw2);
    // Check state.
    if (zsur_ctx.sw2.state == SW2_ON) {
        // Send command on change.
        if ((zsur_ctx.previous_state != SW2_ON) && (MANOMETER_get_pressure(scu_ctx.manometer_cg) > ZSUR_CG_PRESSURE_MIN_MBAR) && (MANOMETER_get_pressure(scu_ctx.manometer_cp) > ((scu_ctx.manometer_cg)->pressure_limit_mbar))) {
            MANOMETER_set_pressure(scu_ctx.manometer_cg, ((scu_ctx.manometer_cg)->pressure_limit_mbar), ZSUR_CG_RE_SPEED_MBAR_PER_SECOND);
            MANOMETER_set_pressure(scu_ctx.manometer_re, ((scu_ctx.manometer_re)->pressure_limit_mbar), ZSUR_CG_RE_SPEED_MBAR_PER_SECOND);
        }
    }
    // Update previous state.
    zsur_ctx.previous_state = zsur_ctx.sw2.state;
    // Manage LSUR indicator.
    if (MANOMETER_get_pressure(scu_ctx.manometer_cg) > LSUR_CG_PRESSURE_THRESHOLD_MBAR) {
        GPIO_write(&GPIO_LSUR, 1);
    }
    else {
        GPIO_write(&GPIO_LSUR, 0);
    }
}
