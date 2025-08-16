/*
 * tch.h
 *
 *  Created on: 15 jul. 2018
 *      Author: Ludo
 */

#ifndef __TCH_H__
#define __TCH_H__

/*** TACHRO macros ***/

#define TCH_SPEED_MAX_KMH   160

/*** TACHRO functions ***/

/*!******************************************************************
 * \fn void TCH_init(void)
 * \brief Init Tachro driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TCH_init(void);

/*!******************************************************************
 * \fn void TCH_process(void)
 * \brief Main task of Tachro driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void TCH_process(void);

#endif /* __TCH_H__ */
