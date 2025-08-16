/*
 * bpsa.h
 *
 *  Created on: 02 jan. 2024
 *      Author: Ludo
 */

#ifndef __BPSA_H__
#define __BPSA_H__

/*** BPSA functions ***/

/*!******************************************************************
 * \fn void BPSA_init(void)
 * \brief Init BPSA driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void BPSA_init(void);

/*!******************************************************************
 * \fn void BPSA_process(void)
 * \brief Main task of BPSA driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void BPSA_process(void);

#endif /* __BPSA_H__ */
