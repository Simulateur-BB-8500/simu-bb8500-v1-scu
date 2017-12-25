/*
 * enum.h
 *
 *  Created on: 15 sept. 2017
 *      Author: Ludovic
 */

#ifndef REGISTERS_ENUM_H_
#define REGISTERS_ENUM_H_

/*** PERIPHERAL LEVEL ***/

typedef enum {
	false = 0,
	true = 1
} boolean;

#define GPIO_STATE_ENUM_OFFSET 10

typedef enum {
	LOW = 10,
	HIGH
} GPIO_State;

typedef enum {
	microseconds = 20,
	milliseconds,
	seconds
} Time_Unit;

typedef enum {
	DAC_Channel1 = 30,
	DAC_Channel2
} DAC_Channel;

#define ADC_CHANNEL_ENUM_OFFSET 40

typedef enum {
	ADC_Channel0 = 40,
	ADC_Channel1,
	ADC_Channel2,
	ADC_Channel3,
	ADC_Channel4,
	ADC_Channel5,
	ADC_Channel6,
	ADC_Channel7,
	ADC_Channel8,
	ADC_Channel9,
	ADC_Channel10,
	ADC_Channel11,
	ADC_Channel12,
	ADC_Channel13,
	ADC_Channel14,
	ADC_Channel15,
	NC,
	VREF,
	VBAT
} ADC_Channel;

typedef enum {
	bits6 = 63,
	bits8 = 255,
	bits10 = 1023,
	bits12 = 4095
} ADC_Resolution;

typedef enum {
	ADC_OFF = 70,
	ADC_ZPT,
	ADC_PBL2,
	ADC_FPB,
	ADC_FD,
	ADC_INV,
	ADC_S,
	ADC_BPSAB,
	ADC_ZF,
	ADC_ZLFR,
	ADC_AM
} ADC_StateMachine;

/*** GENERIC LEVEL ***/

typedef enum {
	SW2_OFF = 100,
	SW2_CONFIRM_ON,
	SW2_ON,
	SW2_CONFIRM_OFF
} SW2_StateMachine;

typedef enum {
	OFF = 110,
	ON
} SW2_State;

typedef enum {
	SW3_NEUTRAL = 120,
	SW3_CONFIRM_NEUTRAL,
	SW3_BACK,
	SW3_CONFIRM_BACK,
	SW3_FRONT,
	SW3_CONFIRM_FRONT
} SW3_StateMachine;

typedef enum {
	BACK = 130,
	NEUTRAL,
	FRONT
} SW3_State;

typedef enum {
	SW4_P0 = 140,
	SW4_CONFIRM_P0,
	SW4_P1,
	SW4_CONFIRM_P1,
	SW4_P2,
	SW4_CONFIRM_P2,
	SW4_P3,
	SW4_CONFIRM_P3
} SW4_StateMachine;

typedef enum {
	P0 = 150,
	P1,
	P2,
	P3
} SW4_State;

#endif /* REGISTERS_ENUM_H_ */
