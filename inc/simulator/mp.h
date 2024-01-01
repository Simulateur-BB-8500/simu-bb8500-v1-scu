/*
 * mp.h
 *
 *  Created on: 22 mar. 2020
 *      Author: Ludo
 */

#ifndef __MP_H__
#define __MP_H__

/*** MP macros ***/

#define MP_VARIATOR_STEP_MAX_SERIES_COUPLING	20

/*** MP structures ***/

/*!******************************************************************
 * \enum MP_motors_coupling_t
 * \brief Motors coupling modes list.
 *******************************************************************/
typedef enum {
	MP_MOTORS_COUPLING_SERIES = 0,
	MP_MOTORS_COUPLING_PARALLEL,
	MP_MOTORS_COUPLING_LAST
} MP_motors_coupling_t;

/*** MP functions ***/

/*!******************************************************************
 * \fn void MP_init(void)
 * \brief Init MP driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void MP_init(void);

/*!******************************************************************
 * \fn void MP_process(void)
 * \brief Main task of MP driver.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void MP_process(void);

#endif /* __MP_H__ */
