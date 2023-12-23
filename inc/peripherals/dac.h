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
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void DAC_init(void);

/*!******************************************************************
 * \fn void DAC_set_voltage_mv(uint32_t voltage_mv)
 * \brief Set DAC output voltage.
 * \param[in]  	voltage_mv: Output voltage in mV.
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void DAC_set_voltage_mv(uint32_t voltage_mv);

/*!******************************************************************
 * \fn void DAC_get_voltage_mv(void)
 * \brief Read current DAC output voltage.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		Current DAC output voltage in mV.
 *******************************************************************/
uint32_t DAC_get_voltage_mv(void);

#endif /* __DAC_H__ */
