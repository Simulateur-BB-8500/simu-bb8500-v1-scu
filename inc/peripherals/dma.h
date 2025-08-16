/*
 * dma.h
 *
 *  Created on: 06 apr. 2023
 *      Author: Ludo
 */

#ifndef __DMA_H__
#define __DMA_H__

#include "stdint.h"

/*** DMA functions ***/

/*!******************************************************************
 * \fn void DMA2_STR0_init(void)
 * \brief Init DMA2 stream 0 peripheral for ADC samples transfer.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void DMA2_STR0_init(void);

#endif /* __DMA_H__ */
