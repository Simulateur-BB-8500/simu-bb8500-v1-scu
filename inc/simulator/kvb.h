/*
 * kvb.h
 *
 *  Created on: 26 dec. 2017
 *      Author: Ludo
 */

#ifndef __KVB_H__
#define __KVB_H__

/*** KVB functions ***/

/*!******************************************************************
 * \fn void KVB_init(void)
 * \brief Init KVB driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void KVB_init(void);

/*!******************************************************************
 * \fn void KVB_process(void)
 * \brief Main task of KVB driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void KVB_process(void);

#endif /* __KVB_H__ */
