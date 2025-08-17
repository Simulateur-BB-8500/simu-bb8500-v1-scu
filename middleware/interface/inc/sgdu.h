/*
 * sgdu.h
 *
 *  Created on: 01 oct. 2017
 *      Author: Ludo
 */

#ifndef SGDU_H
#define SGDU_H

#include "scu_commands.h"
#include "stdint.h"

/*** SGDU functions ***/

/*!******************************************************************
 * \fn void SGDU_init(void)
 * \brief Init SGDU interface.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void SGDU_init(void);

/*!******************************************************************
 * \fn void SGDU_write(uint8_t command)
 * \brief Send a command to SGDU.
 * \param[in]   command: Command to send.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void SGDU_write(SCU_output_command_t command);

/*!******************************************************************
 * \fn void SGDU_process(void)
 * \brief Main task of SGDU interface.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void SGDU_process(void);

#endif /* SGDU_H */
