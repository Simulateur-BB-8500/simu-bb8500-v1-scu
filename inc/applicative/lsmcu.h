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
#include "stdint.h"

/*** LSMCU structure ***/

typedef struct {
	uint8_t zba_closed;
	uint8_t bl_unlocked;
	uint8_t zpt_raised;
	uint8_t dj_closed;
	uint8_t dj_locked;
	uint8_t speed_kmh;
	uint8_t speed_limit_kmh;
	uint8_t rheostat_0;
	uint8_t lsrh_blink_request;
	uint8_t series_traction;
	uint8_t pbl2_on;
	uint8_t emergency;
	volatile uint32_t adc_data[ADC_DATA_INDEX_LAST];
	MANOMETER_context_t* manometer_cp;
	MANOMETER_context_t* manometer_re;
	MANOMETER_context_t* manometer_cg;
	MANOMETER_context_t* manometer_cf1;
	MANOMETER_context_t* manometer_cf2;
} LSMCU_Context;

#endif /* LSMCU_H */
