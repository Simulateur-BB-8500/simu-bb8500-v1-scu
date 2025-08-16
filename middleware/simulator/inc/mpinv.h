/*
 * mpinv.h
 *
 *  Created on: 08 apr. 2018
 *      Author: Ludo
 */

#ifndef __MPINV_H__
#define __MPINV_H__

/*** MPINV functions ***/

/*!******************************************************************
 * \fn void BL_init(void)
 * \brief Init BL driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void MPINV_init(void);

/*!******************************************************************
 * \fn void BL_process(void)
 * \brief Main task of BL driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void MPINV_process(void);

#endif /* __MPINV_H__ */
