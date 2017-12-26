/*
 * zpt.c
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludovic
 */

#include "gpio.h"
#include "mapping.h"
#include "sw4.h"
#include "zpt.h"

/*** ZPT global variables ***/

static SW4_Struct zpt;

/*** ZPT functions ***/

void ZPT_Init(void) {
	SW4_Init(&zpt, ZPT, ZPT_ADC, 2000);
}

void ZPT_SetVoltage(unsigned int newVoltage) {
	SW4_SetVoltage(&zpt, newVoltage);
}

void ZPT_Routine(void) {
	SW4_UpdateState(&zpt);
	switch (zpt.state) {
	case P0:
		GPIO_Write(LED1, LOW);
		GPIO_Write(LED2, LOW);
		GPIO_Write(LED3, LOW);
		GPIO_Write(LED4, HIGH);
		break;
	case P1:
		GPIO_Write(LED1, LOW);
		GPIO_Write(LED2, LOW);
		GPIO_Write(LED3, HIGH);
		GPIO_Write(LED4, LOW);
		break;
	case P2:
		GPIO_Write(LED1, LOW);
		GPIO_Write(LED2, HIGH);
		GPIO_Write(LED3, LOW);
		GPIO_Write(LED4, LOW);
		break;
	case P3:
		GPIO_Write(LED1, HIGH);
		GPIO_Write(LED2, LOW);
		GPIO_Write(LED3, LOW);
		GPIO_Write(LED4, LOW);
		break;
	default:
		// Unknown state.
		break;
	}
}
