/*
 * zpt.c
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludovic
 */

#include "gpio.h"
#include "mapping.h"
#include "sw3.h"
#include "zpt.h"

/*** ZPT global variables ***/

static SW3_Struct zpt;

/*** ZPT functions ***/

void ZPT_Init(void) {
	SW3_Init(&zpt, ZPT, ZPT_ADC, 2000);
}

void ZPT_SetVoltage(unsigned int newVoltage) {
	SW3_SetVoltage(&zpt, newVoltage);
}

void ZPT_Routine(void) {
	SW3_UpdateState(&zpt);
	switch (zpt.state) {
	case NEUTRAL:
		GPIO_Write(LED1, LOW);
		GPIO_Write(LED2, HIGH);
		GPIO_Write(LED3, LOW);
		break;
	case BACK:
		GPIO_Write(LED1, LOW);
		GPIO_Write(LED2, LOW);
		GPIO_Write(LED3, HIGH);
		break;
	case FRONT:
		GPIO_Write(LED1, HIGH);
		GPIO_Write(LED2, LOW);
		GPIO_Write(LED3, LOW);
		break;
	default:
		// Unknown state.
		break;
	}
}
