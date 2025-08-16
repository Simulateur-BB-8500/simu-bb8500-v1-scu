/*
 * adc.h
 *
 *  Created on: 19 sep. 2017
 *      Author: Ludo
 */

#ifndef __ADC_H__
#define __ADC_H__

#include "stdint.h"

/*** ADC macros ***/

#define ADC_CONVERSION_PERIOD_MS    10
#define ADC_FULL_SCALE              4095
#define ADC_VCC_DEFAULT_MV          3300

/*** ADC structures ***/

/*!******************************************************************
 * \enum ADC_data_index_t
 * \brief ADC data indexes.
 *******************************************************************/
typedef enum {
    ADC_DATA_INDEX_ZPT = 0,
    ADC_DATA_INDEX_PBL2,
    ADC_DATA_INDEX_FPB,
    ADC_DATA_INDEX_AMCS,
    ADC_DATA_INDEX_FD,
    ADC_DATA_INDEX_MPINV,
    ADC_DATA_INDEX_S,
    ADC_DATA_INDEX_ZLFR,
    ADC_DATA_INDEX_LAST
} ADC_data_index_t;

/*** ADC functions ***/

/*!******************************************************************
 * \fn void ADC1_init(void)
 * \brief Init ADC peripheral.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void ADC1_init(void);

/*!******************************************************************
 * \fn ADC1_convert_to_mv(data)
 * \brief Convert ADC data to mV.
 * \param[in]   data: 12-bits ADC data to convert.
 * \param[out]  none
 * \retval      Function execution status.
 *******************************************************************/
#define ADC1_convert_to_mv(data)    ((data * ADC_VCC_DEFAULT_MV) / (ADC_FULL_SCALE))

#endif /* __ADC_H__ */
