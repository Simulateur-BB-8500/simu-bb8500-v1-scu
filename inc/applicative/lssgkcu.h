/*
 * lssgkcu.h
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludovic
 */

#ifndef LSSGKCU_IN_H
#define LSSGKCU_IN_H

#include "tch.h"

/*** LSMCU -> LSSGKCU ***/

// Commands list.
typedef enum {
	// Bytes 0 to 100 are reserved for coding CP pressure*10 in bars.
	LSSGKCU_IN_ZBA_ON = 101, 		// Sound only.
	LSSGKCU_IN_ZBA_OFF, 			// Sound only.
	LSSGKCU_IN_RSEC_ON, 			// Sound only.
	LSSGKCU_IN_RSEC_OFF, 			// Sound only.
	LSSGKCU_IN_ZDV_ON, 				// Sound + Keyboard.
	LSSGKCU_IN_ZDV_OFF,				// Sound + Keyboard.
	LSSGKCU_IN_ZPT_BACK_UP,			// Sound + Keyboard.
	LSSGKCU_IN_ZPT_BACK_DOWN,		// Sound + Keyboard.
	LSSGKCU_IN_ZPT_FRONT_UP,		// Sound + Keyboard.
	LSSGKCU_IN_ZPT_FRONT_DOWN,		// Sound + Keyboard.
	LSSGKCU_IN_ZDJ_OFF,				// Sound + Keyboard.
	LSSGKCU_IN_ZEN_ON,				// Sound + Keyboard.
	LSSGKCU_IN_ZCAZCD_STATE0,		// Sound only.
	LSSGKCU_IN_ZCAZCD_STATE1,		// Sound only.
	LSSGKCU_IN_ZCAZCD_STATE2,		// Sound only.
	LSSGKCU_IN_ZCAZCD_STATE3,		// Sound only.
	LSSGKCU_IN_ZCAZCD_STATE4,		// Sound only.
	LSSGKCU_IN_FPB_ON,				// Sound + Keyboard.
	LSSGKCU_IN_FPB_OFF,				// Sound + Keyboard.
	LSSGKCU_IN_FPB_APPLY,			// Sound + Keyboard.
	LSSGKCU_IN_FPB_NEUTRAL,			// Sound + Keyboard.
	LSSGKCU_IN_FPB_RELEASE,			// Sound + Keyboard.
	LSSGKCU_IN_BPGD,				// Sound only.
	LSSGKCU_IN_ZVM_ON,				// Sound only.
	LSSGKCU_IN_ZVM_OFF,				// Sound only.
	LSSGKCU_IN_INV_FORWARD,			// Sound + Keyboard.
	LSSGKCU_IN_INV_NEUTRAL,			// Sound + Keyboard.
	LSSGKCU_IN_INV_BACKWARD,		// Sound + Keyboard.
	LSSGKCU_IN_MP_0,				// Sound + Keyboard.
	LSSGKCU_IN_MP_T_MORE,			// Sound + Keyboard.
	LSSGKCU_IN_MP_T_LESS,			// Sound + Keyboard.
	LSSGKCU_IN_MP_F_MORE,			// Sound + Keyboard.
	LSSGKCU_IN_MP_F_LESS,			// Sound + Keyboard.
	LSSGKCU_IN_MP_PR,				// Sound + Keyboard.
	LSSGKCU_IN_MP_P,				// Sound + Keyboard.
	LSSGKCU_IN_FD_APPLY,			// Sound + Keyboard.
	LSSGKCU_IN_FD_NEUTRAL,			// Sound + Keyboard.
	LSSGKCU_IN_FD_RELEASE,			// Sound + Keyboard.
	LSSGKCU_IN_BPURG,				// Sound + Keyboard.
	LSSGKCU_IN_S_HIGH_TONE,			// Sound only.
	LSSGKCU_IN_S_LOW_TONE,			// Sound only.
	LSSGKCU_IN_S_NEUTRAL,			// Sound only.
	LSSGKCU_IN_BPEV_ON,				// Keyboard only.
	LSSGKCU_IN_BPEV_OFF,			// Keyboard only.
	LSSGKCU_IN_BPSA_ON,				// Keyboard only.
	LSSGKCU_IN_BPSA_OFF,			// Keyboard only.
	LSSGKCU_IN_ZFG_ON,				// Keyboard only.
	LSSGKCU_IN_ZFG_OFF,				// Keyboard only.
	LSSGKCU_IN_ZFD_ON,				// Keyboard only.
	LSSGKCU_IN_ZFD_OFF,				// Keyboard only.
	LSSGKCU_IN_ZPR_ON,				// Keyboard only.
	LSSGKCU_IN_ZPR_OFF,				// Keyboard only.
	LSSGKCU_IN_ZLFRG_ON,			// Keyboard only.
	LSSGKCU_IN_ZLFRG_OFF,			// Keyboard only.
	LSSGKCU_IN_ZLFRD_ON,			// Keyboard only.
	LSSGKCU_IN_ZLFRD_OFF,			// Keyboard only.
	LSSGKCU_IN_ACSF_ON,				// KVB only.
	LSSGKCU_IN_ACSF_OFF,			// KVB only.
	LSSGKCU_IN_KVB_BPVAL_ON,		// KVB only.
	LSSGKCU_IN_KVB_BPVAL_OFF,		// KVB only.
	LSSGKCU_IN_KVB_BPMV_ON,			// KVB only.
	LSSGKCU_IN_KVB_BPMV_OFF,		// KVB only.
	LSSGKCU_IN_KVB_BPFC_ON,			// KVB only.
	LSSGKCU_IN_KVB_BPFC_OFF,		// KVB only.
	LSSGKCU_IN_KVB_BPTEST_ON,		// KVB only.
	LSSGKCU_IN_KVB_BPTEST_OFF,		// KVB only.
	LSSGKCU_IN_KVB_BPSF_ON,			// KVB only.
	LSSGKCU_IN_KVB_BPSF_OFF			// KVB only.
} LSMCU_To_LSSGKCU;

