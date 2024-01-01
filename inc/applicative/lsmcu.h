/*
 * common.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef LSMCU_H
#define LSMCU_H

#include "adc.h"
#include "manometer.h"
#include "mp.h"
#include "stdint.h"

/*** LSMCU structure ***/

/*!******************************************************************
 * \struct LSMCU_context_t
 * \brief Global simulator context.
 *******************************************************************/
typedef struct {
	uint8_t zba_closed;
	uint8_t bl_unlocked;
	uint8_t zpt_raised;
	uint8_t dj_closed;
	uint8_t dj_locked;
	uint8_t speed_kmh;
	uint8_t speed_limit_kmh;
	int8_t variator_step;
	uint8_t overcurrent;
	uint8_t lsrh_blink_request;
	MP_motors_coupling_t motors_coupling;
	uint8_t pbl2_on;
	uint8_t emergency;
	volatile uint32_t adc_data[ADC_DATA_INDEX_LAST];
	MANOMETER_context_t* manometer_cp;
	MANOMETER_context_t* manometer_re;
	MANOMETER_context_t* manometer_cg;
	MANOMETER_context_t* manometer_cf1;
	MANOMETER_context_t* manometer_cf2;
} LSMCU_context_t;

#endif /* LSMCU_H */
