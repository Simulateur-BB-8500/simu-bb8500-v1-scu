/*
 * fault.c
 *
 *  Created on: 25 dec. 2021
 *      Author: Ludo
 */

#include "scb_reg.h"

/*******************************************************************/
void NMI_Handler(void) {
    // Trigger software reset.
    SCB->AIRCR = 0x05FA0000 | ((SCB->AIRCR) & 0x0000FFFF) | (0b1 << 2);
}

/*******************************************************************/
void HardFault_Handler(void) {
    // Trigger software reset.
    SCB->AIRCR = 0x05FA0000 | ((SCB->AIRCR) & 0x0000FFFF) | (0b1 << 2);
}

/*******************************************************************/
void MemManage_Handler(void) {
    // Trigger software reset.
    SCB->AIRCR = 0x05FA0000 | ((SCB->AIRCR) & 0x0000FFFF) | (0b1 << 2);
}

/*******************************************************************/
void BusFault_Handler(void) {
    // Trigger software reset.
    SCB->AIRCR = 0x05FA0000 | ((SCB->AIRCR) & 0x0000FFFF) | (0b1 << 2);
}

/*******************************************************************/
void UsageFault_Handler(void) {
    // Trigger software reset.
    SCB->AIRCR = 0x05FA0000 | ((SCB->AIRCR) & 0x0000FFFF) | (0b1 << 2);
}
