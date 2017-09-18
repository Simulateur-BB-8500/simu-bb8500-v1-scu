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
	microseconds,
	milliseconds,
	seconds
} TimeUnit;

typedef enum {
	Channel1 = 1,
	Channel2 = 2
} DACChannel;

#endif /* REGISTERS_TYPES_H_ */
