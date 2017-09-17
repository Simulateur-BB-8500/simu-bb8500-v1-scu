/*
 * main.c
 *
 *  Created on: 5 sept. 2017
 *      Author: Ludovic
 */

#include "gpio.h"
#include "gpio_reg.h"
#include "mapping.h"
#include "nvic.h"
#include "rcc.h"
#include "tim.h"
#include "tim_reg.h"

/* MAIN FUNCTION.
 * @param: None.
 * @return: 0.
 */
int main(void) {

	/*** Peripherals initialisation ***/

	RCC_Init();
	GPIO_Init();
	NVIC_Init();

	// LED1.
	TIM_Set(TIM6, 1, seconds);
	TIM_Start(TIM6);
	TIM_EnableInterrupt(TIM6);

	// LED2.
	TIM_Set(TIM7, 500, microseconds);
	TIM_Start(TIM7);
	TIM_EnableInterrupt(TIM7);

	/*** Global variables initialisation ***/

	/*** Main loop ***/

	while(1) {
	}

	return 0;
}
