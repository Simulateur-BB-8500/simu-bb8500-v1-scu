/*
 * zba.h
 *
 *  Created on: 06 feb. 2020
 *      Author: Ludo
 */

#ifndef __ZBA_H__
#define __ZBA_H__

/*** ZBA functions ***/

/*!******************************************************************
 * \fn void ZBA_init(void)
 * \brief Init ZBA driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void ZBA_init(void);

/*!******************************************************************
 * \fn void ZBA_process(void)
 * \brief Main task of ZBA driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void ZBA_process(void);

#endif /* __ZBA_H__ */
