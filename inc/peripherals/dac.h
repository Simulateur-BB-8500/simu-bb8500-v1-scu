/*
 * dac.h
 *
 *  Created on: 18 sep. 2017
 *      Author: Ludo
 */

#ifndef __DAC_H__
#define __DAC_H__

#include "stdint.h"

/*** DAC functions ***/

/*!******************************************************************
 * \fn void DAC_init(void)
 * \brief Init DAC peripheral.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void DAC_init(void);

/*!******************************************************************
 * \fn void DAC_set_output(uint32_t output_12bits)
 * \brief Set DAC output voltage.
 * \param[in]   output_12bits: 12-bits output voltage.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void DAC_set_output(uint32_t output_12bits);

#endif /* __DAC_H__ */
