/*
 * flash.h
 *
 *  Created on: 19 dec. 2021
 *      Author: Ludo
 */

#ifndef __FLASH_H__
#define __FLASH_H__

#include "stdint.h"

/*** FLASH functions ***/

/*!******************************************************************
 * \fn FLASH_status_t FLASH_set_latency(uint8_t wait_states)
 * \brief Set FLASH latency.
 * \param[in]  	wait_states: Number of wait states to set.
 * \param[out] 	none
 * \retval		Function execution status.
 *******************************************************************/
void FLASH_set_latency(uint8_t wait_states);

#endif /* __FLASH_H__ */
