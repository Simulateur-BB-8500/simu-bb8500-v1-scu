/*
 * am.c
 *
 *  Created on: 30 dec. 2023
 *      Author: Ludo
 */

#include "am.h"

#include "dac.h"
#include "gpio.h"
#include "mapping.h"
#include "mp.h"
#include "scu.h"
#include "sgdu.h"
#include "stdint.h"

/*** AM local macros ***/

#define AM_CATENARY_VOLTAGE_MV          1500000

#define AM_MOTORS_FEM_COEFFICIENT       6000

#define AM_VARIATOR_MOHM_MAX            1000
#define AM_VARIATOR_MOHM_MIN            300

#define AM_VARIATOR_DRIVE_TABLE_SIZE    (SGDU_MP_VARIATOR_STEP_MAX + 1)
#define AM_VARIATOR_BRAKE_TABLE_SIZE    (((-1) * SGDU_MP_VARIATOR_STEP_MIN) + 1)

#define AM_METER_FULL_SCALE_A           2000
#define AM_METER_DAC_FULL_SCALE         3750

#define AM_CURRENT_SENSE_FACTOR         600
#define AM_OVERCURRENT_THRESHOLD_A      1800

/*** AM local structures ***/

/*******************************************************************/
typedef struct {
    uint32_t variator_drive_mohm[AM_VARIATOR_DRIVE_TABLE_SIZE];
    uint32_t variator_brake_mohm[AM_VARIATOR_BRAKE_TABLE_SIZE];
} AM_context_t;

/*** AM external global variables ***/

extern SCU_context_t scu_ctx;

/*** AM local global variables ***/

static AM_context_t am_ctx;

/*** AM functions ***/

/*******************************************************************/
void AM_init(void) {
    // Local variables.
    uint8_t idx = 0;
    // Init GPIO.
    GPIO_configure(&GPIO_AMCS, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_NONE);
    // Init DAC.
    DAC_init();
    // Build resistor tables.
    for (idx = 0; idx < AM_VARIATOR_DRIVE_TABLE_SIZE; idx++) {
        // Check index.
        if (idx <= MP_VARIATOR_STEP_MAX_SERIES_COUPLING) {
            // Series zone.
            am_ctx.variator_drive_mohm[idx] = AM_VARIATOR_MOHM_MAX - (((AM_VARIATOR_MOHM_MAX - AM_VARIATOR_MOHM_MIN) * idx) / (MP_VARIATOR_STEP_MAX_SERIES_COUPLING));
        }
        else {
            // Parallel zone.
            am_ctx.variator_drive_mohm[idx] = AM_VARIATOR_MOHM_MAX - (((AM_VARIATOR_MOHM_MAX - AM_VARIATOR_MOHM_MIN) * (idx - MP_VARIATOR_STEP_MAX_SERIES_COUPLING - 1)) / (SGDU_MP_VARIATOR_STEP_MAX - MP_VARIATOR_STEP_MAX_SERIES_COUPLING - 1));
        }
    }
    for (idx = 0; idx < AM_VARIATOR_BRAKE_TABLE_SIZE; idx++) {
        am_ctx.variator_brake_mohm[idx] = AM_VARIATOR_MOHM_MAX - (((AM_VARIATOR_MOHM_MAX - AM_VARIATOR_MOHM_MIN) * idx) / (AM_VARIATOR_BRAKE_TABLE_SIZE - 1));
    }
    // Init global context.
    scu_ctx.overcurrent = 0;
}

/*******************************************************************/
void AM_process(void) {
    // Local variables.
    uint32_t am_current_a = 0;
    uint32_t variator_mohm = 0;
    uint32_t motors_fem_mv = 0;
    uint32_t dac_output = 0;
    // Check variator.
    if (scu_ctx.variator_step != 0) {
        // Compute current variator resistor.
        if (scu_ctx.variator_step > 0) {
            variator_mohm = am_ctx.variator_drive_mohm[(uint8_t) scu_ctx.variator_step];
        }
        else {
            variator_mohm = am_ctx.variator_brake_mohm[(uint8_t) ((-1) * (scu_ctx.variator_step))];
        }
        // Compute motor FEM.
        motors_fem_mv = (AM_MOTORS_FEM_COEFFICIENT * scu_ctx.speed_kmh);
        // Check coupling.
        if (scu_ctx.motors_coupling == MP_MOTORS_COUPLING_SERIES) {
            motors_fem_mv <<= 1;
            variator_mohm <<= 1;
        }
        // Check FEM.
        if (motors_fem_mv < AM_CATENARY_VOLTAGE_MV) {
            am_current_a = (AM_CATENARY_VOLTAGE_MV - motors_fem_mv) / (variator_mohm);
        }
    }
    // Clamp value.
    if (am_current_a > AM_METER_FULL_SCALE_A) {
        am_current_a = AM_METER_FULL_SCALE_A;
    }
    dac_output = ((am_current_a * AM_METER_DAC_FULL_SCALE) / (AM_METER_FULL_SCALE_A));
    // Display current.
    DAC_set_output(dac_output);
    // Read effective current.
    am_current_a = ((AM_CURRENT_SENSE_FACTOR * ADC1_convert_to_mv(scu_ctx.adc_data[ADC_DATA_INDEX_AMCS])) / (1000));
    // Update overcurrent flag.
    scu_ctx.overcurrent = (am_current_a > AM_OVERCURRENT_THRESHOLD_A) ? 1 : 0;
}
