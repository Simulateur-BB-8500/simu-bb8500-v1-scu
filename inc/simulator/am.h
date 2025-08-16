/*
 * am.h
 *
 *  Created on: 30 dec. 2023
 *      Author: Ludo
 */

#ifndef __AM_H__
#define __AM_H__

/*** AM functions ***/

/*!******************************************************************
 * \fn void AM_init(void)
 * \brief Init AM driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void AM_init(void);

/*!******************************************************************
 * \fn void AM_process(void)
 * \brief Main task of AM driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void AM_process(void);

#endif /* __AM_H__ */
