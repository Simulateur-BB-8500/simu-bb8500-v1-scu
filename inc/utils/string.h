/*
 * string.h
 *
 *  Created on: 05 dec. 2021
 *      Author: Ludo
 */

#ifndef __STRING_H__
#define __STRING_H__

#include "mode.h"
#include "types.h"

#ifdef DEBUG

/*** STRING macros ***/

#define STRING_CHAR_NULL	'\0'
#define STRING_CHAR_CR		'\r'
#define STRING_CHAR_LF		'\n'
#define STRING_CHAR_MINUS	'-'

/*** STRING structures ***/

typedef enum {
	STRING_FORMAT_BOOLEAN,
	STRING_FORMAT_HEXADECIMAL,
	STRING_FORMAT_DECIMAL,
	STRING_FORMAT_ASCII
} STRING_format_t;

/*** STRING functions ***/

uint8_t STRING_char_to_value(char_t ascii_code);
char_t STRING_decimal_to_ascii(uint8_t decimal_digit);
char_t STRING_hexa_to_ascii(uint8_t hexa_digit);
uint8_t STRING_is_hexadecimal_char(char_t ascii_code);
uint8_t STRING_is_decimal_char(char_t ascii_code);
void STRING_value_to_string(int32_t value, STRING_format_t format, uint8_t print_prefix, char_t* string);

#endif /* DEBUG */

#endif /* __STRING_H__ */
