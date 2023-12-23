/*
 * whistle.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef __WHISTLE_H__
#define __WHISTLE_H__

/*!******************************************************************
 * \fn void WHISTLE_init(void)
 * \brief Init whistle driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void WHISTLE_init(void);

/*!******************************************************************
 * \fn void WHISTLE_process(void)
 * \brief Main task of whistle driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void WHISTLE_process(void);

#endif /* __WHISTLE_H__ */
