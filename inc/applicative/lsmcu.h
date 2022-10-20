/*
 * common.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef LSMCU_H
#define LSMCU_H

#include "manometer.h"

/*** LSMCU structure ***/

typedef struct {
	unsigned char zba_closed;
	unsigned char bl_unlocked;
	unsigned char zpt_raised;
	unsigned char dj_closed;
	unsigned char dj_locked;
	unsigned char compressor_on;
	unsigned char speed_kmh;
	unsigned char speed_limit_kmh;
	unsigned char rheostat_0;
	unsigned char lsrh_blink_request;
	unsigned char series_traction;
	unsigned char pbl2_on;
	unsigned char urgency;
	MANOMETER_context_t* manometer_cp;
	MANOMETER_context_t* manometer_re;
	MANOMETER_context_t* manometer_cg;
	MANOMETER_context_t* manometer_cf1;
	MANOMETER_context_t* manometer_cf2;
} LSMCU_Context;

#endif /* LSMCU_H */
