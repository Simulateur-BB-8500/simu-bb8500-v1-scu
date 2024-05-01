/*
 * font.h
 *
 *  Created on: 30 dec. 2023
 *      Author: Ludo
 */

#ifndef __FONT_H__
#define __FONT_H__

#include "stdint.h"

/*** FONT macros ***/

#define FONT_ASCII_TABLE_SIZE			128
#define FONT_ASCII_TABLE_OFFSET			32
#define FONT_TABLE_SIZE					(FONT_ASCII_TABLE_SIZE - FONT_ASCII_TABLE_OFFSET)

/*** FONT global variables ***/

extern const uint8_t FONT[FONT_TABLE_SIZE];

#endif /* __FONT_H__ */
