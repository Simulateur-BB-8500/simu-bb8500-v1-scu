/*
 * mapping.h
 *
 *  Created on: 14 sep. 2017
 *      Author: Ludo
 */

#ifndef __MAPPING_H__
#define __MAPPING_H__

#include "gpio.h"
#include "gpio_registers.h"

/*** MCU pins mapping ***/

// Serial link to LS-AGIU.
extern const GPIO_pin_t GPIO_USART1_TX;
extern const GPIO_pin_t GPIO_USART1_RX;
// ZBA.
#ifdef RCC_OUTPUT_CLOCK
extern const GPIO_pin_t GPIO_MCO1;
#else
extern const GPIO_pin_t GPIO_ZBA;
#endif
// BL.
extern const GPIO_pin_t GPIO_BL_ZDV;
extern const GPIO_pin_t GPIO_BL_ZDJ;
extern const GPIO_pin_t GPIO_BL_ZEN;
extern const GPIO_pin_t GPIO_BL_ZCA;
extern const GPIO_pin_t GPIO_BL_ZCD;
extern const GPIO_pin_t GPIO_BL_ZVM;
extern const GPIO_pin_t GPIO_BL_ZFG;
extern const GPIO_pin_t GPIO_BL_ZFD;
extern const GPIO_pin_t GPIO_BL_ZPR;
extern const GPIO_pin_t GPIO_BL_ZVA;
// ZPT.
extern const GPIO_pin_t GPIO_ZPT;
// KVB displays.
extern const GPIO_pin_t GPIO_KVB_ZJG;
extern const GPIO_pin_t GPIO_KVB_ZJC;
extern const GPIO_pin_t GPIO_KVB_ZJD;
extern const GPIO_pin_t GPIO_KVB_ZVG;
extern const GPIO_pin_t GPIO_KVB_ZVC;
extern const GPIO_pin_t GPIO_KVB_ZVD;
// KVB segments.
extern const GPIO_pin_t GPIO_KVB_ZSA;
extern const GPIO_pin_t GPIO_KVB_ZSB;
extern const GPIO_pin_t GPIO_KVB_ZSC;
extern const GPIO_pin_t GPIO_KVB_ZSD;
extern const GPIO_pin_t GPIO_KVB_ZSE;
extern const GPIO_pin_t GPIO_KVB_ZSF;
extern const GPIO_pin_t GPIO_KVB_ZSG;
// KVB inputs.
extern const GPIO_pin_t GPIO_KVB_BPVAL;
extern const GPIO_pin_t GPIO_KVB_BPMV;
extern const GPIO_pin_t GPIO_KVB_BPFC;
extern const GPIO_pin_t GPIO_KVB_BPAT;
extern const GPIO_pin_t GPIO_KVB_BPSF;
extern const GPIO_pin_t GPIO_KVB_ACSF;
// KVB lights.
extern const GPIO_pin_t GPIO_KVB_LVAL;
extern const GPIO_pin_t GPIO_KVB_LMV;
extern const GPIO_pin_t GPIO_KVB_LFC;
extern const GPIO_pin_t GPIO_KVB_LV;
extern const GPIO_pin_t GPIO_KVB_LFU;
extern const GPIO_pin_t GPIO_KVB_LPS;
extern const GPIO_pin_t GPIO_KVB_LPE;
extern const GPIO_pin_t GPIO_KVB_LSSF;
// KVB sounds.
extern const GPIO_pin_t GPIO_KVB_BIP;
// S.
extern const GPIO_pin_t GPIO_WHISTLE;
// PBL2.
extern const GPIO_pin_t GPIO_PBL2;
// BPGD.
extern const GPIO_pin_t GPIO_BPGD;
// FPB.
extern const GPIO_pin_t GPIO_FPB;
// FD.
extern const GPIO_pin_t GPIO_FD;
// BPURG.
extern const GPIO_pin_t GPIO_BPURG;
// ZSUR / LSUR.
extern const GPIO_pin_t GPIO_ZSUR;
extern const GPIO_pin_t GPIO_LSUR;
// BP(A)SA.
extern const GPIO_pin_t GPIO_BPSA;
extern const GPIO_pin_t GPIO_BPASA;
// INV.
extern const GPIO_pin_t GPIO_MPINV;
// MP.
extern const GPIO_pin_t GPIO_MP_0;
extern const GPIO_pin_t GPIO_MP_TM;
extern const GPIO_pin_t GPIO_MP_TL;
extern const GPIO_pin_t GPIO_MP_TF;
extern const GPIO_pin_t GPIO_MP_P;
extern const GPIO_pin_t GPIO_MP_FM;
extern const GPIO_pin_t GPIO_MP_FL;
extern const GPIO_pin_t GPIO_MP_FF;
extern const GPIO_pin_t GPIO_MP_TR;
extern const GPIO_pin_t GPIO_MP_VA;
extern const GPIO_pin_t GPIO_MP_SH_ENABLE;
// VACMA.
extern const GPIO_pin_t GPIO_VACMA_RELEASED_ALARM;
extern const GPIO_pin_t GPIO_VACMA_HOLD_ALARM;
// DEP.
extern const GPIO_pin_t GPIO_BELL;
// Motor ammeter.
extern const GPIO_pin_t GPIO_AMCS;
extern const GPIO_pin_t GPIO_AM;
// VLG.
extern const GPIO_pin_t GPIO_VLG;
// IL.
extern const GPIO_pin_t GPIO_LSDJ;
extern const GPIO_pin_t GPIO_LSGR;
extern const GPIO_pin_t GPIO_LSS;
extern const GPIO_pin_t GPIO_LSCB;
extern const GPIO_pin_t GPIO_LSP;
extern const GPIO_pin_t GPIO_LSPAT;
extern const GPIO_pin_t GPIO_LSBA;
extern const GPIO_pin_t GPIO_LSPI;
extern const GPIO_pin_t GPIO_LSRH;
// Manometers.
extern const GPIO_pin_t GPIO_MANOMETER_POWER_ENABLE;
extern const GPIO_pin_t GPIO_MCP_1;
extern const GPIO_pin_t GPIO_MCP_2;
extern const GPIO_pin_t GPIO_MCP_SD;
extern const GPIO_pin_t GPIO_MRE_1;
extern const GPIO_pin_t GPIO_MRE_2;
extern const GPIO_pin_t GPIO_MRE_SD;
extern const GPIO_pin_t GPIO_MCG_1;
extern const GPIO_pin_t GPIO_MCG_2;
extern const GPIO_pin_t GPIO_MCG_SD;
extern const GPIO_pin_t GPIO_MCF1_1;
extern const GPIO_pin_t GPIO_MCF1_2;
extern const GPIO_pin_t GPIO_MCF1_SD;
extern const GPIO_pin_t GPIO_MCF2_1;
extern const GPIO_pin_t GPIO_MCF2_2;
extern const GPIO_pin_t GPIO_MCF2_SD;
// BPEV.
extern const GPIO_pin_t GPIO_BPEV;
// ZLFR.
extern const GPIO_pin_t GPIO_ZLFR;
// ZLCT.
extern const GPIO_pin_t GPIO_ZLCT;
// Tachro.
extern const GPIO_pin_t GPIO_TCH_PWM_A;
extern const GPIO_pin_t GPIO_TCH_PWM_B;
#ifdef RCC_OUTPUT_CLOCK
extern const GPIO_pin_t GPIO_MCO2;
#else
extern const GPIO_pin_t GPIO_TCH_PWM_C;
#endif
extern const GPIO_pin_t GPIO_TCH_INH_A;
extern const GPIO_pin_t GPIO_TCH_INH_B;
extern const GPIO_pin_t GPIO_TCH_INH_C;
// RS.
extern const GPIO_pin_t GPIO_RS;
// LEDs.
extern const GPIO_pin_t GPIO_LED_RED;
extern const GPIO_pin_t GPIO_LED_BLUE;

#endif /* __MAPPING_H__ */
