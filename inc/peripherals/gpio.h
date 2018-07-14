/*
 * gpio.h
 *
 *  Created on: 12 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_GPIO_H
#define PERIPHERALS_GPIO_H

#include "gpio_reg.h"

/*** GPIO macros ***/

// MCU power supply in mV.
#define VCC_MV	3300

/*** GPIO structures ***/

// GPIO peripheral structure.
typedef struct {
	GPIO_BaseAddress* port; // 'GPIOA' to 'GPIOK'.
	unsigned int num; // '0' to '15'.
	unsigned char af_num; // Alternate function number if used.
} GPIO_Periph;

/*** GPIO enumerations ***/

typedef enum {
	Input,
	Output,
	AlternateFunction,
	Analog
} GPIO_Mode;

typedef enum {
	PushPull,
	OpenDrain
} GPIO_OutputType;

typedef enum {
	LowSpeed,
	MediumSpeed,
	HighSpeed,
	VeryHighSpeed
} GPIO_OutputSpeed;

typedef enum {
	NoPullUpNoPullDown,
	PullUp,
	PullDown
} GPIO_PullResistor;

/*** GPIO functions ***/

void GPIO_Configure(GPIO_Periph gpio_periph, GPIO_Mode mode, GPIO_OutputType output_type, GPIO_OutputSpeed output_speed, GPIO_PullResistor pull_resistor);
void GPIO_Init(void);
void GPIO_Write(GPIO_Periph gpio_periph, unsigned char state);
unsigned char GPIO_Read(GPIO_Periph gpio_periph);
void GPIO_Toggle(GPIO_Periph gpio_periph);

#endif /* PERIPHERALS_GPIO_H */
