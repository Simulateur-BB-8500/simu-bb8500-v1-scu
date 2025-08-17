/*
 * pbl2.c
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#include "pbl2.h"

#include "adc.h"
#include "gpio.h"
#include "mapping.h"
#include "scu.h"
#include "scu_commands.h"
#include "sgdu.h"
#include "sw4.h"
#include "stdint.h"

/*** PBL2 local macros ***/

#define PBL2_ON_CG_PRESSURE_MBAR            3500
#define PBL2_ON_CG_SPEED_MBAR_PER_SECOND    800

#define PBL2_ON_RE_PRESSURE_MBAR            3500
#define PBL2_ON_RE_SPEED_MBAR_PER_SECOND    800

/*** PBL2 external global variables ***/

extern SCU_context_t scu_ctx;

/*** PBL2 local global variables ***/

static SW4_context_t pbl2_sw4;

/*** PBL2 functions ***/

/*******************************************************************/
void PBL2_init(void) {
    // Init GPIO.
    SW4_init(&pbl2_sw4, &GPIO_PBL2, 500, (uint32_t*) &(scu_ctx.adc_data[ADC_DATA_INDEX_PBL2]));
    // Init global context.
    scu_ctx.pbl2_on = 0;
}

/*******************************************************************/
void PBL2_process(void) {
    // Update current state.
    SW4_update_state(&pbl2_sw4);
    // Perform actions according to state.
    switch (pbl2_sw4.state) {
    case SW4_P0:
        // Retrait.
        if (scu_ctx.pbl2_on != 0) {
            // Send command on change.
            SGDU_write(SCU_OUT_PBL2_OFF);
            // Empty CG and RE.
            MANOMETER_set_pressure(scu_ctx.manometer_cg, 0, PBL2_ON_CG_SPEED_MBAR_PER_SECOND);
            MANOMETER_set_pressure(scu_ctx.manometer_re, 0, PBL2_ON_RE_SPEED_MBAR_PER_SECOND);
            // Update global context.
            scu_ctx.pbl2_on = 0;
        }
        break;
    case SW4_P1:
        // Isolement.
        break;
    case SW4_P2:
        // Service.
        if ((scu_ctx.pbl2_on == 0) && (MANOMETER_get_pressure(scu_ctx.manometer_cp) > ((scu_ctx.manometer_cg)->pressure_limit_mbar)) && (MANOMETER_get_pressure(scu_ctx.manometer_cp) > ((scu_ctx.manometer_re)->pressure_limit_mbar))) {
            // Send command on change.
            SGDU_write(SCU_OUT_PBL2_ON);
            // Start CG and RE manometers.
            MANOMETER_set_pressure(scu_ctx.manometer_cg, PBL2_ON_CG_PRESSURE_MBAR, PBL2_ON_CG_SPEED_MBAR_PER_SECOND);
            MANOMETER_set_pressure(scu_ctx.manometer_re, PBL2_ON_RE_PRESSURE_MBAR, PBL2_ON_RE_SPEED_MBAR_PER_SECOND);
            // Update global context.
            scu_ctx.pbl2_on = 1;
        }
        break;
    case SW4_P3:
        // Neutre.
        break;
    default:
        // Unknown state.
        break;
    }
}

