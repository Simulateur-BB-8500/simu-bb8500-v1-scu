/*
 * lsagiu.h
 *
 *  Created on: 01 oct. 2017
 *      Author: Ludo
 */

#ifndef LSAGIU_H
#define LSAGIU_H

#include "tch.h"
#include "stdint.h"

/*** LSAGIU macros ***/

#define LSAGIU_SPEED_LIMIT_FACTOR	10

/*** LSAGIU structures ***/

/*!******************************************************************
 * \enum LSMCU_output_command_t
 * \brief LSMCU output commands to LSAGIU.
 *******************************************************************/
typedef enum {
	LSMCU_OUT_ZBA_ON = 0,
	LSMCU_OUT_ZBA_OFF,
	LSMCU_OUT_RSEC_ON,
	LSMCU_OUT_RSEC_OFF,
	LSMCU_OUT_ZDV_ON,
	LSMCU_OUT_ZDV_OFF,
	LSMCU_OUT_ZPT_REAR_UP,
	LSMCU_OUT_ZPT_REAR_DOWN,
	LSMCU_OUT_ZPT_FRONT_UP,
	LSMCU_OUT_ZPT_FRONT_DOWN,
	LSMCU_OUT_ZDJ_OFF,
	LSMCU_OUT_ZEN_ON,
	LSMCU_OUT_ZCA_REGULATION_MIN,
	LSMCU_OUT_ZCA_REGULATION_MAX,
	LSMCU_OUT_ZCD_ON,
	LSMCU_OUT_ZCX_OFF,
	LSMCU_OUT_PBL2_ON,
	LSMCU_OUT_PBL2_OFF,
	LSMCU_OUT_BPGD,
	LSMCU_OUT_FPB_APPLY,
	LSMCU_OUT_FPB_NEUTRAL,
	LSMCU_OUT_FPB_RELEASE,
	LSMCU_OUT_ZVM_ON,
	LSMCU_OUT_ZVM_OFF,
	LSMCU_OUT_MPINV_FORWARD,
	LSMCU_OUT_MPINV_NEUTRAL,
	LSMCU_OUT_MPINV_BACKWARD,
	LSMCU_OUT_MP_0,
	LSMCU_OUT_MP_T_MORE,
	LSMCU_OUT_MP_T_LESS,
	LSMCU_OUT_MP_P,
	LSMCU_OUT_MP_F_MORE,
	LSMCU_OUT_MP_F_LESS,
	LSMCU_OUT_MP_FR,
	LSMCU_OUT_FD_APPLY,
	LSMCU_OUT_FD_NEUTRAL,
	LSMCU_OUT_FD_RELEASE,
	LSMCU_OUT_WHISTLE_HIGH_TONE,
	LSMCU_OUT_WHISTLE_LOW_TONE,
	LSMCU_OUT_WHISTLE_NEUTRAL,
	LSMCU_OUT_BPEV_ON,
	LSMCU_OUT_BPEV_OFF,
	LSMCU_OUT_BPSA_ON,
	LSMCU_OUT_BPSA_OFF,
	LSMCU_OUT_ZFG_ON,
	LSMCU_OUT_ZFG_OFF,
	LSMCU_OUT_ZFD_ON,
	LSMCU_OUT_ZFD_OFF,
	LSMCU_OUT_ZPR_ON,
	LSMCU_OUT_ZPR_OFF,
	LSMCU_OUT_ZLFRG_ON,
	LSMCU_OUT_ZLFRG_OFF,
	LSMCU_OUT_ZLFRD_ON,
	LSMCU_OUT_ZLFRD_OFF,
	LSMCU_OUT_EMERGENCY_ON,
	LSMCU_OUT_EMERGENCY_OFF,
	LSMCU_OUT_NOP = 0xFF
} LSMCU_output_command_t;

/*!******************************************************************
 * \enum LSMCU_input_commands_t
 * \brief LSMCU input commands from LSAGIU.
 *******************************************************************/
typedef enum {
	// First range is reserved for coding speed in km/h.
	LSMCU_TCH_SPEED_OFFSET = 0,
	LSMCU_TCH_SPEED_LAST = (LSMCU_TCH_SPEED_OFFSET + TCH_SPEED_MAX_KMH),
	// Second range is reserved for coding speed limit in tens of km/h.
	LSMCU_SPEED_LIMIT_OFFSET = (LSMCU_TCH_SPEED_LAST + 1),
	LSMCU_SPEED_LIMIT_LAST = (LSMCU_SPEED_LIMIT_OFFSET + 1 + (TCH_SPEED_MAX_KMH / LSAGIU_SPEED_LIMIT_FACTOR)),
	// NOP.
	LSMCU_IN_NOP = 0xFF
} LSMCU_input_commands_t;

/*** LSAGIU functions ***/

/*!******************************************************************
 * \fn void LSAGIU_init(void)
 * \brief Init LSAGIU interface.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void LSAGIU_init(void);

/*!******************************************************************
 * \fn void LSAGIU_write(uint8_t command)
 * \brief Send a command to LSAGIU.
 * \param[in]  	command: Command to send.
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void LSAGIU_write(LSMCU_output_command_t command);

/*!******************************************************************
 * \fn void LSAGIU_process(void)
 * \brief Main task of LSAGIU interface.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void LSAGIU_process(void);

#endif /* LSAGIU_H */
