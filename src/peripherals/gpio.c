/*
 * gpio.c
 *
 *  Created on: 12 sept. 2017
 *      Author: Ludovic
 */

#include "gpio.h"
#include "gpio_reg.h"
#include "mapping.h"
#include "rcc_reg.h"

/*** GPIO local functions ***/

/* SET THE MODE OF A GPIO PIN.
 * @param gpio_periph:	GPIO identifier.
 * @param mode: 		Desired mode ('Input', 'Output', 'AlternateFunction' or 'Analog').
 * @return: 			None.
 */
void GPIO_SetMode(GPIO_Periph gpio_periph, GPIO_Mode mode) {
	// Ensure GPIO exists.
	if ((gpio_periph.num >= 0) && (gpio_periph.num < GPIO_PER_PORT)) {
		switch(mode) {
		case Input:
			// MODERy = '00'.
			gpio_periph.port -> MODER &= ~(0b1 << (2*gpio_periph.num));
			gpio_periph.port -> MODER &= ~(0b1 << (2*gpio_periph.num+1));
			break;
		case Output:
			// MODERy = '01'.
			gpio_periph.port -> MODER |= (0b1 << (2*gpio_periph.num));
			gpio_periph.port -> MODER &= ~(0b1 << (2*gpio_periph.num+1));
			break;
		case Analog:
			// MODERy = '11'.
			gpio_periph.port -> MODER |= (0b1 << (2*gpio_periph.num));
			gpio_periph.port -> MODER |= (0b1 << (2*gpio_periph.num+1));
			break;
		case AlternateFunction:
			// MODERy = '10'.
			gpio_periph.port -> MODER &= ~(0b1 << (2*gpio_periph.num));
			gpio_periph.port -> MODER |= (0b1 << (2*gpio_periph.num+1));
			break;
		default:
			break;
		}
	}
}

/* GET THE MODE OF A GPIO PIN.
 * @param gpio_periph:	GPIO identifier.
 * @return gpioMode: 	Current mode of the  GPIO ('Input', 'Output', 'AlternateFunction' or 'Analog').
 */
GPIO_Mode GPIO_GetMode(GPIO_Periph gpio_periph) {
	unsigned char bit0 = ((gpio_periph.port -> MODER) & (0b1 << (2*gpio_periph.num))) >> (2*gpio_periph.num);
	unsigned char bit1 = ((gpio_periph.port -> MODER) & (0b1 << (2*gpio_periph.num+1))) >> (2*gpio_periph.num+1);
	GPIO_Mode gpioMode = (bit1 << 1) + bit0;
	return gpioMode;
}

/* SET THE OUTPUT TYPE OF A GPIO PIN.
 * @param gpio_periph:	GPIO identifier.
 * @param outputType: 	Desired output ('PushPull' or 'OpenDrain').
 * @return: 			None.
 */
void GPIO_SetOutputType(GPIO_Periph gpio_periph, GPIO_OutputType output_type) {
	// Ensure GPIO exists.
	if ((gpio_periph.num >= 0) && (gpio_periph.num < GPIO_PER_PORT)) {
		switch(output_type) {
		case PushPull:
			// OTy = '0'.
			gpio_periph.port -> OTYPER &= ~(0b1 << gpio_periph.num);
			break;
		case OpenDrain:
			// OTy = '1'.
			gpio_periph.port -> OTYPER |= (0b1 << gpio_periph.num);
			break;
		default:
			break;
		}
	}
}

/* SET THE OUTPUT SPEED OF A GPIO PIN.
 * @param gpio_periph:	GPIO identifier.
 * @param outputSpeed: 	Desired output speed ('LowSpeed', 'MediumSpeed', 'HighSpeed' or 'VeryHighSpeed').
 * @return: 			None.
 */
