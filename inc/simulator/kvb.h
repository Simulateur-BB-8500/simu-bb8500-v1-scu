/*
 * kvb.h
 *
 *  Created on: 26 dec. 2017
 *      Author: Ludo
 */

#ifndef KVB_H
#define KVB_H

/*** KVB macros ***/

#define KVB_YG_PA400		((unsigned char*) "PA 400")
#define KVB_YG_UC512		((unsigned char*) "UC 512")
#define KVB_YG_888			((unsigned char*) "888888")
#define KVB_YG_DASH			((unsigned char*) "------")
#define KVB_G_B				((unsigned char*) "    b ")
#define KVB_Y_B				((unsigned char*) " b    ")
#define KVB_G_P				((unsigned char*) "    P ")
#define KVB_Y_P				((unsigned char*) " P    ")
#define KVB_G_L				((unsigned char*) "    L ")
#define KVB_Y_L				((unsigned char*) " L    ")
#define KVB_G_00			((unsigned char*) "    00")
#define KVB_Y_00			((unsigned char*) " 00   ")
#define KVB_G_000			((unsigned char*) "   000")
#define KVB_Y_000			((unsigned char*) "000   ")

/*** KVB functions ***/

void KVB_Init(void);
void KVB_StartSweepTimer(void);
void KVB_StopSweepTimer(void);
void KVB_Display(unsigned char* display);
void KVB_DisplayOff(void);
void KVB_Sweep(void);
void KVB_EnableBlinkLVAL(unsigned char blink_enabled);
void KVB_EnableBlinkLSSF(unsigned char blink_enabled);
void KVB_Task(void);

#endif /* KVB_H */
