/*
 * mpinv.c
 *
 *  Created on: 08 apr. 2018
 *      Author: Ludo
 */

#include "mpinv.h"

#include "adc.h"
#include "gpio.h"
#include "mapping.h"
#include "scu.h"
#include "scu_commands.h"
#include "sgdu.h"
#include "sw3.h"
#include "stdint.h"

/*** MPINV local structures ***/

/*******************************************************************/
typedef struct {
    SW3_context_t sw3;
    SW3_state_t previous_state;
} MPINV_context_t;

/*** MPINV external global variables ***/

extern SCU_context_t scu_ctx;

/*** MPINV local global variables ***/

static MPINV_context_t mpinv_ctx;

/*** MPINV functions ***/

/*******************************************************************/
void MPINV_init(void) {
    // Init GPIO.
    SW3_init(&mpinv_ctx.sw3, &GPIO_MPINV, 100, (uint32_t*) &(scu_ctx.adc_data[ADC_DATA_INDEX_MPINV]));
    mpinv_ctx.previous_state = SW3_NEUTRAL;
}

/*******************************************************************/
void MPINV_process(void) {
    // Update current state.
    SW3_update_state(&mpinv_ctx.sw3);
    // Perform actions according to state.
    switch (mpinv_ctx.sw3.state) {
    case SW3_BACK:
        if (mpinv_ctx.previous_state != SW3_BACK) {
            // Backward.
            SGDU_write(SCU_OUT_MPINV_BACKWARD);
        }
        break;
    case SW3_NEUTRAL:
        if (mpinv_ctx.previous_state != SW3_NEUTRAL) {
            // Forward.
            SGDU_write(SCU_OUT_MPINV_NEUTRAL);
        }
        break;
    case SW3_FRONT:
        if (mpinv_ctx.previous_state != SW3_FRONT) {
            // Forward.
            SGDU_write(SCU_OUT_MPINV_FORWARD);
        }
        break;
    default:
        // Unknown state.
        break;
    }
    // Update previous state.
    mpinv_ctx.previous_state = mpinv_ctx.sw3.state;
}
