/*
 * main.c
 *
 *  Created on: 5 sept. 2017
 *      Author: Ludovic
 */

#include "adc.h"
#include "adc_reg.h"
#include "at.h"
#include "bl.h"
#include "dac.h"
#include "gpio.h"
#include "kvb.h"
#include "mapping.h"
#include "mpinv.h"
#include "rcc.h"
#include "tim.h"
#include "tim_reg.h"
#include "usart.h"
#include "zpt.h"

/* MAIN FUNCTION.
 * @param: 	None.
 * @return: 0.
 */
int main(void) {

	/*** Peripherals ***/

	// Clocks and timers.
	RCC_Init();
	TIM2_Init();
	// GPIO.
	GPIO_Init();
	// DAC.
	DAC_Init();
	DAC_SetVoltageMv(0, VCC_MV/2);
	// ADC.
	ADC1_Init();
	// USART.
	USART2_Init();

	// AT manager.
	AT_Init();

	/*** Simulator ***/

	//BL_Init();

	//MPINV_Init();
	//ZPT_Init();

	KVB_Init();

	/*** Global variables initialisation ***/

	/*** Main loop ***/

	while(1) {
		//TIM2_DelayMs(1000);
		//GPIO_Toggle(LED1_GPIO);

		//USART2_SendByte(0x46, Hexadecimal);
		//AT_Routine();

		//BL_Routine();

		//ADC1_Routine(1);
		//MPINV_Routine();
		//ZPT_Routine();

		KVB_Routine(1);
	}

	return (0);
}
