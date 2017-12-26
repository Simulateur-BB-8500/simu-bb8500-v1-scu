/*
 * sw3.h
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludovic
 */

#ifndef GENERIC_INPUTS_SW3_H_
#define GENERIC_INPUTS_SW3_H_

#include "adc.h"
#include "enum.h"
#include "gpio.h"

/*** 3-poles switch #define ***/

#define SW3_DELTA_HYSTERESIS_MV		100 // Set the voltage difference (in mV) between low and high thresholds.

#define SW3_DELTA_HYSTERESIS		((SW3_DELTA_HYSTERESIS_MV*ADC_RESOLUTION)/(VCC_MV))
#define SW3_BACK_THRESHOLD_LOW 		((ADC_RESOLUTION/4)-(SW3_DELTA_HYSTERESIS/2))
#define SW3_BACK_THRESHOLD_HIGH 	((ADC_RESOLUTION/4)+(SW3_DELTA_HYSTERESIS/2))
#define SW3_FRONT_THRESHOLD_LOW 	((3*(ADC_RESOLUTION/4))-(SW3_DELTA_HYSTERESIS/2))
#define SW3_FRONT_THRESHOLD_HIGH 	((3*(ADC_RESOLUTION/4))+(SW3_DELTA_HYSTERESIS/2))

/*** 3-poles switch structure ***/

typedef struct {
	GPIO_Struct* gpio;
	ADC_Struct* adc;
	unsigned int voltage; // Current voltage measured by ADC.
	SW3_StateMachine machineState; // Current state in SW3 state machine.
	SW3_State state; // State after anti-bouncing (used in higher levels).
	unsigned int debouncingMs; // Delay before validating states (in ms).
	unsigned int confirmStartTime;
} SW3_Struct;

/*** 3-poles switch functions ***/

void SW3_Init(SW3_Struct* sw3, GPIO_Struct* pGpio, ADC_Struct* pAdc, unsigned int pDebouncingMs);
void SW3_SetVoltage(SW3_Struct* sw3, unsigned int newVoltage);
void SW3_UpdateState(SW3_Struct* sw3);

#endif /* GENERIC_INPUTS_SW3_H_ */