void GPIO_SetOutputSpeed(GPIO_Periph gpio_periph, GPIO_OutputSpeed output_speed) {
	// Ensure GPIO exists.
	if ((gpio_periph.num >= 0) && (gpio_periph.num < GPIO_PER_PORT)) {
		switch(output_speed) {
		case LowSpeed:
			// OSPEEDRy = '00'.
			gpio_periph.port -> OSPEEDR &= ~(0b1 << (2*gpio_periph.num));
			gpio_periph.port -> OSPEEDR &= ~(0b1 << (2*gpio_periph.num+1));
			break;
		case MediumSpeed:
			// OSPEEDRy = '01'.
			gpio_periph.port -> OSPEEDR |= (0b1 << (2*gpio_periph.num));
			gpio_periph.port -> OSPEEDR &= ~(0b1 << (2*gpio_periph.num+1));
			break;
		case HighSpeed:
			// OSPEEDRy = '10'.
			gpio_periph.port -> OSPEEDR &= ~(0b1 << (2*gpio_periph.num));
			gpio_periph.port -> OSPEEDR |= (0b1 << (2*gpio_periph.num+1));
			break;
		case VeryHighSpeed:
			// OSPEEDRy = '11'.
			gpio_periph.port -> OSPEEDR |= (0b1 << (2*gpio_periph.num));
			gpio_periph.port -> OSPEEDR |= (0b1 << (2*gpio_periph.num+1));
			break;
		default:
			break;
		}
	}
}

/* ENABLE OR DISABLE PULL-UP AND PULL-DOWN RESISTORS ON A GPIO PIN.
 * @param gpio_periph:		GPIO identifier.
 * @param pullResistor: 	Desired configuration ('NoPullUpNoPullDown', 'PullUp', or 'PullDown').
 * @return: 				None.
 */
void GPIO_SetPullUpPullDown(GPIO_Periph gpio_periph, GPIO_PullResistor pull_resistor) {
	// Ensure GPIO exists.
	if ((gpio_periph.num >= 0) && (gpio_periph.num < GPIO_PER_PORT)) {
		switch(pull_resistor) {
		case NoPullUpNoPullDown:
			// PUPDRy = '00'.
			gpio_periph.port -> PUPDR &= ~(0b1 << (2*gpio_periph.num));
			gpio_periph.port -> PUPDR &= ~(0b1 << (2*gpio_periph.num+1));
			break;
		case PullUp:
			// PUPDRy = '01'.
			gpio_periph.port -> PUPDR |= (0b1 << (2*gpio_periph.num));
			gpio_periph.port -> PUPDR &= ~(0b1 << (2*gpio_periph.num+1));
			break;
		case PullDown:
			// PUPDRy = '10'.
			gpio_periph.port -> PUPDR &= ~(0b1 << (2*gpio_periph.num));
			gpio_periph.port -> PUPDR |= (0b1 << (2*gpio_periph.num+1));
			break;
		default:
			break;
		}
	}
}

/* SELECT THE ALTERNATE FUNCTION OF A GPIO PIN (REQUIRES THE MODE 'AlternateFunction').
 * @param gpio_periph:	GPIO identifier.
 * @param af_num: 		Alternate function number (0 to 15).
 * @return: None.
 */
void GPIO_SetAlternateFunction(GPIO_Periph gpio_periph, unsigned int af_num) {
	// Ensure alternate function exists.
	if ((af_num >= 0) && (af_num < AF_PER_GPIO)) {
		unsigned int i = 0;
		// Select proper register to set.
		if ((gpio_periph.num >= 0) && (gpio_periph.num < AFRH_OFFSET)) {
			// Set AFRL register: AFRy = 'af_num'.
			for (i=0 ; i<4 ; i++) {
				if (af_num & (0b1 << i)) {
					// Bit = '1'.
					gpio_periph.port -> AFRL |= (0b1 << (4*gpio_periph.num+i));
				}
				else {
					// Bit = '0'.
					gpio_periph.port -> AFRL &= ~(0b1 << (4*gpio_periph.num+i));
				}
			}
		}
		else {
			if ((gpio_periph.num >= AFRH_OFFSET) && (gpio_periph.num < GPIO_PER_PORT)) {
				// Set AFRH register: AFRy = 'af_num'.
				for (i=0 ; i<4 ; i++) {
					if (af_num & (0b1 << i)) {
						// Bit = '1'.
						gpio_periph.port -> AFRH |= (0b1 << (4*(gpio_periph.num-AFRH_OFFSET)+i));
					}
					else {
						// Bit = '0'.
						gpio_periph.port -> AFRH &= ~(0b1 << (4*(gpio_periph.num-AFRH_OFFSET)+i));
					}
				}
			}
		}
	}
}

/*** GPIO functions ***/

