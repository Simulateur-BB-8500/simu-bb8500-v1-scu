/*
 * flash.c
 *
 *  Created on: 19 dec. 2021
 *      Author: Ludo
 */

#include "flash.h"

#include "flash_reg.h"

/*** FLASH local macros ***/

#define FLASH_TIMEOUT_COUNT		10000

/*** FLASH functions ***/

/* SET FLASH LATENCY.
 * @param wait_states:	Number of wait states.
 * @return:				None.
 */
void FLASH_SetLatency(unsigned char wait_states) {
	// Configure number of wait states.
	FLASH -> ACR &= ~(0b1111 << 0); // Reset bits.
	FLASH -> ACR |= ((wait_states & 0b1111) << 0); // Set latency.
	// Wait until configuration is done.
	unsigned int count = 0;
	while ((((FLASH -> ACR) & (0b1111 << 0)) != ((wait_states & 0b1111) << 0)) && (count < FLASH_TIMEOUT_COUNT)) {
		count++;
	}
	// Enable prefetch in case of wait state.
	if (wait_states > 0) {
		FLASH -> ACR |= (0b1 << 8);
	}
}
