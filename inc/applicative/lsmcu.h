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
	unsigned char lsmcu_zba_closed;
	unsigned char lsmcu_bl_unlocked;
	unsigned char lsmcu_zpt_raised;
	unsigned char lsmcu_dj_closed;
	unsigned char lsmcu_dj_locked;
	unsigned char lsmcu_compressor_on;
	unsigned char lsmcu_speed_kmh;
	unsigned char lsmcu_rheostat_0;
	unsigned char lsmcu_lsrh_blink_request;
	unsigned char lsmcu_series_traction;
	unsigned char lsmcu_pbl2_on;
	unsigned char lsmcu_urgency;
	MANOMETER_Context* lsmcu_manometer_cp;
	MANOMETER_Context* lsmcu_manometer_re;
	MANOMETER_Context* lsmcu_manometer_cg;
	MANOMETER_Context* lsmcu_manometer_cf1;
	MANOMETER_Context* lsmcu_manometer_cf2;
} LSMCU_Context;

#endif /* LSMCU_H */
