/*
 * gpio.h
 *
 *  Created on: 12 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_GPIO_GPIO_H_
#define PERIPHERALS_GPIO_GPIO_H_

#include "gpio_reg.h"
#include "types.h"

/*** GPIO structures ***/

typedef struct {
	GPIO_BaseAddress* GPIO_Port; // GPIOA to GPIOK.
	unsigned int GPIO_Num; // 0 to 15.
} GPIO_Struct;

/*** GPIO functions ***/

void GPIO_Init(void);
void GPIO_Write(GPIO_Struct* gpioStruct, GPIOState value);
GPIOState GPIO_Read(GPIO_Struct* gpioStruct);
void GPIO_Toggle(GPIO_Struct* gpioStruct);

#endif /* PERIPHERALS_GPIO_GPIO_H_ */
