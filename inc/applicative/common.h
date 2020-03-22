/*
 * common.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef COMMON_H
#define COMMON_H

/*** COMMON structures ***/

typedef struct {
	unsigned char lsmcu_zba_closed;
	unsigned char lsmcu_bl_unlocked;
	unsigned char lsmcu_dj_closed;
	unsigned char lsmcu_dj_locked;
	unsigned char lsmcu_zpt_raised;
	unsigned char lsmcu_speed_kmh;
} LSMCU_Context;

#endif /* COMMON_H */
