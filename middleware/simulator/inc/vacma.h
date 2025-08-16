/*
 * vacma.h
 *
 *  Created on: 22 mar. 2020
 *      Author: Ludo
 */

#ifndef __VACMA_H__
#define __VACMA_H__

/*** VACMA functions ***/

/*!******************************************************************
 * \fn void VACMA_init(void)
 * \brief Init VACMA driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void VACMA_init(void);

/*!******************************************************************
 * \fn void VACMA_process(void)
 * \brief Main task of VACMA driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void VACMA_process(void);

#endif /* __VACMA_H__ */
