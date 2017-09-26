/*
 * tim.c
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludovic
 */

#include "dac.h"
#include "gpio.h"
#include "mapping.h"
#include "masks.h"
#include "tim.h"
#include "tim_reg.h"

/* TIM interrupt handlers */

/* TIM6 INTERRUPT HANDLER.
 * @param: None.
 * @return: None.
 */
void TIM6_Handler(void) {
	TIM_ClearFlag(TIM6);
	GPIO_Toggle(LED1);
}

/* TIM7 INTERRUPT HANDLER.
 * @param: None.
 * @return: None.
 */
void TIM7_Handler(void) {
	TIM_ClearFlag(TIM7);
	// Get DAC current voltage.
	unsigned int currentVoltage = DAC_GetVoltage(DACChannel1);
	if (currentVoltage == DAC_FULL_SCALE) {
		currentVoltage = 0;
	}
	// Increase DAC output voltage.
	currentVoltage++;
	DAC_SetVoltage(DACChannel1, currentVoltage);
}
