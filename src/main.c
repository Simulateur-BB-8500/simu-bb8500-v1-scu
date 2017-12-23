/*
 * main.c
 *
 *  Created on: 5 sept. 2017
 *      Author: Ludovic
 */

#include "adc.h"
#include "adc_reg.h"
#include "at.h"
#include "dac.h"
#include "gpio.h"
#include "gpio_reg.h"
#include "mapping.h"
#include "nvic.h"
#include "rcc.h"
#include "tim.h"
#include "tim_reg.h"
#include "types.h"
#include "usart.h"
#include "usart_reg.h"

/* MAIN FUNCTION.
 * @param: None.
 * @return: 0.
 */
int main(void) {

	/*** Peripherals initialisation ***/

	RCC_Init();
	TIM_InitMs();
	GPIO_Init();
	NVIC_Init();
	DAC_Init();

	// ADC.
	ADCCR_Init();
	ADC_Init(ADC1, bit12);
	ADC_SetChannel(ADC1, ADCChannel6);
	//ADC_StartConversion(ADC1);

	//USART_Init(USART2);

	/*TIM_Set(TIM6, 1, seconds);
	TIM_EnableInterrupt(TIM6);
	TIM_Start(TIM6, true);*/

	/*TIM_Set(TIM7, 10, microseconds);
	TIM_Start(TIM7, true);
	TIM_EnableInterrupt(TIM7);*/

	/*** Global variables initialisation ***/

	/*** Main loop ***/

	while(1) {
		TIM_DelayMs(3000);
		GPIO_Toggle(LED1);
	}

	return 0;
}
