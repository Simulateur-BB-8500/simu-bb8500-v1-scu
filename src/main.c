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
#include "enum.h"
#include "gpio.h"
#include "gpio_reg.h"
#include "mapping.h"
#include "nvic.h"
#include "rcc.h"
#include "sw2.h"
#include "tim.h"
#include "tim_reg.h"
#include "usart.h"
#include "usart_reg.h"

/* MAIN FUNCTION.
 * @param: None.
 * @return: 0.
 */
int main(void) {

	/*** Peripherals initialisation ***/

	// Clocks and timers.
	RCC_Init();
	TIM_InitMs();
	// GPIO.
	GPIO_Init();
	// DAC.
	DAC_Init();
	// ADC (only ADC1 is used).
	ADCCR_Init();
	ADC_Init(ADC1, bits8);
	// USART;
	USART_Init(USART_SGKCU, USART2);

	//TIM_Init(TIM6, 1, seconds, true);
	//TIM_Start(TIM6, true);

	//TIM_Init(TIM7, 200, milliseconds, true);
	//TIM_Start(TIM7, true);

	/*** Global variables initialisation ***/

	SW2_Struct bl;
	SW2_Init(&bl, BUTTON, LOW, 2000);

	/*** Main loop ***/

	while(1) {
		SW2_UpdateState(&bl);
		if (bl.state == ON) {
			GPIO_Write(LED1, HIGH);
		}
		else {
			GPIO_Write(LED1, LOW);
		}
		ADC_Routine(true);
	}

	return 0;
}
