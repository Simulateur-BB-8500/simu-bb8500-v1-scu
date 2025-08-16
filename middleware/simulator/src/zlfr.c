/*
 * zlfr.c
 *
 *  Created on: 21 mar. 2020
 *      Author: Ludo
 */

#include "zlfr.h"

#include "gpio.h"
#include "mapping.h"
#include "stdint.h"

/*** ZLFR functions ***/

/*******************************************************************/
void ZLFR_init(void) {
    // Init GPIO.
    GPIO_configure(&GPIO_ZLFR, GPIO_MODE_ANALOG, GPIO_TYPE_OPEN_DRAIN, GPIO_SPEED_LOW, GPIO_PULL_NONE);
}