/* FUNCTION FOR CONFIGURING A GPIO PIN.
 * @param gpio_periph:	GPIO identifier.
 * @param mode: 		Desired mode ('Input', 'Output', 'AlternateFunction' or 'Analog').
 * @param outputType:	Desired output ('PushPull' or 'OpenDrain').
 * @param outputSpeed: 	Desired output speed ('0Speed', 'MediumSpeed', '1Speed' or 'Very1Speed').
 * @param pullResistor: Desired configuration ('NoPullUpNoPullDown', 'PullUp', or 'PullDown').
 * @param af_num: 		Alternate function number (0 to 15) if 'AlternateFunction' mode is selected.
 */
void GPIO_Configure(GPIO_Periph gpio_periph, GPIO_Mode mode, GPIO_OutputType output_type, GPIO_OutputSpeed output_speed, GPIO_PullResistor pull_resistor) {
	GPIO_SetMode(gpio_periph, mode);
	if (mode == AlternateFunction) {
		GPIO_SetAlternateFunction(gpio_periph, gpio_periph.af_num);
	}
	GPIO_SetOutputType(gpio_periph, output_type);
	GPIO_SetOutputSpeed(gpio_periph, output_speed);
	GPIO_SetPullUpPullDown(gpio_periph, pull_resistor);
}

/* CONFIGURE MCU GPIOs.
 * @param: None.
 * @return: None.
 */
void GPIO_Init(void) {

	/* Enable all GPIOx clocks */

	RCC -> AHB1ENR |= 0x000007FF; // GPIOxEN='1'.

	/* Configure single GPIOs */

	GPIO_Configure(LED1_GPIO, Output, PushPull, LowSpeed, NoPullUpNoPullDown);
	GPIO_Configure(LED2_GPIO, Output, PushPull, LowSpeed, NoPullUpNoPullDown);
	GPIO_Configure(LED3_GPIO, Output, PushPull, LowSpeed, NoPullUpNoPullDown);
	GPIO_Configure(LED4_GPIO, Output, PushPull, LowSpeed, NoPullUpNoPullDown);

#ifdef RCC_OUTPUT_CLOCK
	// MCO1 configured as AF0 to output HSI clock.
	GPIO_Configure(MCO1_GPIO, AlternateFunction, PushPull, VeryHighSpeed, NoPullUpNoPullDown);
	// MCO2 configured as AF0 to output SYSCLK clock.
	GPIO_Configure(MCO2_GPIO, AlternateFunction, PushPull, VeryHighSpeed, NoPullUpNoPullDown);
#endif

	/* Others GPIOs are configured in their corresponding peripheral or applicative file */
}

/* SET THE STATE OF A GPIO.
 * @param gpio_periph:	GPIO identifier.
 * @param state: 		Desired state of the pin ('0' or '1').
 * @return: 			None.
 */
void GPIO_Write(GPIO_Periph gpio_periph, unsigned char state) {
	// Ensure GPIO exists.
	if ((gpio_periph.num >= 0) && (gpio_periph.num < GPIO_PER_PORT)) {
		if (state) {
			gpio_periph.port -> ODR |= (0b1 << gpio_periph.num);
		}
		else {
			gpio_periph.port -> ODR &= ~(0b1 << gpio_periph.num);
		}
	}
}

/* READ THE STATE OF A GPIO.
 * @param gpio_periph:	GPIO identifier.
 * @return state: 		GPIO state ('0' or '1').
 */
unsigned char GPIO_Read(GPIO_Periph gpio_periph) {
	unsigned char state = 0;
	if ((gpio_periph.num >= 0) && (gpio_periph.num < GPIO_PER_PORT)) {
		switch (GPIO_GetMode(gpio_periph)) {
		case Input:
			// GPIO configured as input -> read IDR register.
			if (((gpio_periph.port -> IDR) & (0b1 << gpio_periph.num)) != 0) {
				state = 1;
			}
			break;
		case Output:
			// GPIO configured as output -> read ODR register.
			if (((gpio_periph.port -> ODR) & (0b1 << gpio_periph.num)) != 0) {
				state = 1;
			}
			break;
		default:
			break;
		}
	}
	return state;
}

/* INVERT THE STATE OF A GPIO.
 * @param gpio_periph:	GPIO identifier (port + number).
 * @return: 			None.
 */
void GPIO_Toggle(GPIO_Periph gpio_periph) {
	// Ensure GPIO exists.
	if ((gpio_periph.num >= 0) && (gpio_periph.num < GPIO_PER_PORT)) {
		if (GPIO_Read(gpio_periph) == 0) {
			GPIO_Write(gpio_periph, 1);
		}
		else {
			GPIO_Write(gpio_periph, 0);
		}
	}
}
