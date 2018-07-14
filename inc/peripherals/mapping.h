/*
 * mapping.h
 *
 *  Created on: 14 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_MAPPING_H
#define PERIPHERALS_MAPPING_H

#include "adc.h"
#include "adc_reg.h"
#include "dac.h"
#include "dac_reg.h"
#include "gpio.h"
#include "gpio_reg.h"
#include "nvic.h"
#include "rcc.h"
#include "rcc_reg.h"
#include "tim.h"
#include "tim_reg.h"
#include "usart.h"
#include "usart_reg.h"

/*** USART ***/

#define USART_SGKCU_TX_GPIO			(GPIO_Periph) {GPIOD, 5, 7} // AF7 = USART2 TX.
#define USART_SGKCU_RX_GPIO			(GPIO_Periph) {GPIOD, 6, 7} // AF7 = USART2 RX.

/*** ADC ***/

#define ADC_ZPT_GPIO				(GPIO_Periph) {GPIOB, 0, 0}
#define ADC_MPINV_GPIO				(GPIO_Periph) {GPIOB, 0, 0}

/*** DAC ***/

#define DAC_MOTOR_AMP_GPIO			(GPIO_Periph) {GPIOA, 4, 0}

/*** SINGLE GPIOs ***/

// BL inputs.
#define BL_ZDV_GPIO					(GPIO_Periph) {GPIOB, 8, 0}
// KVB segments
#define KVB_ZSA_GPIO				(GPIO_Periph) {GPIOF, 14, 0}
#define KVB_ZSB_GPIO				(GPIO_Periph) {GPIOC, 2, 0}
#define KVB_ZSC_GPIO				(GPIO_Periph) {GPIOE, 9, 0}
#define KVB_ZSD_GPIO				(GPIO_Periph) {GPIOF, 13, 0}
#define KVB_ZSE_GPIO				(GPIO_Periph) {GPIOE, 11, 0}
#define KVB_ZSF_GPIO				(GPIO_Periph) {GPIOE, 13, 0}
#define KVB_ZSG_GPIO				(GPIO_Periph) {GPIOB, 1, 0}
#define KVB_ZD_GPIO					(GPIO_Periph) {GPIOA, 0, 0}
// KVB displays
#define KVB_ZJG_GPIO				(GPIO_Periph) {GPIOF, 15, 0}
#define KVB_ZJC_GPIO				(GPIO_Periph) {GPIOB, 6, 0}
#define KVB_ZJD_GPIO				(GPIO_Periph) {GPIOB, 2, 0}
#define KVB_ZVG_GPIO				(GPIO_Periph) {GPIOE, 8, 0}
#define KVB_ZVC_GPIO				(GPIO_Periph) {GPIOE, 7, 0}
#define KVB_ZVD_GPIO				(GPIO_Periph) {GPIOE, 10, 0}
// KVB lights
#define KVB_LVAL_GPIO				(GPIO_Periph) {GPIOC, 6, 3} // AF3 = TIM8_CH1.
#define KVB_LMV_GPIO				(GPIO_Periph) {GPIOH, 6, 0}
#define KVB_LFC_GPIO				(GPIO_Periph) {GPIOH, 6, 0}
#define KVB_LV_GPIO					(GPIO_Periph) {GPIOH, 6, 0}
#define KVB_LFU_GPIO				(GPIO_Periph) {GPIOH, 6, 0}
#define KVB_LPS_GPIO				(GPIO_Periph) {GPIOH, 6, 0}
#define KVB_LPE_GPIO				(GPIO_Periph) {GPIOH, 6, 0}
#define KVB_LSSF_GPIO				(GPIO_Periph) {GPIOB, 15, 0}

// Debug.
#define LED1_GPIO 					(GPIO_Periph) {GPIOA, 6, 0}
#define LED2_GPIO					(GPIO_Periph) {GPIOA, 5, 0}
#define LED3_GPIO					(GPIO_Periph) {GPIOB, 9, 0}
#define LED4_GPIO					(GPIO_Periph) {GPIOB, 8, 0}

#ifdef RCC_OUTPUT_CLOCK
// Clock output.
#define MCO1_GPIO					(GPIO_Periph) {GPIOA, 8, 0} // AF0 = MCO1.
#define MCO2_GPIO					(GPIO_Periph) {GPIOC, 9, 0} // AF0 = MCO2.
#endif

#endif /* PERIPHERALS_MAPPING_H */
