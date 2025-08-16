/*
 * rcc.c
 *
 *  Created on: 09 sep. 2017
 *      Author: Ludo
 */

#include "rcc.h"

#include "flash.h"
#include "rcc_reg.h"
#include "stdint.h"

/*** RCC local macros ***/

#define RCC_HSI_FREQUENCY_DEFAULT_HZ    16000000

/*** RCC local structures ***/

/*******************************************************************/
typedef struct {
    uint32_t clock_frequency[RCC_CLOCK_LAST];
} RCC_context_t;

/*** RCC local global variables ***/

static RCC_context_t rcc_ctx;

/*** RCC functions ***/

/*******************************************************************/
void RCC_init(void) {
    // Peripherals clock prescalers:
    // HPRE = 1 -> HCLK = SYSCLK = 200MHz (max 216).
    // PPRE1 = 4 -> PCLK1 = HCLK/4 = 50MHz (max 54).
    // PPRE2 = 4 -> PCLK2 = HCLK/4 = 50MHz (max 108).
    RCC->CFGR &= 0xFFE0030F; // Reset bits 4-7 and 10-15 + HPRE='0000' (1).
    RCC->CFGR |= (0b101 << 10) | (0b101 << 13); // PPRE1='101' (4) and PPRE2='101' (4).
    RCC->DKCFGR1 &= ~(0b1 << 24); // Timers clock is 2*PLCKx (PPRE1 and PPRE2 != 1).
    // Configure main PLL.
    RCC->CR &= ~(0b1 << 24);
    // Source = HSI 16MHz
    // M = 16 -> input clock = 1MHz (typical value).
    // N = 400 -> VCO output clock = 400MHz (max 432).
    // P = 2 -> PLLCLK = 200MHz (max 216).
    // Q = 8 -> PLL48CLK = 50MHz (min 48 - max 75).
    RCC->PLLCFGR = 0; // Reset all bits + PLLSRC='0' (HSI) + PLLP='00' (2)
    RCC->PLLCFGR |= (16 << 0) | (400 << 6) | (8 << 24);
    // Enable PLL.
    RCC->CR |= (0b1 << 24);
    // Wait for PLL to be ready.
    while (((RCC->CR) & (0b1 << 25)) == 0);
    // Increase flash latency according to new system clock frequency (see p.74 of RM0385 datasheet).
    // HCLK = SYSCLK = 200MHz -> flash latency must be set to 6 wait states (WS).
    FLASH_set_latency(6);
    // Switch to PLLCLK.
    RCC->CFGR |= (0b10 << 0); // SW='10'.
    while (((RCC->CFGR) & (0b11 << 0)) != 0b10); // // Wait for clock switch (SWS='00').
#ifdef RCC_OUTPUT_CLOCK
    // Output HSI on MCO1 (PA8 as AF0) and SYSCLK on MCO2 (PC9 as AF0) with both prescalers = 4.
    RCC -> CFGR &= 0x369FFFFF;
    RCC -> CFGR |= 0x36000000;
#endif
    // Update frequencies.
    rcc_ctx.clock_frequency[RCC_CLOCK_SYSTEM] = 200000000;
    rcc_ctx.clock_frequency[RCC_CLOCK_APB1] = 50000000;
    rcc_ctx.clock_frequency[RCC_CLOCK_APB2] = 50000000;
}

/*******************************************************************/
uint32_t RCC_get_frequency_hz(RCC_clock_t rcc_clock) {
    // Local variables.
    uint32_t freq_khz = 0;
    // Check parameter.
    if (rcc_clock < RCC_CLOCK_LAST) {
        freq_khz = rcc_ctx.clock_frequency[rcc_clock];
    }
    return freq_khz;
}
