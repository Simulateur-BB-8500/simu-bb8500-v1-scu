/*
 * gpio.h
 *
 *  Created on: 12 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_GPIO_GPIO_H_
#define PERIPHERALS_GPIO_GPIO_H_

#include "gpio_reg.h"
#include "enum.h"

/*** GPIO #define ***/

#define VCC_MV	3300 // MCU power supply in mV.

/*** GPIO structures ***/

typedef struct {
	GPIO_BaseAddress* port; // 'GPIOA' to 'GPIOK'.
	unsigned int num; // '0' to '15'.
} GPIO_Struct;

/*** GPIO parameters enumerations ***/

typedef enum {
	Input = 0,
	Output = 1,
	AlternateFunction = 2,
	Analog = 3
} GPIO_Mode;

typedef enum {
	PushPull = 0,
	OpenDrain = 1
} GPIO_OutputType;

typedef enum {
	LowSpeed = 0,
	MediumSpeed = 1,
	HighSpeed = 2,
	VeryHighSpeed = 3
} GPIO_OutputSpeed;

typedef enum {
	NoPullUpNoPullDown = 0,
	PullUp = 1,
	PullDown = 2
} GPIO_PullResistor;

/*** GPIO functions ***/

void GPIO_Configure(GPIO_Struct* gpioStruct, GPIO_Mode mode, GPIO_OutputType outputType, GPIO_OutputSpeed outputSpeed, GPIO_PullResistor pullResistor, unsigned int AFNum);
void GPIO_Init(void);
void GPIO_Write(GPIO_Struct* gpioStruct, GPIO_State value);
GPIO_State GPIO_Read(GPIO_Struct* gpioStruct);
void GPIO_Toggle(GPIO_Struct* gpioStruct);

#endif /* PERIPHERALS_GPIO_GPIO_H_ */
