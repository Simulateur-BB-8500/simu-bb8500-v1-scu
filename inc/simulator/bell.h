/*
 * bell.h
 *
 *  Created on: 22 mar. 2020
 *      Author: Ludo
 */

#ifndef __BELL_H__
#define __BELL_H__

/*** BELL functions ***/

/*!******************************************************************
 * \fn void BELL_init(void)
 * \brief Init BELL driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void BELL_init(void);

/*!******************************************************************
 * \fn void BELL_process(void)
 * \brief Main task of BELL driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void BELL_process(void);

#endif /* __BELL_H___ */
