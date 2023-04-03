/*
 * mapping.h
 *
 *  Created on: 14 sept. 2017
 *      Author: Ludo
 */

#ifndef __MAPPING_H__
#define __MAPPING_H__

#include "gpio.h"
#include "gpio_reg.h"

/*** MCU pins mapping ***/

// Serial link to LSSGKCU.
static const GPIO GPIO_USART1_TX =					(GPIO) {GPIOA, 0, 9, 7};	// AF7 = USART1 TX.
static const GPIO GPIO_USART1_RX =					(GPIO) {GPIOA, 0, 10, 7}; 	// AF7 = USART1 RX.
// ZBA.
#ifdef RCC_OUTPUT_CLOCK
static const GPIO GPIO_MCO1 =						(GPIO) {GPIOA, 0, 8, 0}; 	// AF0 = MCO1.
#else
static const GPIO GPIO_ZBA =						(GPIO) {GPIOA, 0, 8, 0};
#endif
// BL.
static const GPIO GPIO_BL_ZDV =						(GPIO) {GPIOA, 0, 11, 0};
static const GPIO GPIO_BL_ZDJ =						(GPIO) {GPIOA, 0, 12, 0};
static const GPIO GPIO_BL_ZEN =						(GPIO) {GPIOA, 0, 15, 0};
static const GPIO GPIO_BL_ZCA =						(GPIO) {GPIOB, 1, 2, 0};
static const GPIO GPIO_BL_ZCD =						(GPIO) {GPIOB, 1, 4, 0};
static const GPIO GPIO_BL_ZVM =						(GPIO) {GPIOB, 1, 5, 0};
static const GPIO GPIO_BL_ZFG =						(GPIO) {GPIOB, 1, 6, 0};
static const GPIO GPIO_BL_ZFD =						(GPIO) {GPIOB, 1, 8, 0};
static const GPIO GPIO_BL_ZPR =						(GPIO) {GPIOB, 1, 9, 0};
static const GPIO GPIO_BL_ZVA =						(GPIO) {GPIOB, 1, 10, 0};
// ZPT.
static const GPIO GPIO_ZPT =						(GPIO) {GPIOA, 0, 0, 0};	// ADC1_IN0.
// KVB displays.
static const GPIO GPIO_KVB_ZJG =					(GPIO) {GPIOG, 6, 8, 0};
static const GPIO GPIO_KVB_ZJC =					(GPIO) {GPIOG, 6, 9, 0};
static const GPIO GPIO_KVB_ZJD =					(GPIO) {GPIOG, 6, 10, 0};
static const GPIO GPIO_KVB_ZVG =					(GPIO) {GPIOG, 6, 11, 0};
static const GPIO GPIO_KVB_ZVC =					(GPIO) {GPIOG, 6, 12, 0};
static const GPIO GPIO_KVB_ZVD =					(GPIO) {GPIOG, 6, 13, 0};
// KVB segments.
static const GPIO GPIO_KVB_ZSA =					(GPIO) {GPIOG, 6, 0, 0};
static const GPIO GPIO_KVB_ZSB =					(GPIO) {GPIOG, 6, 1, 0};
static const GPIO GPIO_KVB_ZSC =					(GPIO) {GPIOG, 6, 2, 0};
static const GPIO GPIO_KVB_ZSD =					(GPIO) {GPIOG, 6, 3, 0};
static const GPIO GPIO_KVB_ZSE =					(GPIO) {GPIOG, 6, 4, 0};
static const GPIO GPIO_KVB_ZSF =					(GPIO) {GPIOG, 6, 5, 0};
static const GPIO GPIO_KVB_ZSG =					(GPIO) {GPIOG, 6, 6, 0};
// KVB inputs.
static const GPIO GPIO_KVB_BPVAL =					(GPIO) {GPIOF, 5, 3, 0};
static const GPIO GPIO_KVB_BPMV	=					(GPIO) {GPIOF, 5, 4, 0};
static const GPIO GPIO_KVB_BPFC =					(GPIO) {GPIOF, 5, 5, 0};
static const GPIO GPIO_KVB_BPAT =					(GPIO) {GPIOF, 5, 6, 0};
static const GPIO GPIO_KVB_BPSF =					(GPIO) {GPIOF, 5, 7, 0};
static const GPIO GPIO_KVB_ACSF =					(GPIO) {GPIOF, 5, 8, 0};
// KVB lights.
static const GPIO GPIO_KVB_LVAL =					(GPIO) {GPIOC, 2, 6, 3}; 	// AF3 = TIM8_CH1.
static const GPIO GPIO_KVB_LMV =					(GPIO) {GPIOF, 5, 9, 0};
static const GPIO GPIO_KVB_LFC =					(GPIO) {GPIOF, 5, 10, 0};
static const GPIO GPIO_KVB_LV =						(GPIO) {GPIOF, 5, 11, 0};
static const GPIO GPIO_KVB_LFU =					(GPIO) {GPIOF, 5, 12, 0};
static const GPIO GPIO_KVB_LPS =					(GPIO) {GPIOF, 5, 13, 0};
static const GPIO GPIO_KVB_LPE =					(GPIO) {GPIOF, 5, 14, 0};
static const GPIO GPIO_KVB_LSSF =					(GPIO) {GPIOF, 5, 15, 0};
// S.
static const GPIO GPIO_WHISTLE =					(GPIO) {GPIOA, 0, 7, 0}; 	// ADC1_IN7.
// PBL2.
static const GPIO GPIO_PBL2 =						(GPIO) {GPIOA, 0, 1, 0}; 	// ADC1_IN1.
// BPGD.
static const GPIO GPIO_BPGD =						(GPIO) {GPIOB, 1, 11, 0};
// FPB.
static const GPIO GPIO_FPB =						(GPIO) {GPIOA, 0, 2, 0}; 	// ADC1_IN2.
// FD.
static const GPIO GPIO_FD =							(GPIO) {GPIOA, 0, 5, 0};	// ADC1_IN5.
// BPURG.
static const GPIO GPIO_BPURG =						(GPIO) {GPIOB, 1, 12, 0};
// ZSUR / LSUR.
static const GPIO GPIO_ZSUR =						(GPIO) {GPIOB, 1, 13, 0};
static const GPIO GPIO_LSUR =						(GPIO) {GPIOB, 1, 15, 0};
// BP(A)SA.
static const GPIO GPIO_BPSA =						(GPIO) {GPIOC, 2, 0, 0};
static const GPIO GPIO_BPASA =						(GPIO) {GPIOC, 2, 1, 0};
// INV.
static const GPIO GPIO_MPINV =						(GPIO) {GPIOA, 0, 6, 0}; 	// ADC1_IN6.
// MP.
static const GPIO GPIO_MP_0 =						(GPIO) {GPIOD, 3, 0, 0};
static const GPIO GPIO_MP_TM =						(GPIO) {GPIOD, 3, 1, 0};
static const GPIO GPIO_MP_TL =						(GPIO) {GPIOD, 3, 2, 0};
static const GPIO GPIO_MP_TF =						(GPIO) {GPIOD, 3, 3, 0};
static const GPIO GPIO_MP_P =						(GPIO) {GPIOD, 3, 4, 0};
static const GPIO GPIO_MP_FM =						(GPIO) {GPIOD, 3, 5, 0};
static const GPIO GPIO_MP_FL =						(GPIO) {GPIOD, 3, 6, 0};
static const GPIO GPIO_MP_FF =						(GPIO) {GPIOD, 3, 7, 0};
static const GPIO GPIO_MP_TR =						(GPIO) {GPIOD, 3, 8, 0};
static const GPIO GPIO_MP_VA =						(GPIO) {GPIOD, 3, 9, 0};
static const GPIO GPIO_MP_SH_ENABLE = 				(GPIO) {GPIOG, 6, 14, 0};
// VACMA.
static const GPIO GPIO_VACMA_RELEASED_ALARM =		(GPIO) {GPIOD, 3, 10, 0};
static const GPIO GPIO_VACMA_HOLD_ALARM =			(GPIO) {GPIOD, 3, 11, 0};
// DEP.
static const GPIO GPIO_BELL =						(GPIO) {GPIOC, 2, 2, 0};
// Motor amperemeters.
static const GPIO GPIO_AMCS =						(GPIO) {GPIOA, 0, 3, 0};	// ADC1_IN3 or GPIO input.
static const GPIO GPIO_AM =							(GPIO) {GPIOA, 0, 4, 0}; 	// DAC1_OUT1.
// VLG.
static const GPIO GPIO_VLG =						(GPIO) {GPIOC, 2, 3, 0};
// IL.
static const GPIO GPIO_LSDJ =						(GPIO) {GPIOD, 3, 12, 0};
static const GPIO GPIO_LSGR =						(GPIO) {GPIOD, 3, 13, 0};
static const GPIO GPIO_LSS =						(GPIO) {GPIOD, 3, 14, 0};
static const GPIO GPIO_LSCB =						(GPIO) {GPIOD, 3, 15, 0};
static const GPIO GPIO_LSP =						(GPIO) {GPIOE, 4, 0, 0};
static const GPIO GPIO_LSPAT =						(GPIO) {GPIOE, 4, 1, 0};
static const GPIO GPIO_LSBA =						(GPIO) {GPIOE, 4, 2, 0};
static const GPIO GPIO_LSPI =						(GPIO) {GPIOE, 4, 3, 0};
static const GPIO GPIO_LSRH =						(GPIO) {GPIOE, 4, 4, 0};
// Manometers.
static const GPIO GPIO_MANOMETER_POWER_ENABLE =		(GPIO) {GPIOE, 4, 5, 0};
static const GPIO GPIO_MCP_1 =						(GPIO) {GPIOE, 4, 6, 0};
static const GPIO GPIO_MCP_2 =						(GPIO) {GPIOE, 4, 7, 0};
static const GPIO GPIO_MCP_SD =						(GPIO) {GPIOC, 2, 13, 0};
static const GPIO GPIO_MRE_1 =						(GPIO) {GPIOE, 4, 8, 0};
static const GPIO GPIO_MRE_2 =						(GPIO) {GPIOE, 4, 9, 0};
static const GPIO GPIO_MRE_SD =						(GPIO) {GPIOF, 5, 0, 0};
static const GPIO GPIO_MCG_1 =						(GPIO) {GPIOE, 4, 10, 0};
static const GPIO GPIO_MCG_2 =						(GPIO) {GPIOE, 4, 11, 0};
static const GPIO GPIO_MCG_SD =						(GPIO) {GPIOF, 5, 1, 0};
static const GPIO GPIO_MCF1_1 =						(GPIO) {GPIOE, 4, 12, 0};
static const GPIO GPIO_MCF1_2 =						(GPIO) {GPIOE, 4, 13, 0};
static const GPIO GPIO_MCF1_SD =					(GPIO) {GPIOG, 6, 7, 0};
static const GPIO GPIO_MCF2_1 =						(GPIO) {GPIOE, 4, 14, 0};
static const GPIO GPIO_MCF2_2 =						(GPIO) {GPIOE, 4, 15, 0};
static const GPIO GPIO_MCF2_SD =					(GPIO) {GPIOG, 6, 15, 0};
// BPEV.
static const GPIO GPIO_BPEV =						(GPIO) {GPIOC, 3, 4, 0};
// ZLFR.
static const GPIO GPIO_ZLFR =						(GPIO) {GPIOB, 1, 1, 0}; 	// ADC1_IN9.
// ZLCT.
static const GPIO GPIO_ZLCT =						(GPIO) {GPIOC, 3, 5, 0};
// Tachro.
static const GPIO GPIO_TCH_PWM_A =					(GPIO) {GPIOC, 2, 7, 0}; 	// AF3 = TIM8_CH2 or GPIO out.
static const GPIO GPIO_TCH_PWM_B =					(GPIO) {GPIOC, 2, 8, 0};	// AF3 = TIM8_CH3 or GPIO out.
#ifdef RCC_OUTPUT_CLOCK
static const GPIO GPIO_MCO2 =						(GPIO) {GPIOC, 2, 9, 0};	// AF0 = MCO2.
#else
static const GPIO GPIO_TCH_PWM_C =					(GPIO) {GPIOC, 2, 9, 0}; 	// AF3 = TIM8_CH4 or GPIO out.
#endif
static const GPIO GPIO_TCH_INH_A =					(GPIO) {GPIOC, 2, 10, 0};
static const GPIO GPIO_TCH_INH_B =					(GPIO) {GPIOC, 2, 11, 0};
static const GPIO GPIO_TCH_INH_C =					(GPIO) {GPIOC, 2, 12, 0};
// RS.
static const GPIO GPIO_RS =							(GPIO) {GPIOF, 5, 2, 0};
// LEDs.
static const GPIO GPIO_LED_RED =					(GPIO) {GPIOB, 1, 14, 0};
static const GPIO GPIO_LED_GREEN = 					(GPIO) {GPIOB, 1, 0, 0};
static const GPIO GPIO_LED_BLUE =					(GPIO) {GPIOB, 1, 7, 0};

#endif /* __MAPPING_H__ */
