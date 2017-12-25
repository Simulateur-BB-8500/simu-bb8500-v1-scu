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
#include "sw2.h"
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

	//DAC_Init();

	// ADC.
	/*ADCCR_Init();
	ADC_Init(ADC1, bit12);
	ADC_SetChannel(ADC1, ADCChannel6);
	ADC_StartConversion(ADC1);*/

	USART_Init(USART_SGKCU, USART2);

	//TIM_Init(TIM6, 1, seconds, true);
	//TIM_Start(TIM6, true);

	//TIM_Init(TIM7, 200, milliseconds, true);
	//TIM_Start(TIM7, true);

	/*** Global variables initialisation ***/

	SW2_Struct bl;
	bl.gpio = BUTTON;
	bl.activeState = LOW;
	bl.currentState = SW2_OFF;
	bl.state = OFF;
	bl.confirmDuration = 2000;
	bl.confirmStartTime = 0;

	/*** Main loop ***/

	while(1) {
		SW2_UpdateState(&bl);
		if (bl.state == ON) {
			GPIO_Write(LED1, HIGH);
		}
		else {
			GPIO_Write(LED1, LOW);
		}
	}

	return 0;
}
