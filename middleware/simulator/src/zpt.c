/*
 * zpt.c
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludo
 */

#include "zpt.h"

#include "adc.h"
#include "gpio.h"
#include "mapping.h"
#include "scu.h"
#include "scu_commands.h"
#include "sgdu.h"
#include "sw4.h"
#include "stdint.h"

/*** ZPT local structures ***/

/*******************************************************************/
typedef enum {
    ZPT_STATE_0,
    ZPT_STATE_AR,
    ZPT_STATE_ARAV,
    ZPT_STATE_AV
} ZPT_state_t;

/*******************************************************************/
typedef struct {
    SW4_context_t sw4;
    ZPT_state_t state;
} ZPT_context_t;

/*** ZPT external global variables ***/

extern SCU_context_t scu_ctx;

/*** ZPT local global variables ***/

static ZPT_context_t zpt_ctx;

/*** ZPT functions ***/

/*******************************************************************/
void ZPT_init(void) {
    // Init GPIOs.
    SW4_init(&zpt_ctx.sw4, &GPIO_ZPT, 500, (uint32_t*) &(scu_ctx.adc_data[ADC_DATA_INDEX_ZPT]));
    zpt_ctx.state = ZPT_STATE_0;
    GPIO_configure(&GPIO_VLG, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
    GPIO_write(&GPIO_VLG, 1);
    // Init global context.
    scu_ctx.zpt_raised = 0;
}

/*******************************************************************/
void ZPT_process(void) {
    // Update selector state.
    SW4_update_state(&zpt_ctx.sw4);
    // Perform state machine.
    switch (zpt_ctx.state) {
    case ZPT_STATE_0:
        if (scu_ctx.bl_unlocked != 0) {
            switch (zpt_ctx.sw4.state) {
            case SW4_P0:
                // Nothing to do.
                break;
            case SW4_P1:
                // Rise back pantograph.
                SGDU_write(SCU_OUT_ZPT_REAR_UP);
                zpt_ctx.state = ZPT_STATE_AR;
                scu_ctx.zpt_raised = 1;
                GPIO_write(&GPIO_VLG, 0);
                break;
            case SW4_P2:
                // Rise both pantograph.
                SGDU_write(SCU_OUT_ZPT_REAR_UP);
                SGDU_write(SCU_OUT_ZPT_FRONT_UP);
                zpt_ctx.state = ZPT_STATE_ARAV;
                scu_ctx.zpt_raised = 1;
                GPIO_write(&GPIO_VLG, 0);
                break;
            case SW4_P3:
                // Rise front pantograph.
                SGDU_write(SCU_OUT_ZPT_FRONT_UP);
                zpt_ctx.state = ZPT_STATE_AV;
                scu_ctx.zpt_raised = 1;
                GPIO_write(&GPIO_VLG, 0);
                break;
            default:
                break;
            }
        }
        break;
    case ZPT_STATE_AR:
        if (scu_ctx.bl_unlocked != 0) {
            switch (zpt_ctx.sw4.state) {
            case SW4_P0:
                // Lower back pantograph.
                SGDU_write(SCU_OUT_ZPT_REAR_DOWN);
                zpt_ctx.state = ZPT_STATE_0;
                scu_ctx.zpt_raised = 0;
                GPIO_write(&GPIO_VLG, 1);
                break;
            case SW4_P1:
                // Nothing to do.
                break;
            case SW4_P2:
                // Rise front pantograph.
                SGDU_write(SCU_OUT_ZPT_FRONT_UP);
                zpt_ctx.state = ZPT_STATE_ARAV;
                scu_ctx.zpt_raised = 1;
                GPIO_write(&GPIO_VLG, 0);
                break;
            case SW4_P3:
                // Lower back and raise front pantograph.
                SGDU_write(SCU_OUT_ZPT_REAR_DOWN);
                SGDU_write(SCU_OUT_ZPT_FRONT_UP);
                zpt_ctx.state = ZPT_STATE_AV;
                scu_ctx.zpt_raised = 1;
                GPIO_write(&GPIO_VLG, 0);
                break;
            }
        }
        else {
            // Disable ZPT.
            SGDU_write(SCU_OUT_ZPT_REAR_DOWN);
            zpt_ctx.state = ZPT_STATE_0;
            scu_ctx.zpt_raised = 0;
            GPIO_write(&GPIO_VLG, 1);
        }
        break;
    case ZPT_STATE_ARAV:
        if (scu_ctx.bl_unlocked != 0) {
            switch (zpt_ctx.sw4.state) {
            case SW4_P0:
                // Lower both pantograph.
                SGDU_write(SCU_OUT_ZPT_REAR_DOWN);
                SGDU_write(SCU_OUT_ZPT_FRONT_DOWN);
                zpt_ctx.state = ZPT_STATE_0;
                scu_ctx.zpt_raised = 0;
                GPIO_write(&GPIO_VLG, 0);
                break;
            case SW4_P1:
                // Lower back and raise front pantograph.
                SGDU_write(SCU_OUT_ZPT_FRONT_DOWN);
                zpt_ctx.state = ZPT_STATE_AR;
                scu_ctx.zpt_raised = 1;
                GPIO_write(&GPIO_VLG, 0);
                break;
            case SW4_P2:
                // Nothing to do.
                break;
            case SW4_P3:
                // Lower back pantograph.
                SGDU_write(SCU_OUT_ZPT_REAR_DOWN);
                zpt_ctx.state = ZPT_STATE_AV;
                scu_ctx.zpt_raised = 1;
                GPIO_write(&GPIO_VLG, 0);
                break;
            }
        }
        else {
            // Disable ZPT.
            SGDU_write(SCU_OUT_ZPT_REAR_DOWN);
            SGDU_write(SCU_OUT_ZPT_FRONT_DOWN);
            zpt_ctx.state = ZPT_STATE_0;
            scu_ctx.zpt_raised = 0;
            GPIO_write(&GPIO_VLG, 1);
        }
        break;
    case ZPT_STATE_AV:
        if (scu_ctx.bl_unlocked != 0) {
            switch (zpt_ctx.sw4.state) {
            case SW4_P0:
                // Lower front pantograph.
                SGDU_write(SCU_OUT_ZPT_FRONT_DOWN);
                zpt_ctx.state = ZPT_STATE_0;
                scu_ctx.zpt_raised = 0;
                GPIO_write(&GPIO_VLG, 1);
                break;
            case SW4_P1:
                // Rise back and lower front pantograph.
                SGDU_write(SCU_OUT_ZPT_REAR_UP);
                SGDU_write(SCU_OUT_ZPT_FRONT_DOWN);
                zpt_ctx.state = ZPT_STATE_AR;
                scu_ctx.zpt_raised = 1;
                GPIO_write(&GPIO_VLG, 0);
                break;
            case SW4_P2:
                // Rise back pantograph.
                SGDU_write(SCU_OUT_ZPT_REAR_UP);
                zpt_ctx.state = ZPT_STATE_ARAV;
                scu_ctx.zpt_raised = 1;
                GPIO_write(&GPIO_VLG, 0);
                break;
            case SW4_P3:
                // Nothing to do.
                break;
            }
        }
        else {
            // Disable ZPT.
            SGDU_write(SCU_OUT_ZPT_FRONT_DOWN);
            zpt_ctx.state = ZPT_STATE_0;
            scu_ctx.zpt_raised = 0;
            GPIO_write(&GPIO_VLG, 1);
        }
        break;
    default:
        // Unknown state.
        zpt_ctx.state = ZPT_STATE_0;
        break;
    }
}
