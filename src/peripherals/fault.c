/*
 * fault.c
 *
 *  Created on: 25 dec. 2021
 *      Author: Ludo
 */

#include "scb_reg.h"

/* NON MASKABLE INTERRUPT HANDLER.
 * @param:	None.
 * @return:	None.
 */
void __attribute__((optimize("-O0"))) NMI_Handler(void) {
	// Trigger software reset.
	SCB -> AIRCR = 0x05FA0000 | ((SCB -> AIRCR) & 0x0000FFFF) | (0b1 << 2);
}

/* HARD FAULT INTERRUPT HANDLER.
 * @param:	None.
 * @return:	None.
 */
void __attribute__((optimize("-O0"))) HardFault_Handler(void) {
	// Trigger software reset.
	SCB -> AIRCR = 0x05FA0000 | ((SCB -> AIRCR) & 0x0000FFFF) | (0b1 << 2);
}

/* MEMORY FAULT INTERRUPT HANDLER.
 * @param:	None.
 * @return:	None.
 */
void __attribute__((optimize("-O0"))) MemManage_Handler(void) {
	// Trigger software reset.
	SCB -> AIRCR = 0x05FA0000 | ((SCB -> AIRCR) & 0x0000FFFF) | (0b1 << 2);
}

/* BUS FAULT INTERRUPT HANDLER.
 * @param:	None.
 * @return:	None.
 */
void __attribute__((optimize("-O0"))) BusFault_Handler(void) {
	// Trigger software reset.
	SCB -> AIRCR = 0x05FA0000 | ((SCB -> AIRCR) & 0x0000FFFF) | (0b1 << 2);
}

/* USAGE FAULT INTERRUPT HANDLER.
 * @param:	None.
 * @return:	None.
 */
void __attribute__((optimize("-O0"))) UsageFault_Handler(void) {
	// Trigger software reset.
	SCB -> AIRCR = 0x05FA0000 | ((SCB -> AIRCR) & 0x0000FFFF) | (0b1 << 2);
}
