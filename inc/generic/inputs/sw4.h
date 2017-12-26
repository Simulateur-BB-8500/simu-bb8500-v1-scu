/*
 * sw4.h
 *
 *  Created on: 26 dec. 2017
 *      Author: Ludovic
 */

#ifndef GENERIC_INPUTS_SW4_H_
#define GENERIC_INPUTS_SW4_H_

/*** 4-poles switch #define ***/

#define SW4_DELTA_HYSTERESIS_MV		100 // Set the voltage difference (in mV) between low and high thresholds.

#define SW4_DELTA_HYSTERESIS		((SW4_DELTA_HYSTERESIS_MV*ADC_RESOLUTION)/(VCC_MV))
#define SW4_P0P1_THRESHOLD_LOW 		((ADC_RESOLUTION/6)-(SW4_DELTA_HYSTERESIS/2))
#define SW4_P0P1_THRESHOLD_HIGH 	((ADC_RESOLUTION/6)+(SW4_DELTA_HYSTERESIS/2))
#define SW4_P1P2_THRESHOLD_LOW 		((3*(ADC_RESOLUTION/6))-(SW4_DELTA_HYSTERESIS/2))
#define SW4_P1P2_THRESHOLD_HIGH 	((3*(ADC_RESOLUTION/6))+(SW4_DELTA_HYSTERESIS/2))
#define SW4_P2P3_THRESHOLD_LOW 		((5*(ADC_RESOLUTION/6))-(SW4_DELTA_HYSTERESIS/2))
#define SW4_P2P3_THRESHOLD_HIGH 	((5*(ADC_RESOLUTION/6))+(SW4_DELTA_HYSTERESIS/2))

/*** 4-poles switch structure ***/

typedef struct {
	GPIO_Struct* gpio;
	ADC_Struct* adc;
	unsigned int voltage; // Current voltage measured by ADC.
	SW4_StateMachine machineState; // Current state in SW4 state machine.
	SW4_State state; // State after anti-bouncing (used in higher levels).
	unsigned int debouncingMs; // Delay before validating states (in ms).
	unsigned int confirmStartTime;
} SW4_Struct;

/*** 4-poles switch functions ***/

void SW4_Init(SW4_Struct* sw4, GPIO_Struct* pGpio, ADC_Struct* pAdc, unsigned int pDebouncingMs);
void SW4_SetVoltage(SW4_Struct* sw4, unsigned int newVoltage);
void SW4_UpdateState(SW4_Struct* sw4);

#endif /* GENERIC_INPUTS_SW4_H_ */
