/*
 * bpgd.h
 *
 *  Created on: 26 dec. 2021
 *      Author: Ludo
 */

#ifndef __BPGD_H__
#define __BPGD_H__

/*** BPGD functions ***/

/*!******************************************************************
 * \fn void BPGD_init(void)
 * \brief Init BPGD driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void BPGD_init(void);

/*!******************************************************************
 * \fn void BPGD_process(void)
 * \brief Main task of BPGD driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void BPGD_process(void);

#endif /* __BPGD_H__ */
