/*
 * fd.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef __FD_H__
#define __FD_H__

/*** FD functions ***/

/*!******************************************************************
 * \fn void FD_init(void)
 * \brief Init FD driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void FD_init(void);

/*!******************************************************************
 * \fn void FD_process(void)
 * \brief Main task of FD driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void FD_process(void);

#endif /* __FD_H__ */
