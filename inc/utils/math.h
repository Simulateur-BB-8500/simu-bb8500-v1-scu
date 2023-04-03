/*
 * math.h
 *
 *  Created on: 28 aug. 2020
 *      Author: Ludo
 */

#ifndef __MATH_H__
#define __MATH_H__

#include "types.h"

/*** MATH functions ***/

uint32_t MATH_pow_10(uint8_t power);
uint32_t MATH_average(uint32_t* data, uint8_t data_length);
uint32_t MATH_median_filter(uint32_t* data, uint8_t median_length, uint8_t average_length);

#endif /* __MATH_H__ */