// CP pressure.
#define CP_SUPPLY_TIME_MS 	125000 // Must correspond to the length of 'regulationMax.wav' sound (in ms).
#define CP_HYSTERESIS_LOW 	7800 // In mbars.
#define CP_HYSTERESIS_HIGH 	9000 // In mbars.
#define CP_MAXIMUM_VALUE 	9500 // In mbars.
#define CP_SUPPLY_SPEED 	((CP_HYSTERESIS_HIGH) / (CP_SUPPLY_TIME)) // In mbars/ms.

/*** LSSGKCU -> LSMCU ***/

// Command list.
typedef enum {
	// Bytes 0 to TCH_SPEED_MAX_KMH are reserved for coding speed in km/h.
	// KVB lights.
	LSSGKCU_OUT_KVB_LVAL_BLINK = TCH_SPEED_MAX_KMH + 1,
	LSSGKCU_OUT_KVB_LVAL_ON,
	LSSGKCU_OUT_KVB_LVAL_OFF,
	LSSGKCU_OUT_KVB_LMV_ON,
	LSSGKCU_OUT_KVB_LMV_OFF,
	LSSGKCU_OUT_KVB_LFC_ON,
	LSSGKCU_OUT_KVB_LFC_OFF,
	LSSGKCU_OUT_KVB_LV_ON,
	LSSGKCU_OUT_KVB_LV_OFF,
	LSSGKCU_OUT_KVB_LFU_ON,
	LSSGKCU_OUT_KVB_LFU_OFF,
	LSSGKCU_OUT_KVB_LPS_ON,
	LSSGKCU_OUT_KVB_LPS_OFF,
	LSSGKCU_OUT_KVB_LSSF_BLINK,
	LSSGKCU_OUT_KVB_LSSF_ON,
	LSSGKCU_OUT_KVB_LSSF_OFF,
	// KVB 7-segments displays.
	LSSGKCU_OUT_KVB_YG_OFF,
	LSSGKCU_OUT_KVB_YG_PA400,
	LSSGKCU_OUT_KVB_YG_UC512,
	LSSGKCU_OUT_KVB_YG_888,
	LSSGKCU_OUT_KVB_YG_DASH,
	LSSGKCU_OUT_KVB_G_B,
	LSSGKCU_OUT_KVB_Y_B,
	LSSGKCU_OUT_KVB_G_P,
	LSSGKCU_OUT_KVB_Y_P,
	LSSGKCU_OUT_KVB_G_L,
	LSSGKCU_OUT_KVB_Y_L,
	LSSGKCU_OUT_KVB_G_00,
	LSSGKCU_OUT_KVB_Y_00,
	LSSGKCU_OUT_KVB_G_000,
	LSSGKCU_OUT_KVB_Y_000,
} LSSGKCU_IN_To_LSMCU;

/*** LSSGKCU functions ***/

void LSSGKCU_Init(void);
void LSSGKCU_FillRxBuffer(unsigned char at_cmd);
void LSSGKCU_Send(unsigned char at_cmd);
void LSSGKCU_Task(void);

#endif /* LSSGKCU_IN_H */
