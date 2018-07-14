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

static SW4_Context zpt;

/*** ZPT functions ***/

/* INIT ZPT MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZPT_Init(void) {
	SW4_Init(&zpt, 2000);
}

/* UPDATE THE VOLTAGE READ ON ZPT SELECTOR (CALLED BY ADC ROUTINE).
 * @param new_voltage:	New voltage measured.
 * @return:				None.
 */
void ZPT_SetVoltage(unsigned int new_voltage) {
	SW4_SetVoltage(&zpt, new_voltage);
}

/* MAIN ROUTINE OF ZPT MODULE.
 * @param:	None.
 * @return:	None.
 */
void ZPT_Routine(void) {
	SW4_UpdateState(&zpt);
	switch (zpt.state) {
	case SW4_P0:
		GPIO_Write(LED1_GPIO, 1);
		GPIO_Write(LED2_GPIO, 0);
		GPIO_Write(LED3_GPIO, 0);
		GPIO_Write(LED4_GPIO, 0);
		break;
	case SW4_P1:
		GPIO_Write(LED1_GPIO, 0);
		GPIO_Write(LED2_GPIO, 1);
		GPIO_Write(LED3_GPIO, 0);
		GPIO_Write(LED4_GPIO, 0);
		break;
	case SW4_P2:
		GPIO_Write(LED1_GPIO, 0);
		GPIO_Write(LED2_GPIO, 0);
		GPIO_Write(LED3_GPIO, 1);
		GPIO_Write(LED4_GPIO, 0);
		break;
	case SW4_P3:
		GPIO_Write(LED1_GPIO, 0);
		GPIO_Write(LED2_GPIO, 0);
		GPIO_Write(LED3_GPIO, 0);
		GPIO_Write(LED4_GPIO, 1);
		break;
	default:
		// Unknown state.
		break;
	}
}
