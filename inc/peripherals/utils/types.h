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
} boolean;

typedef enum {
	microseconds,
	milliseconds,
	seconds
} TimeUnit;

#endif /* REGISTERS_TYPES_H_ */
