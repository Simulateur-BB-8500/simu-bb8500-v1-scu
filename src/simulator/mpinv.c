/*
 * mpinv.c
 *
 *  Created on: 8 avr. 2018
 *      Author: Ludovic
 */

#include "gpio.h"
#include "mapping.h"
#include "mpinv.h"
#include "sw3.h"

/*** MPINV global variables ***/

static SW3_Context mpinv;

/*** MPINV functions ***/

/* INIT MPINV MODULE.
 * @param:	None.
 * @return:	None.
 */
void MPINV_Init(void) {
	GPIO_Configure(ADC_MPINV_GPIO, Analog, OpenDrain, LowSpeed, NoPullUpNoPullDown);
	SW3_Init(&mpinv, 2000);
}

/* UPDATE THE VOLTAGE READ ON MPINV SELECTOR (CALLED BY ADC ROUTINE).
 * @param new_voltage:	New voltage measured.
 * @return:				None.
 */
void MPINV_SetVoltage(unsigned int new_voltage) {
	SW3_SetVoltage(&mpinv, new_voltage);
}

/* MAIN ROUTINE OF MPINV MODULE.
 * @param:	None.
 * @return:	None.
 */
void MPINV_Routine(void) {
	SW3_UpdateState(&mpinv);
	switch (mpinv.state) {
	case SW3_BACK:
		GPIO_Write(LED1_GPIO, 1);
		GPIO_Write(LED2_GPIO, 0);
		GPIO_Write(LED3_GPIO, 0);
		break;
	case SW3_NEUTRAL:
		GPIO_Write(LED1_GPIO, 0);
		GPIO_Write(LED2_GPIO, 1);
		GPIO_Write(LED3_GPIO, 0);
		break;
	case SW3_FRONT:
		GPIO_Write(LED1_GPIO, 0);
		GPIO_Write(LED2_GPIO, 0);
		GPIO_Write(LED3_GPIO, 1);
		break;
	default:
		// Unknown state.
		break;
	}
}
