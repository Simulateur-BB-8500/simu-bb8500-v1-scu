/*
 * dac.c
 *
 *  Created on: 18 sep. 2017
 *      Author: Ludo
 */

#include "dac.h"

#include "adc.h"
#include "dac_reg.h"
#include "gpio.h"
#include "mapping.h"
#include "rcc_reg.h"
#include "stdint.h"

/*** DAC functions ***/

/*******************************************************************/
void DAC_init(void) {
    // Configure analog GPIOs.
    GPIO_configure(&GPIO_AM, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_DOWN);
    // Enable peripheral clock.
    RCC->APB1ENR |= (0b1 << 29); // DACEN='1'.
    // Configure peripheral.
    DAC->CR &= ~(0b1 << 1); // BOFF1='0'.
    DAC->CR |= (0b1 << 17); // BOFF2='1'.
    DAC->CR |= (0b1 << 0); // EN1='1'.
    DAC->CR &= ~(0b1 << 16); // EN2='0'.
    // Reset output.
    DAC->DHR12R1 = 0;
}

/*******************************************************************/
void DAC_set_output(uint32_t output_12bits) {
    // Set register.
    DAC->DHR12R1 = (output_12bits & 0x3FFF);
}
