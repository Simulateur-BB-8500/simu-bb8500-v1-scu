/*
 * math.c
 *
 *  Created on: 28 aug. 2020
 *      Author: Ludo
 */

#include "math.h"
#include "stdint.h"

/*** MATH local macros ***/

#define MATH_MEDIAN_FILTER_LENGTH_MAX	0xFF
#define MATH_DECIMAL_MAX_DIGITS			10

/*** MATH functions ***/

/* COMPUTE A POWER A 10.
 * @param power:	Desired power.
 * @return result:	Result of computation.
 */
uint32_t MATH_pow_10(uint8_t power) {
	uint32_t result = 0;
	uint32_t pow10_buf[MATH_DECIMAL_MAX_DIGITS] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
	if (power < MATH_DECIMAL_MAX_DIGITS) {
		result = pow10_buf[power];
	}
	return result;
}

/* COMPUTE AVERAGE VALUE.
 * @param data:			Input buffer.
 * @param data_length:	Input buffer length.
 * @return average: 	Average value of the input buffer.
 */
uint32_t MATH_average(uint32_t* data, uint8_t data_length) {
	// Local variables.
	uint8_t idx = 0;
	uint32_t average = 0;
	// Compute
	for (idx=0 ; idx<data_length ; idx++) {
		average = ((average * idx) + data[idx]) / (idx + 1);
	}
	return average;
}

/* COMPUTE AVERAGE MEDIAN VALUE
 * @param data:				Input buffer.
 * @param median_length:	Number of elements taken for median value search.
 * @param average_length:	Number of center elements taken for final average.
 * @return filter_out:		Output value of the median filter.
 */
uint32_t MATH_median_filter(uint32_t* data, uint8_t median_length, uint8_t average_length) {
	// Local variables.
	uint32_t local_buf[MATH_MEDIAN_FILTER_LENGTH_MAX];
	uint8_t buffer_sorted = 0;
	uint8_t idx1 = 0;
	uint8_t idx2 = 0;
	uint8_t start_idx = 0;
	uint8_t end_idx = 0;
	uint32_t filter_out = 0;
	uint32_t temp = 0;
	// Copy input buffer into local buffer.
	for (idx1=0 ; idx1<median_length ; idx1++) {
		local_buf[idx1] = data[idx1];
	}
	// Sort buffer in ascending order.
	for (idx1=0; idx1<median_length; ++idx1) {
		buffer_sorted = 1;
		for (idx2=1 ; idx2<(median_length-idx1) ; ++idx2) {
			if (local_buf[idx2 - 1] > local_buf[idx2]) {
				temp = local_buf[idx2 - 1];
				local_buf[idx2 - 1] = local_buf[idx2];
				local_buf[idx2] = temp;
				buffer_sorted = 0;
			}
		}
		if (buffer_sorted != 0) break;
	}
	// Compute average of center values if required.
	if (average_length > 0) {
		// Clamp value.
		if (average_length > median_length) {
			average_length = median_length;
		}
		start_idx = (median_length / 2) - (average_length / 2);
		end_idx = (median_length / 2) + (average_length / 2);
		if (end_idx >= median_length) {
			end_idx = (median_length - 1);
		}
		// Compute average.
		filter_out = MATH_average(&(data[start_idx]), (end_idx - start_idx + 1));
	}
	else {
		// Return median value.
		filter_out = local_buf[(median_length / 2)];
	}
	return filter_out;
}
