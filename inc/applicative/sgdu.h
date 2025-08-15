/*
 * sgdu.h
 *
 *  Created on: 01 oct. 2017
 *      Author: Ludo
 */

#ifndef SGDU_H
#define SGDU_H

#include "tch.h"
#include "stdint.h"

/*** SGDU macros ***/

#define SGDU_SPEED_LIMIT_FACTOR		10

#define SGDU_MP_VARIATOR_STEP_0		0
#define SGDU_MP_VARIATOR_STEP_P		(-1)
#define SGDU_MP_VARIATOR_STEP_MAX	32
#define SGDU_MP_VARIATOR_STEP_MIN	(-11)

/*** SGDU structures ***/

/*!******************************************************************
 * \enum SCU_output_command_t
 * \brief SCU output commands to SGDU.
 *******************************************************************/
typedef enum {
	SCU_OUT_ZBA_ON = 0,
	SCU_OUT_ZBA_OFF,
	SCU_OUT_RSEC_ON,
	SCU_OUT_RSEC_OFF,
	SCU_OUT_ZDV_ON,
	SCU_OUT_ZDV_OFF,
	SCU_OUT_ZPT_REAR_UP,
	SCU_OUT_ZPT_REAR_DOWN,
	SCU_OUT_ZPT_FRONT_UP,
	SCU_OUT_ZPT_FRONT_DOWN,
	SCU_OUT_ZDJ_OFF,
	SCU_OUT_ZEN_ON,
	SCU_OUT_ZCA_REGULATION_MIN,
	SCU_OUT_ZCA_REGULATION_MAX,
	SCU_OUT_ZCD_ON,
	SCU_OUT_ZCX_OFF,
	SCU_OUT_PBL2_ON,
	SCU_OUT_PBL2_OFF,
	SCU_OUT_BPGD,
	SCU_OUT_FPB_APPLY,
	SCU_OUT_FPB_NEUTRAL,
	SCU_OUT_FPB_RELEASE,
	SCU_OUT_ZVM_ON,
	SCU_OUT_ZVM_OFF,
	SCU_OUT_MPINV_FORWARD,
	SCU_OUT_MPINV_NEUTRAL,
	SCU_OUT_MPINV_BACKWARD,
	SCU_OUT_MP_0,
	SCU_OUT_MP_T_MORE,
	SCU_OUT_MP_T_LESS,
	SCU_OUT_MP_P,
	SCU_OUT_MP_F_MORE,
	SCU_OUT_MP_F_LESS,
	SCU_OUT_FD_APPLY,
	SCU_OUT_FD_NEUTRAL,
	SCU_OUT_FD_RELEASE,
	SCU_OUT_WHISTLE_HIGH_TONE,
	SCU_OUT_WHISTLE_LOW_TONE,
	SCU_OUT_WHISTLE_NEUTRAL,
	SCU_OUT_BPEV_ON,
	SCU_OUT_BPEV_OFF,
	SCU_OUT_BPSA_ON,
	SCU_OUT_BPSA_OFF,
	SCU_OUT_ZFG_ON,
	SCU_OUT_ZFG_OFF,
	SCU_OUT_ZFD_ON,
	SCU_OUT_ZFD_OFF,
	SCU_OUT_ZPR_ON,
	SCU_OUT_ZPR_OFF,
	SCU_OUT_ZLFRG_ON,
	SCU_OUT_ZLFRG_OFF,
	SCU_OUT_ZLFRD_ON,
	SCU_OUT_ZLFRD_OFF,
	SCU_OUT_EMERGENCY_ON,
	SCU_OUT_EMERGENCY_OFF,
	SCU_OUT_NOP = 0xFF
} SCU_output_command_t;

/*!******************************************************************
 * \enum SCU_input_commands_t
 * \brief SCU input commands from SGDU.
 *******************************************************************/
typedef enum {
	// First range is reserved for coding speed in km/h.
	SCU_TCH_SPEED_OFFSET = 0,
	SCU_TCH_SPEED_LAST = (SCU_TCH_SPEED_OFFSET + TCH_SPEED_MAX_KMH),
	// Second range is reserved for coding speed limit in tens of km/h.
	SCU_SPEED_LIMIT_OFFSET = (SCU_TCH_SPEED_LAST + 1),
	SCU_SPEED_LIMIT_LAST = (SCU_SPEED_LIMIT_OFFSET + 1 + (TCH_SPEED_MAX_KMH / SGDU_SPEED_LIMIT_FACTOR)),
	// NOP.
	SCU_IN_NOP = 0xFF
} SCU_input_commands_t;

/*** SGDU functions ***/

/*!******************************************************************
 * \fn void SGDU_init(void)
 * \brief Init SGDU interface.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void SGDU_init(void);

/*!******************************************************************
 * \fn void SGDU_write(uint8_t command)
 * \brief Send a command to SGDU.
 * \param[in]  	command: Command to send.
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void SGDU_write(SCU_output_command_t command);

/*!******************************************************************
 * \fn void SGDU_process(void)
 * \brief Main task of SGDU interface.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void SGDU_process(void);

#endif /* SGDU_H */
