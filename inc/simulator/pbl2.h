/*
 * pbl2.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef __PBL2_H__
#define __PBL2_H__

/*** PBL2 functions ***/

/*!******************************************************************
 * \fn void BL_init(void)
 * \brief Init BL driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void PBL2_init(void);

/*!******************************************************************
 * \fn void BL_process(void)
 * \brief Main task of BL driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void PBL2_process(void);

#endif /* __PBL2_H__ */
