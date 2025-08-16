/*
 * mapping.c
 *
 *  Created on: 01 may 2024
 *      Author: Ludo
 */

#include "mapping.h"

#include "gpio.h"
#include "gpio_reg.h"

/*** MAPPING global variables ***/

// Serial link to LS-AGIU.
const GPIO_pin_t GPIO_USART1_TX = (GPIO_pin_t ) { GPIOA, 0, 9, 7 }; // AF7 = USART1 TX.
const GPIO_pin_t GPIO_USART1_RX = (GPIO_pin_t ) { GPIOA, 0, 10, 7 }; // AF7 = USART1 RX.
// ZBA.
#ifdef RCC_OUTPUT_CLOCK
const GPIO_pin_t GPIO_MCO1 = (GPIO_pin_t) {GPIOA, 0, 8, 0}; // AF0 = MCO1.
#else
const GPIO_pin_t GPIO_ZBA = (GPIO_pin_t ) { GPIOA, 0, 8, 0 };
#endif
// BL.
const GPIO_pin_t GPIO_BL_ZDV = (GPIO_pin_t ) { GPIOA, 0, 11, 0 };
const GPIO_pin_t GPIO_BL_ZDJ = (GPIO_pin_t ) { GPIOA, 0, 12, 0 };
const GPIO_pin_t GPIO_BL_ZEN = (GPIO_pin_t ) { GPIOA, 0, 15, 0 };
const GPIO_pin_t GPIO_BL_ZCA = (GPIO_pin_t ) { GPIOB, 1, 2, 0 };
const GPIO_pin_t GPIO_BL_ZCD = (GPIO_pin_t ) { GPIOB, 1, 4, 0 };
const GPIO_pin_t GPIO_BL_ZVM = (GPIO_pin_t ) { GPIOB, 1, 5, 0 };
const GPIO_pin_t GPIO_BL_ZFG = (GPIO_pin_t ) { GPIOB, 1, 6, 0 };
const GPIO_pin_t GPIO_BL_ZFD = (GPIO_pin_t ) { GPIOB, 1, 8, 0 };
const GPIO_pin_t GPIO_BL_ZPR = (GPIO_pin_t ) { GPIOB, 1, 9, 0 };
const GPIO_pin_t GPIO_BL_ZVA = (GPIO_pin_t ) { GPIOB, 1, 10, 0 };
// ZPT.
const GPIO_pin_t GPIO_ZPT = (GPIO_pin_t ) { GPIOA, 0, 0, 0 }; // ADC1_IN0.
// KVB displays.
const GPIO_pin_t GPIO_KVB_ZJG = (GPIO_pin_t ) { GPIOG, 6, 8, 0 };
const GPIO_pin_t GPIO_KVB_ZJC = (GPIO_pin_t ) { GPIOG, 6, 9, 0 };
const GPIO_pin_t GPIO_KVB_ZJD = (GPIO_pin_t ) { GPIOG, 6, 10, 0 };
const GPIO_pin_t GPIO_KVB_ZVG = (GPIO_pin_t ) { GPIOG, 6, 11, 0 };
const GPIO_pin_t GPIO_KVB_ZVC = (GPIO_pin_t ) { GPIOG, 6, 12, 0 };
const GPIO_pin_t GPIO_KVB_ZVD = (GPIO_pin_t ) { GPIOG, 6, 13, 0 };
// KVB segments.
const GPIO_pin_t GPIO_KVB_ZSA = (GPIO_pin_t ) { GPIOG, 6, 0, 0 };
const GPIO_pin_t GPIO_KVB_ZSB = (GPIO_pin_t ) { GPIOG, 6, 1, 0 };
const GPIO_pin_t GPIO_KVB_ZSC = (GPIO_pin_t ) { GPIOG, 6, 2, 0 };
const GPIO_pin_t GPIO_KVB_ZSD = (GPIO_pin_t ) { GPIOG, 6, 3, 0 };
const GPIO_pin_t GPIO_KVB_ZSE = (GPIO_pin_t ) { GPIOG, 6, 4, 0 };
const GPIO_pin_t GPIO_KVB_ZSF = (GPIO_pin_t ) { GPIOG, 6, 5, 0 };
const GPIO_pin_t GPIO_KVB_ZSG = (GPIO_pin_t ) { GPIOG, 6, 6, 0 };
// KVB inputs.
const GPIO_pin_t GPIO_KVB_BPVAL = (GPIO_pin_t ) { GPIOF, 5, 3, 0 };
const GPIO_pin_t GPIO_KVB_BPMV = (GPIO_pin_t ) { GPIOF, 5, 4, 0 };
const GPIO_pin_t GPIO_KVB_BPFC = (GPIO_pin_t ) { GPIOF, 5, 5, 0 };
const GPIO_pin_t GPIO_KVB_BPAT = (GPIO_pin_t ) { GPIOF, 5, 6, 0 };
const GPIO_pin_t GPIO_KVB_BPSF = (GPIO_pin_t ) { GPIOF, 5, 7, 0 };
const GPIO_pin_t GPIO_KVB_ACSF = (GPIO_pin_t ) { GPIOF, 5, 8, 0 };
// KVB lights.
const GPIO_pin_t GPIO_KVB_LVAL = (GPIO_pin_t ) { GPIOC, 2, 6, 3 }; // AF3 = TIM8_CH1.
const GPIO_pin_t GPIO_KVB_LMV = (GPIO_pin_t ) { GPIOF, 5, 9, 0 };
const GPIO_pin_t GPIO_KVB_LFC = (GPIO_pin_t ) { GPIOF, 5, 10, 0 };
const GPIO_pin_t GPIO_KVB_LV = (GPIO_pin_t ) { GPIOF, 5, 11, 0 };
const GPIO_pin_t GPIO_KVB_LFU = (GPIO_pin_t ) { GPIOF, 5, 12, 0 };
const GPIO_pin_t GPIO_KVB_LPS = (GPIO_pin_t ) { GPIOF, 5, 13, 0 };
const GPIO_pin_t GPIO_KVB_LPE = (GPIO_pin_t ) { GPIOF, 5, 14, 0 };
const GPIO_pin_t GPIO_KVB_LSSF = (GPIO_pin_t ) { GPIOF, 5, 15, 0 };
// KVB sounds.
const GPIO_pin_t GPIO_KVB_BIP = (GPIO_pin_t ) { GPIOB, 1, 0, 0 };
// S.
const GPIO_pin_t GPIO_WHISTLE = (GPIO_pin_t ) { GPIOA, 0, 7, 0 }; // ADC1_IN7.
// PBL2.
const GPIO_pin_t GPIO_PBL2 = (GPIO_pin_t ) { GPIOA, 0, 1, 0 }; // ADC1_IN1.
// BPGD.
const GPIO_pin_t GPIO_BPGD = (GPIO_pin_t ) { GPIOB, 1, 11, 0 };
// FPB.
const GPIO_pin_t GPIO_FPB = (GPIO_pin_t ) { GPIOA, 0, 2, 0 }; // ADC1_IN2.
// FD.
const GPIO_pin_t GPIO_FD = (GPIO_pin_t ) { GPIOA, 0, 5, 0 }; // ADC1_IN5.
// BPURG.
const GPIO_pin_t GPIO_BPURG = (GPIO_pin_t ) { GPIOB, 1, 12, 0 };
// ZSUR / LSUR.
const GPIO_pin_t GPIO_ZSUR = (GPIO_pin_t ) { GPIOB, 1, 13, 0 };
const GPIO_pin_t GPIO_LSUR = (GPIO_pin_t ) { GPIOB, 1, 15, 0 };
// BP(A)SA.
const GPIO_pin_t GPIO_BPSA = (GPIO_pin_t ) { GPIOC, 2, 0, 0 };
const GPIO_pin_t GPIO_BPASA = (GPIO_pin_t ) { GPIOC, 2, 1, 0 };
// INV.
const GPIO_pin_t GPIO_MPINV = (GPIO_pin_t ) { GPIOA, 0, 6, 0 }; // ADC1_IN6.
// MP.
const GPIO_pin_t GPIO_MP_0 = (GPIO_pin_t ) { GPIOD, 3, 0, 0 };
const GPIO_pin_t GPIO_MP_TM = (GPIO_pin_t ) { GPIOD, 3, 1, 0 };
const GPIO_pin_t GPIO_MP_TL = (GPIO_pin_t ) { GPIOD, 3, 2, 0 };
const GPIO_pin_t GPIO_MP_TF = (GPIO_pin_t ) { GPIOD, 3, 3, 0 };
const GPIO_pin_t GPIO_MP_P = (GPIO_pin_t ) { GPIOD, 3, 4, 0 };
const GPIO_pin_t GPIO_MP_FM = (GPIO_pin_t ) { GPIOD, 3, 5, 0 };
const GPIO_pin_t GPIO_MP_FL = (GPIO_pin_t ) { GPIOD, 3, 6, 0 };
const GPIO_pin_t GPIO_MP_FF = (GPIO_pin_t ) { GPIOD, 3, 7, 0 };
const GPIO_pin_t GPIO_MP_TR = (GPIO_pin_t ) { GPIOD, 3, 8, 0 };
const GPIO_pin_t GPIO_MP_VA = (GPIO_pin_t ) { GPIOD, 3, 9, 0 };
const GPIO_pin_t GPIO_MP_SH_ENABLE = (GPIO_pin_t ) { GPIOG, 6, 14, 0 };
// VACMA.
const GPIO_pin_t GPIO_VACMA_RELEASED_ALARM = (GPIO_pin_t ) { GPIOD, 3, 10, 0 };
const GPIO_pin_t GPIO_VACMA_HOLD_ALARM = (GPIO_pin_t ) { GPIOD, 3, 11, 0 };
// DEP.
const GPIO_pin_t GPIO_BELL = (GPIO_pin_t ) { GPIOC, 2, 2, 0 };
// Motor ammeter.
const GPIO_pin_t GPIO_AMCS = (GPIO_pin_t ) { GPIOA, 0, 3, 0 }; // ADC1_IN3 or GPIO input.
const GPIO_pin_t GPIO_AM = (GPIO_pin_t ) { GPIOA, 0, 4, 0 }; // DAC1_OUT1.
// VLG.
const GPIO_pin_t GPIO_VLG = (GPIO_pin_t ) { GPIOC, 2, 3, 0 };
// IL.
const GPIO_pin_t GPIO_LSDJ = (GPIO_pin_t ) { GPIOD, 3, 12, 0 };
const GPIO_pin_t GPIO_LSGR = (GPIO_pin_t ) { GPIOD, 3, 13, 0 };
const GPIO_pin_t GPIO_LSS = (GPIO_pin_t ) { GPIOD, 3, 14, 0 };
const GPIO_pin_t GPIO_LSCB = (GPIO_pin_t ) { GPIOD, 3, 15, 0 };
const GPIO_pin_t GPIO_LSP = (GPIO_pin_t ) { GPIOE, 4, 0, 0 };
const GPIO_pin_t GPIO_LSPAT = (GPIO_pin_t ) { GPIOE, 4, 1, 0 };
const GPIO_pin_t GPIO_LSBA = (GPIO_pin_t ) { GPIOE, 4, 2, 0 };
const GPIO_pin_t GPIO_LSPI = (GPIO_pin_t ) { GPIOE, 4, 3, 0 };
const GPIO_pin_t GPIO_LSRH = (GPIO_pin_t ) { GPIOE, 4, 4, 0 };
// Manometers.
const GPIO_pin_t GPIO_MANOMETER_POWER_ENABLE = (GPIO_pin_t ) { GPIOE, 4, 5, 0 };
const GPIO_pin_t GPIO_MCP_1 = (GPIO_pin_t ) { GPIOE, 4, 6, 0 };
const GPIO_pin_t GPIO_MCP_2 = (GPIO_pin_t ) { GPIOE, 4, 7, 0 };
const GPIO_pin_t GPIO_MCP_SD = (GPIO_pin_t ) { GPIOC, 2, 13, 0 };
const GPIO_pin_t GPIO_MRE_1 = (GPIO_pin_t ) { GPIOE, 4, 8, 0 };
const GPIO_pin_t GPIO_MRE_2 = (GPIO_pin_t ) { GPIOE, 4, 9, 0 };
const GPIO_pin_t GPIO_MRE_SD = (GPIO_pin_t ) { GPIOF, 5, 0, 0 };
const GPIO_pin_t GPIO_MCG_1 = (GPIO_pin_t ) { GPIOE, 4, 10, 0 };
const GPIO_pin_t GPIO_MCG_2 = (GPIO_pin_t ) { GPIOE, 4, 11, 0 };
const GPIO_pin_t GPIO_MCG_SD = (GPIO_pin_t ) { GPIOF, 5, 1, 0 };
const GPIO_pin_t GPIO_MCF1_1 = (GPIO_pin_t ) { GPIOE, 4, 12, 0 };
const GPIO_pin_t GPIO_MCF1_2 = (GPIO_pin_t ) { GPIOE, 4, 13, 0 };
const GPIO_pin_t GPIO_MCF1_SD = (GPIO_pin_t ) { GPIOG, 6, 7, 0 };
const GPIO_pin_t GPIO_MCF2_1 = (GPIO_pin_t ) { GPIOE, 4, 14, 0 };
const GPIO_pin_t GPIO_MCF2_2 = (GPIO_pin_t ) { GPIOE, 4, 15, 0 };
const GPIO_pin_t GPIO_MCF2_SD = (GPIO_pin_t ) { GPIOG, 6, 15, 0 };
// BPEV.
const GPIO_pin_t GPIO_BPEV = (GPIO_pin_t ) { GPIOC, 2, 4, 0 };
// ZLFR.
const GPIO_pin_t GPIO_ZLFR = (GPIO_pin_t ) { GPIOB, 1, 1, 0 }; // ADC1_IN9.
// ZLCT.
const GPIO_pin_t GPIO_ZLCT = (GPIO_pin_t ) { GPIOC, 2, 5, 0 };
// Tachro.
const GPIO_pin_t GPIO_TCH_PWM_A = (GPIO_pin_t ) { GPIOC, 2, 7, 0 }; // AF3 = TIM8_CH2 or GPIO out.
const GPIO_pin_t GPIO_TCH_PWM_B = (GPIO_pin_t ) { GPIOC, 2, 8, 0 }; // AF3 = TIM8_CH3 or GPIO out.
#ifdef RCC_OUTPUT_CLOCK
const GPIO_pin_t GPIO_MCO2 = (GPIO_pin_t) {GPIOC, 2, 9, 0}; // AF0 = MCO2.
#else
const GPIO_pin_t GPIO_TCH_PWM_C = (GPIO_pin_t ) { GPIOC, 2, 9, 0 }; // AF3 = TIM8_CH4 or GPIO out.
#endif
const GPIO_pin_t GPIO_TCH_INH_A = (GPIO_pin_t ) { GPIOC, 2, 10, 0 };
const GPIO_pin_t GPIO_TCH_INH_B = (GPIO_pin_t ) { GPIOC, 2, 11, 0 };
const GPIO_pin_t GPIO_TCH_INH_C = (GPIO_pin_t ) { GPIOC, 2, 12, 0 };
// RS.
const GPIO_pin_t GPIO_RS = (GPIO_pin_t ) { GPIOF, 5, 2, 0 };
// LEDs.
const GPIO_pin_t GPIO_LED_RED = (GPIO_pin_t ) { GPIOB, 1, 14, 0 };
const GPIO_pin_t GPIO_LED_BLUE = (GPIO_pin_t ) { GPIOB, 1, 7, 0 };
