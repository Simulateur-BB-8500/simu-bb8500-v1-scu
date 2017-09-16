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
#include "type.h"

/* MAIN FUNCTION.
 * @param: None.
 * @return: 0.
 */
int main(void) {

	/*** Peripherals initialisation ***/

	RCC_Init();
	GPIO_Init();
	NVIC_Init();
	TIM6_Init();
	TIM6_Start();

	/*** Global variables initialisation ***/

	/*** Main loop ***/

	while(1) {
	}

	return 0;
}
