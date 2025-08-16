/*
 * zpt.h
 *
 *  Created on: 25 dec. 2017
 *      Author: Ludo
 */

#ifndef __ZPT_H__
#define __ZPT_H__

#include "stdint.h"

/*** ZPT functions ***/

/*!******************************************************************
 * \fn void ZPT_init(void)
 * \brief Init ZPT driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void ZPT_init(void);

/*!******************************************************************
 * \fn void ZPT_process(void)
 * \brief Main task of ZPT driver.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void ZPT_process(void);

#endif /* __ZPT_H__ */
