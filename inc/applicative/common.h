/*
 * common.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef COMMON_H
#define COMMON_H

#include "mano.h"

/*** COMMON structures ***/

typedef struct {
	unsigned char lsmcu_zba_closed;
	unsigned char lsmcu_bl_unlocked;
	unsigned char lsmcu_dj_closed;
	unsigned char lsmcu_dj_locked;
	unsigned char lsmcu_zpt_raised;
	unsigned char lsmcu_speed_kmh;
	MANO_Context lsmcu_mano_cp;
	MANO_Context lsmcu_mano_re;
	MANO_Context lsmcu_mano_cg;
	MANO_Context lsmcu_mano_cf1;
	MANO_Context lsmcu_mano_cf2;
	unsigned char lsmcu_compressor_on;
} LSMCU_Context;

/*** COMMMON global variables ***/

LSMCU_Context lsmcu_ctx;

#endif /* COMMON_H */
