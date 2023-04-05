/*
 * gpio.h
 *
 *  Created on: 12 sept. 2017
 *      Author: Ludo
 */

#ifndef __GPIO_H__
#define __GPIO_H__

#include "gpio_reg.h"
#include "stdint.h"

/*** GPIO structures ***/

// GPIO structure.
typedef struct {
	GPIO_registers_t* port; // GPIOA to GPIOC.
	uint8_t port_index; // 0 for GPIOA, 1 for GPIOB, etc.
	uint8_t pin; // 0 to 15.
	uint8_t alternate_function; // Alternate function number if used.
} GPIO;

typedef enum {
	GPIO_MODE_INPUT = 0,
	GPIO_MODE_OUTPUT,
	GPIO_MODE_ALTERNATE_FUNCTION,
	GPIO_MODE_ANALOG
} GPIO_mode_t;

typedef enum {
	GPIO_TYPE_PUSH_PULL = 0,
	GPIO_TYPE_OPEN_DRAIN
} GPIO_output_type_t;

typedef enum {
	GPIO_SPEED_LOW = 0,
	GPIO_SPEED_MEDIUM,
	GPIO_SPEED_HIGH,
	GPIO_SPEED_VERY_HIGH,
} GPIO_output_speed_t;

typedef enum {
	GPIO_PULL_NONE = 0,
	GPIO_PULL_UP,
	GPIO_PULL_DOWN
} GPIO_pull_resistor_t;

/*** GPIO functions ***/

void GPIO_init(void);
void GPIO_configure(const GPIO* gpio, GPIO_mode_t mode, GPIO_output_type_t output_type, GPIO_output_speed_t output_speed, GPIO_pull_resistor_t pull_resistor);
void GPIO_write(const GPIO* gpio, uint8_t state);
uint8_t GPIO_read(const GPIO* gpio);
void GPIO_toggle(const GPIO* gpio);

#endif /* __GPIO_H__ */
