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
#include "gpio_reg.h"
#include "mapping.h"
#include "nvic.h"
#include "rcc.h"
#include "tim.h"
#include "tim_reg.h"
#include "types.h"

/* MAIN FUNCTION.
 * @param: None.
 * @return: 0.
 */
int main(void) {

	/*** Peripherals initialisation ***/

	RCC_Init();
	GPIO_Init();
	NVIC_Init();
	DAC_Init();

	// ADC.
	ADCCR_Init();
	ADC_Init(ADC1, bit12);
	ADC_SetChannel(ADC1, ADCChannel6);
	ADC_StartConversion(ADC1);

	// LED1.
	/*TIM_Set(TIM6, 1, seconds);
	TIM_Start(TIM6, true);
	TIM_EnableInterrupt(TIM6);*/

	// LED2.
	/*TIM_Set(TIM7, 10, microseconds);
	TIM_Start(TIM7, true);
	TIM_EnableInterrupt(TIM7);*/

	/*** Global variables initialisation ***/

	/*** Main loop ***/

	while(1) {
	}

	return 0;
}
