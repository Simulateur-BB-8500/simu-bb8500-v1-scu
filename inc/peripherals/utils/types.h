/*
 * types.h
 *
 *  Created on: 15 sept. 2017
 *      Author: Ludovic
 */

#ifndef REGISTERS_TYPES_H_
#define REGISTERS_TYPES_H_

/*** Types declaration ***/

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
	OFF = 20,
	ON
} SW2_State;

typedef enum {
	BACK = 30,
	NEUTRAL,
	FRONT
} SW3_State;

typedef enum {
	P0 = 40,
	P1,
	P2,
	P3
} SW4_State;

typedef enum {
	microseconds = 50,
	milliseconds,
	seconds
} Time_Unit;

typedef enum {
	bit6 = 63,
	bit8 = 255,
	bit10 = 1023,
	bit12 = 4095
} ADC_Resolution;

typedef enum {
	DAC_Channel1 = 70,
	DAC_Channel2
} DAC_Channel;

#define ADC_CHANNEL_ENUM_OFFSET 80

typedef enum {
	ADC_Channel0 = 80,
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

#endif /* REGISTERS_TYPES_H_ */
