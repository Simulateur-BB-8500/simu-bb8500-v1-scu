/*
 * main.c
 *
 *  Created on: 5 sept. 2017
 *      Author: Ludovic
 */

#include "gpio.h"
#include "gpio_reg.h"
#include "mapping.h"
#include "rcc.h"
#include "type.h"

/* MAIN FUNCTION.
 * @param: None.
 * @return: 0.
 */
int main(void) {

	/*** Peripherals initialisation ***/

	RCC_Init();
	GPIO_Init();

	/*** Global variables initialisation ***/

	boolean currentButton = LOW;
	boolean previousButton = LOW;

	/*** Main loop ***/

	while(1) {
		// Read button state.
		if (GPIO_Read(BUTTON)) {
			currentButton = LOW;
		}
		else {
			currentButton = HIGH;
		}

		// Toggle LED.
		if ((currentButton == HIGH) && (previousButton == LOW)) {
			GPIO_Toggle(LED);
		}

		// Update button state.
		previousButton = currentButton;
	}

	return 0;
}
