/*
 * bl.h
 *
 *  Created on: 8 apr. 2018
 *      Author: Ludo
 */

#ifndef __BL_H__
#define __BL_H__

/*** BL functions ***/

/*!******************************************************************
 * \fn void BL_init(void)
 * \brief Init BL driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void BL_init(void);

/*!******************************************************************
 * \fn void BL_process(void)
 * \brief Main task of BL driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void BL_process(void);

#endif /* __BL_H__ */
