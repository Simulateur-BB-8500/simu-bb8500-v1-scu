/*
 * atCommands.h
 *
 * This file is shared with SGKCU software.
 *
 *  Created on: 1 oct. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_USART_ATCOMMANDS_H_
#define PERIPHERALS_USART_ATCOMMANDS_H_

/*** USART configuration ***/

#define BAUD_RATE 9600

/*** SCU -> SGKCU ***/

// AT commands.
typedef enum {
	// Bytes 0 to 100 are reserved for coding CP pressure*10 in bars.
	AT_ZBA_ON = 100, 	// Sound only.
	AT_ZBA_OFF, 		// Sound only.
	AT_RSEC_ON, 		// Sound only.
	AT_RSEC_OFF, 		// Sound only.
	AT_BL_ON, 			// Sound + Keyboard.
	AT_BL_OFF,			// Sound + Keyboard.
	AT_ZPT_BACK_UP,		// Sound + Keyboard.
	AT_ZPT_BACK_DOWN,	// Sound + Keyboard.
	AT_ZPT_FRONT_UP,	// Sound + Keyboard.
	AT_ZPT_FRONT_DOWN,	// Sound + Keyboard.
	AT_ZEN_ON,			// Sound + Keyboard.
	AT_ZEN_OFF,			// Sound + Keyboard.
	AT_ZDJ_OPEN,		// Sound + Keyboard.
	AT_ZCAZCD_STATE0,	// Sound only.
	AT_ZCAZCD_STATE1,	// Sound only.
	AT_ZCAZCD_STATE2,	// Sound only.
	AT_ZCAZCD_STATE3,	// Sound only.
	AT_ZCAZCD_STATE4,	// Sound only.
	AT_FPB_ON,			// Sound + Keyboard.
	AT_FPB_OFF,			// Sound + Keyboard.
	AT_FPB_APPLY,		// Sound + Keyboard.
	AT_FPB_NEUTRAL,		// Sound + Keyboard.
	AT_FPB_RELEASE,		// Sound + Keyboard.
	AT_BPGD,			// Sound only.
	AT_ZVM_ON,			// Sound only.
	AT_ZVM_OFF,			// Sound only.
	AT_INV_FORWARD,		// Sound + Keyboard.
	AT_INV_NEUTRAL,		// Sound + Keyboard.
	AT_INV_BACKWARD,	// Sound + Keyboard.
	AT_MP_0,			// Sound + Keyboard.
	AT_MP_T_MORE,		// Sound + Keyboard.
	AT_MP_T_LESS,		// Sound + Keyboard.
	AT_MP_F_MORE,		// Sound + Keyboard.
	AT_MP_F_LESS,		// Sound + Keyboard.
	AT_MP_PR,			// Sound + Keyboard.
	AT_MP_P,			// Sound + Keyboard.
	AT_FD_APPLY,		// Sound + Keyboard.
	AT_FD_NEUTRAL,		// Sound + Keyboard.
	AT_FD_RELEASE,		// Sound + Keyboard.
	AT_BPURG,			// Sound + Keyboard.
	AT_S_HIGH_TONE,		// Sound only.
	AT_S_LOW_TONE,		// Sound only.
	AT_S_NEUTRAL,		// Sound only.
	AT_BPEV_ON,			// Keyboard only.
	AT_BPEV_OFF,		// Keyboard only.
	AT_BPSA_ON,			// Keyboard only.
	AT_BPSA_OFF,		// Keyboard only.
	AT_ZFG_ON,			// Keyboard only.
	AT_ZFG_OFF,			// Keyboard only.
	AT_ZFD_ON,			// Keyboard only.
	AT_ZFD_OFF,			// Keyboard only.
	AT_ZPR_ON,			// Keyboard only.
	AT_ZPR_OFF,			// Keyboard only.
	AT_ZLFRG_ON,		// Keyboard only.
	AT_ZLFRG_OFF,		// Keyboard only.
	AT_ZLFRD_ON,		// Keyboard only.
	AT_ZLFRD_OFF,		// Keyboard only.
	AT_ACSF_ON,			// KVB only.
	AT_ACSF_OFF,		// KVB only.
	AT_KVB_BPVAL_ON,	// KVB only.
	AT_KVB_BPVAL_OFF,	// KVB only.
	AT_KVB_BPMV_ON,		// KVB only.
	AT_KVB_BPMV_OFF,	// KVB only.
	AT_KVB_BPFC_ON,		// KVB only.
	AT_KVB_BPFC_OFF,	// KVB only.
	AT_KVB_BPTEST_ON,	// KVB only.
	AT_KVB_BPTEST_OFF,	// KVB only.
	AT_KVB_BPSF_ON,		// KVB only.
	AT_KVB_BPSF_OFF		// KVB only.
} SCUtoSGKCU;

// CP pressure.
#define CP_SUPPLY_TIME 		125000.0 // Must correspond to the length of 'regulationMax.wav' sound (in ms).
#define CP_HYSTERESIS_LOW 	7.8 // In bars.
#define CP_HYSTERESIS_HIGH 	9.0 // In bars.
#define CP_MAXIMUM_VALUE 	9.5 // In bars.
#define CP_SUPPLY_SPEED 	(CP_HYSTERESIS_HIGH)/(CP_SUPPLY_TIME) // In bars/ms.

/*** SGKCU -> SCU ***/

typedef enum {
	// Bytes 0 to 160 are reserved for coding speed in km/h.
	// KVB lights.
	AT_KVB_LVAL_BLINK = 161,
	AT_KVB_LVAL_ON,
	AT_KVB_LVAL_OFF,
	AT_KVB_LMV_ON,
	AT_KVB_LMV_OFF,
	AT_KVB_LFC_ON,
	AT_KVB_LFC_OFF,
	AT_KVB_LV_ON,
	AT_KVB_LV_OFF,
	AT_KVB_LFU_ON,
	AT_KVB_LFU_OFF,
	AT_KVB_LPS_ON,
	AT_KVB_LPS_OFF,
	AT_KVB_LSSF_BLINK,
	AT_KVB_LSSF_ON,
	AT_KVB_LSSF_OFF,
	// KVB 7-segments displays.
	AT_KVB_YG_OFF,
	AT_KVB_YG_PA400,
	AT_KVB_YG_UC512,
	AT_KVB_YG_888,
	AT_KVB_YG_DASH,
	AT_KVB_G_B,
	AT_KVB_Y_B,
	AT_KVB_G_P,
	AT_KVB_Y_P,
	AT_KVB_G_L,
	AT_KVB_Y_L,
	AT_KVB_G_00,
	AT_KVB_Y_00,
	AT_KVB_G_000,
	AT_KVB_Y_000,
} SGKCUtoSCU;

// Maximum speed in km/h.
#define AT_SPEED_MAX	160

#endif /* PERIPHERALS_USART_ATCOMMANDS_H_ */
