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
	LOW = 0,
	HIGH = 1
} GPIOState;

typedef enum {
	false = 0,
	true = 1
} boolean;

typedef enum {
	microseconds = 0,
	milliseconds = 1,
	seconds = 2
} TimeUnit;

typedef enum {
	DACChannel1 = 1,
	DACChannel2 = 2
} DACChannel;

typedef enum {
	bit6 = 63,
	bit8 = 255,
	bit10 = 1023,
	bit12 = 4095
} ADCResolution;

typedef enum {
	ADCChannel0 = 0,
	ADCChannel1 = 1,
	ADCChannel2 = 2,
	ADCChannel3 = 3,
	ADCChannel4 = 4,
	ADCChannel5 = 5,
	ADCChannel6 = 6,
	ADCChannel7 = 7,
	ADCChannel8 = 8,
	ADCChannel9 = 9,
	ADCChannel10 = 10,
	ADCChannel11 = 11,
	ADCChannel12 = 12,
	ADCChannel13 = 13,
	ADCChannel14 = 14,
	ADCChannel15 = 15,
	NC = 16,
	VREF = 17,
	VBAT = 18
} ADCChannel;

#endif /* REGISTERS_TYPES_H_ */
