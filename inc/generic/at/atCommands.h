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

// Bytes 0 (0x0) to 100 (0x64) are reserved for coding CP pressure*10 in bars.

// ZBA.
#define AT_ZBA_ON 			0x65 // Sound only.
#define AT_ZBA_OFF			0x66 // Sound only.
// RSEC.
#define AT_RSEC_ON 			0x67 // Sound only.
#define AT_RSEC_OFF 		0x68 // Sound only.
// BL.
#define AT_BL_ON 			0x69 // Sound + Keyboard.
#define AT_BL_OFF 			0x6A // Sound + Keyboard.
// ZPT.
#define AT_ZPT_BACK_UP 		0x6B // Sound + Keyboard.
#define AT_ZPT_BACK_DOWN 	0x6C // Sound + Keyboard.
#define AT_ZPT_FRONT_UP 	0x6D // Sound + Keyboard.
#define AT_ZPT_FRONT_DOWN 	0x6E // Sound + Keyboard.
// DJ.
#define AT_ZEN_ON 			0x6F // Sound + Keyboard.
#define AT_ZEN_OFF 			0x70 // Sound + Keyboard.
#define AT_ZDJ_OPEN 		0x71 // Sound + Keyboard.
// ZCA/ZCD.
#define AT_ZCAZCD_STATE0 	0x72 // Sound only.
#define AT_ZCAZCD_STATE1 	0x73 // Sound only.
#define AT_ZCAZCD_STATE2 	0x74 // Sound only.
#define AT_ZCAZCD_STATE3 	0x75 // Sound only.
#define AT_ZCAZCD_STATE4 	0x76 // Sound only.
// CP pressure.
#define CP_SUPPLY_TIME 		125000.0 // Must correspond to the length of 'regulationMax.wav' sound (in ms).
#define CP_HYSTERESIS_LOW 	7.8 // In bars.
#define CP_HYSTERESIS_HIGH 	9.0 // In bars.
#define CP_MAXIMUM_VALUE 	9.5 // In bars.
#define CP_SUPPLY_SPEED 	(CP_HYSTERESIS_HIGH)/(CP_SUPPLY_TIME) // In bars/ms.
// FPB
#define AT_FPB_ON 			0x77 // Sound + Keyboard.
#define AT_FPB_OFF 			0x78 // Sound + Keyboard.
#define AT_FPB_APPLY 		0x79 // Sound + Keyboard.
#define AT_FPB_NEUTRAL 		0x7A // Sound + Keyboard.
#define AT_FPB_RELEASE 		0x7B // Sound + Keyboard.
// BPGD.
#define AT_BPGD 			0x7C // Sound only.
// ZVM
#define AT_ZVM_ON 			0x7D // Sound only.
#define AT_ZVM_OFF 			0x7E // Sound only.
// INV.
#define AT_INV_FORWARD 		0x7F // Sound + Keyboard.
#define AT_INV_NEUTRAL 		0x80 // Sound + Keyboard.
#define AT_INV_BACKWARD 	0x81 // Sound + Keyboard.
// MP.
#define AT_MP_T_MORE 		0x82 // Sound + Keyboard.
#define AT_MP_T_LESS 		0x83 // Sound + Keyboard.
#define AT_MP_F_MORE 		0x84 // Sound + Keyboard.
#define AT_MP_F_LESS 		0x85 // Sound + Keyboard.
#define AT_MP_0 			0x86 // Sound + Keyboard.
#define AT_MP_PR 			0x87 // Sound + Keyboard.
#define AT_MP_P 			0x88 // Sound + Keyboard.
// FD.
#define AT_FD_APPLY	 		0x89 // Sound + Keyboard.
#define AT_FD_NEUTRAL 		0x8A // Sound + Keyboard.
#define AT_FD_RELEASE 		0x8B // Sound + Keyboard.
// BPURG.
#define AT_BPURG 			0x8C // Sound + Keyboard.
// S.
#define AT_S_HIGH_TONE 		0x8D // Sound only.
#define AT_S_NEUTRAL 		0x8E // Sound only.
#define AT_S_LOW_TONE 		0x8F // Sound only.
// BPEV.
#define AT_BPEV_ON			0x90 // Keyboard only.
#define AT_BPEV_OFF			0x91 // Keyboard only.
// BP(A)SA.
#define AT_BPSA_ON			0x92 // Keyboard only.
#define AT_BPSA_OFF			0x93 // Keyboard only.
// Lights.
#define AT_ZFG_ON			0x94 // Keyboard only.
#define AT_ZFG_OFF			0x95 // Keyboard only.
#define AT_ZFD_ON			0x96 // Keyboard only.
#define AT_ZFD_OFF			0x97 // Keyboard only.
#define AT_ZPR_ON			0x98 // Keyboard only.
#define AT_ZPR_OFF			0x99 // Keyboard only.
#define AT_ZLFRG_ON			0x9A // Keyboard only.
#define AT_ZLFRG_OFF		0x9B // Keyboard only.
#define AT_ZLFRD_ON			0x9C // Keyboard only.
#define AT_ZLFRD_OFF		0x9D // Keyboard only.
// KVB buttons.
#define AT_ACSF_ON
#define AT_ACSF_OFF
#define AT_KVB_BPVAL_ON
#define AT_KVB_BPVAL_OFF
#define AT_KVB_BPMV_ON
#define AT_KVB_BPMV_OFF
#define AT_KVB_BPFC_ON
#define AT_KVB_BPFC_OFF
#define AT_KVB_BPTEST_ON
#define AT_KVB_BPTEST_OFF
#define AT_KVB_BPSF_ON
#define AT_KVB_BPSF_OFF

/*** SGKCU -> SCU ***/

// Bytes 0 (0x0) to 160 (0xA0) are reserved for coding speed in km/h.
#define AT_SPEED_MAX		0xA0

// KVB lights.
#define AT_KVB_LVAL_BLINK	0xA1
#define AT_KVB_LVAL_ON		0xA2
#define AT_KVB_LVAL_OFF		0xA3
#define AT_KVB_LMV_ON		0xA4
#define AT_KVB_LMV_OFF		0xA5
#define AT_KVB_LFC_ON		0xA6
#define AT_KVB_LFC_OFF		0xA7
#define AT_KVB_LV_ON		0xA8
#define AT_KVB_LV_OFF		0xA9
#define AT_KVB_LFU_ON		0xAA
#define AT_KVB_LFU_OFF		0xAB
#define AT_KVB_LPS_ON		0xAC
#define AT_KVB_LPS_OFF		0xAD
#define AT_KVB_LPE_ON		0xAE
#define AT_KVB_LPE_OFF		0xAF
#define AT_KVB_LSSF_BLINK	0xB0
#define AT_KVB_LSSF_ON		0xB1
#define AT_KVB_LSSF_OFF		0xB2
// KVB 7-segments displays.
#define AT_KVB_YG_OFF
#define AT_KVB_YG_PA400
#define AT_KVB_YG_UC512
#define AT_KVB_YG_888
#define AT_KVB_YG_DASH
#define AT_KVB_G_B
#define AT_KVB_Y_B
#define AT_KVB_G_P
#define AT_KVB_Y_P
#define AT_KVB_G_L
#define AT_KVB_Y_L
#define AT_KVB_G_00
#define AT_KVB_Y_00
#define AT_KVB_G_000
#define AT_KVB_Y_000

#endif /* PERIPHERALS_USART_ATCOMMANDS_H_ */
