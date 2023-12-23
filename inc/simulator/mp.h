/*
 * mp.h
 *
 *  Created on: 22 mar. 2020
 *      Author: Ludo
 */

#ifndef __MP_H__
#define __MP_H__

/*** MP functions ***/

/*!******************************************************************
 * \fn void MP_init(void)
 * \brief Init MP driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void MP_init(void);

/*!******************************************************************
 * \fn void MP_process(void)
 * \brief Main task of MP driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void MP_process(void);

#endif /* __MP_H__ */
