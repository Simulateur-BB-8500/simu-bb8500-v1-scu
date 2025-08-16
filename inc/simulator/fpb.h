/*
 * fpb.h
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#ifndef __FPB_H__
#define __FPB_H__

/*** FPB functions ***/

/*!******************************************************************
 * \fn void FPB_init(void)
 * \brief Init FPB driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void FPB_init(void);

/*!******************************************************************
 * \fn void FPB_process(void)
 * \brief Main task of FPB driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void FPB_process(void);

#endif /* __FPB_H__ */
