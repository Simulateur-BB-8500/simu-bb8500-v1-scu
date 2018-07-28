/*
 * mapping.h
 *
 *  Created on: 14 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_MAPPING_H
#define PERIPHERALS_MAPPING_H

#include "gpio.h"
#include "gpio_reg.h"

/*** Nucleo-F746ZG mapping ***/

/* Serial link to LSSGKCU */

#define USART2_TX_GPIO			(GPIO_Periph) {GPIOD, 5, 7} // AF7 = USART2 TX.
#define USART2_RX_GPIO			(GPIO_Periph) {GPIOD, 6, 7} // AF7 = USART2 RX.

/* ZPT */

#define ZPT_GPIO				(GPIO_Periph) {GPIOA, 0, 0} // ADC1_IN0.

/* PBL2 */

#define PBL2_GPIO				(GPIO_Periph) {GPIOA, 1, 0} // ADC1_IN1.

/* FPB */

#define FPB_GPIO				(GPIO_Periph) {GPIOA, 2, 0} // ADC1_IN2.

/* AM */

#define AMCS_GPIO				(GPIO_Periph) {GPIOA, 3, 0} // GPIO or ADC1_IN3.
#define AM_GPIO					(GPIO_Periph) {GPIOA, 4, 0} // DAC1_OUT1.

/* FD */

#define FD_GPIO					(GPIO_Periph) {GPIOA, 5, 0} // ADC1_IN5.

/* MPINV */

#define MPINV_GPIO				(GPIO_Periph) {GPIOA, 6, 0} // ADC1_IN6.

/* S */

#define S_GPIO					(GPIO_Periph) {GPIOA, 7, 0} // ADC1_IN7.

/* ZBA */

#define ZBA_GPIO				(GPIO_Periph) {GPIOA, 0, 0}

/* BL */

#define BL_ZDV_GPIO				(GPIO_Periph) {GPIOA, 9, 0}
#define BL_ZDJ_GPIO				(GPIO_Periph) {GPIOA, 10, 0}
#define BL_ZEN_GPIO				(GPIO_Periph) {GPIOA, 11, 0}
#define BL_ZCA_GPIO				(GPIO_Periph) {GPIOA, 12, 0}
#define BL_ZCD_GPIO				(GPIO_Periph) {GPIOA, 15, 0}
#define BL_ZVM_GPIO				(GPIO_Periph) {GPIOB, 0, 0}
#define BL_ZFG_GPIO				(GPIO_Periph) {GPIOB, 1, 0}
#define BL_ZFD_GPIO				(GPIO_Periph) {GPIOB, 2, 0}
#define BL_ZPR_GPIO				(GPIO_Periph) {GPIOB, 4, 0}
#define BL_ZVA_GPIO				(GPIO_Periph) {GPIOB, 5, 0}

/* KVB */

// Buttons.
#define KVB_BPVAL_GPIO			(GPIO_Periph) {GPIOB, 6, 0}
#define KVB_BPMV_GPIO			(GPIO_Periph) {GPIOB, 7, 0}
#define KVB_BPFC_GPIO			(GPIO_Periph) {GPIOB, 8, 0}
#define KVB_BPAT_GPIO			(GPIO_Periph) {GPIOB, 9, 0}
#define KVB_BPSF_GPIO			(GPIO_Periph) {GPIOB, 10, 0}
#define KVB_ACSF_GPIO			(GPIO_Periph) {GPIOB, 11, 0}
// Lights.
#define KVB_LVAL_GPIO			(GPIO_Periph) {GPIOC, 6, 3} // AF3 = TIM8_CH1.
#define KVB_LMV_GPIO			(GPIO_Periph) {GPIOB, 12, 0}
#define KVB_LFC_GPIO			(GPIO_Periph) {GPIOB, 13, 0}
#define KVB_LV_GPIO				(GPIO_Periph) {GPIOB, 14, 0}
#define KVB_LFU_GPIO			(GPIO_Periph) {GPIOB, 15, 0}
#define KVB_LPS_GPIO			(GPIO_Periph) {GPIOC, 0, 0}
#define KVB_LPE_GPIO			(GPIO_Periph) {GPIOC, 1, 0}
#define KVB_LSSF_GPIO			(GPIO_Periph) {GPIOC, 2, 0}
// Segments.
#define KVB_ZSA_GPIO			(GPIO_Periph) {GPIOG, 0, 0}
#define KVB_ZSB_GPIO			(GPIO_Periph) {GPIOG, 1, 0}
#define KVB_ZSC_GPIO			(GPIO_Periph) {GPIOG, 2, 0}
#define KVB_ZSD_GPIO			(GPIO_Periph) {GPIOG, 3, 0}
#define KVB_ZSE_GPIO			(GPIO_Periph) {GPIOG, 4, 0}
#define KVB_ZSF_GPIO			(GPIO_Periph) {GPIOG, 5, 0}
#define KVB_ZSG_GPIO			(GPIO_Periph) {GPIOG, 6, 0}
#define KVB_ZD_GPIO				(GPIO_Periph) {GPIOG, 7, 0}
// Displays.
#define KVB_ZJG_GPIO			(GPIO_Periph) {GPIOG, 8, 0}
#define KVB_ZJC_GPIO			(GPIO_Periph) {GPIOG, 9, 0}
#define KVB_ZJD_GPIO			(GPIO_Periph) {GPIOG, 10, 0}
#define KVB_ZVG_GPIO			(GPIO_Periph) {GPIOG, 11, 0}
#define KVB_ZVC_GPIO			(GPIO_Periph) {GPIOG, 12, 0}
#define KVB_ZVD_GPIO			(GPIO_Periph) {GPIOG, 13, 0}

/* TCH */

#define TCH_A_INH_GPIO			(GPIO_Periph) {GPIOF, 10, 0}
#define TCH_B_INH_GPIO			(GPIO_Periph) {GPIOF, 11, 0}
#define TCH_C_INH_GPIO			(GPIO_Periph) {GPIOF, 12, 0}
#define TCH_A_PWM_GPIO			(GPIO_Periph) {GPIOC, 7, 0} // Optional AF3 = TIM8_CH2.
#define TCH_B_PWM_GPIO			(GPIO_Periph) {GPIOC, 8, 0} // Optional AF3 = TIM8_CH3.
#define TCH_C_PWM_GPIO			(GPIO_Periph) {GPIOC, 9, 0} // Optional AF3 = TIM8_CH4.

/* Debug */

#define LED1_GPIO 				(GPIO_Periph) {GPIOA, 6, 0}
#define LED2_GPIO				(GPIO_Periph) {GPIOA, 5, 0}
#define LED3_GPIO				(GPIO_Periph) {GPIOB, 9, 0}
#define LED4_GPIO				(GPIO_Periph) {GPIOB, 8, 0}

/* Clock output */

#ifdef RCC_OUTPUT_CLOCK
#define MCO1_GPIO				(GPIO_Periph) {GPIOA, 8, 0} // AF0 = MCO1.
#define MCO2_GPIO				(GPIO_Periph) {GPIOC, 9, 0} // AF0 = MCO2.
#endif

#endif /* PERIPHERALS_MAPPING_H */
