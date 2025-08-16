/*
 * emergency.h
 *
 *  Created on: 26 dec. 2021
 *      Author: Ludo
 */

#ifndef __EMERGENCY_H__
#define __EMERGENCY_H__

/*!******************************************************************
 * \fn void EMERGENCY_init(void)
 * \brief Init EMERGENCY driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void EMERGENCY_init(void);

/*!******************************************************************
 * \fn void EMERGENCY_trigger(void)
 * \brief Trigger emergency brake.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void EMERGENCY_trigger(void);

/*!******************************************************************
 * \fn void EMERGENCY_process(void)
 * \brief Main task of emergency driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void EMERGENCY_process(void);

#endif /* __EMERGENCY_H__ */
