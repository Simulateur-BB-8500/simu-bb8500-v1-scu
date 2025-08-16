/*
 * zsur.h
 *
 *  Created on: 27 dec. 2021
 *      Author: Ludo
 */

#ifndef __ZSUR_H__
#define __ZSUR_H__

/*** ZSUR functions ***/

/*!******************************************************************
 * \fn void ZSUR_init(void)
 * \brief Init ZSUR driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void ZSUR_init(void);

/*!******************************************************************
 * \fn void ZSUR_process(void)
 * \brief Main task of ZSUR driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void ZSUR_process(void);

#endif /* __ZSUR_H__ */
