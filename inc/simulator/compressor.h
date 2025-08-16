/*
 * compressor.h
 *
 *  Created on: 07 may 2020
 *      Author: Ludo
 */

#ifndef __COMPRESSOR_H__
#define __COMPRESSOR_H__

/*** COMPRESSOR functions ***/

/*!******************************************************************
 * \fn void COMPRESSOR_init(void)
 * \brief Init COMPRESSOR driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void COMPRESSOR_init(void);

/*!******************************************************************
 * \fn void COMPRESSOR_process(void)
 * \brief Main task of compressor driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void COMPRESSOR_process(void);

#endif /* __COMPRESSOR_H__ */
