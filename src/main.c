/*
 * main.c
 *
 *  Created on: 5 sept. 2017
 *      Author: Ludovic
 */

#include "adc.h"
#include "adc_reg.h"
#include "dac.h"
#include "gpio.h"
#include "kvb.h"
#include "rcc.h"
#include "tim.h"
#include "tim_reg.h"
#include "usart.h"
#include "usart_reg.h"
#include "zpt.h"

/* MAIN FUNCTION.
 * @param: None.
 * @return: 0.
 */
int main(void) {

	/*** Peripherals ***/

	// Clocks and timers.
	RCC_Init();
	TIM_InitMs();
	// GPIO.
	GPIO_Init();
	// DAC.
	DAC_Init();
	// ADC (only ADC1 is used).
	ADCCR_Init();
	ADC_Init(ADC1, ADC_RESOLUTION);
	// USART.
	USART_Init(USART_SGKCU, USART2);

	/*** Simulator ***/

	ZPT_Init();
	KVB_Init();

	/*** Global variables initialisation ***/

	/*** Main loop ***/

	while(1) {
		ADC_Routine(true);
		ZPT_Routine();
		KVB_Routine(true);
	}

	return 0;
}
