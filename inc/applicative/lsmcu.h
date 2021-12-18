/*
 * common.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef LSMCU_H
#define LSMCU_H

#include "mano.h"

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
	MANO_Context* lsmcu_mano_cp;
	MANO_Context* lsmcu_mano_re;
	MANO_Context* lsmcu_mano_cg;
	MANO_Context* lsmcu_mano_cf1;
	MANO_Context* lsmcu_mano_cf2;
} LSMCU_Context;

#endif /* LSMCU_H */
