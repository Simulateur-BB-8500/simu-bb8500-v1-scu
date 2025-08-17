/*
 * zba.c
 *
 *  Created on: 06 feb. 2020
 *      Author: Ludo
 */

#include "zba.h"

#include "mapping.h"
#include "scu.h"
#include "scu_commands.h"
#include "sgdu.h"
#include "sw2.h"
#include "stdint.h"

/*** ZBA external global variables ***/

extern SCU_context_t scu_ctx;

/*** ZBA local global variables ***/

static SW2_context_t zba;

/*** ZBA functions ***/

/*******************************************************************/
void ZBA_init(void) {
    // Init GPIO.
    SW2_init(&zba, &GPIO_ZBA, 1, 100); // ZBA active high (+3.3V supply present).
    // Init global context.
    scu_ctx.zba_closed = 0;
}

/*******************************************************************/
void ZBA_process(void) {
    // Update ZBA state.
    SW2_update_state(&zba);
    if (zba.state == SW2_ON) {
        // Send command on change.
        if (scu_ctx.zba_closed == 0) {
            SGDU_write(SCU_OUT_ZBA_ON);
        }
        scu_ctx.zba_closed = 1;
    }
    else {
        // Send command on change.
        if (scu_ctx.zba_closed != 0) {
            SGDU_write(SCU_OUT_ZBA_OFF);
        }
        scu_ctx.zba_closed = 0;
    }
}
