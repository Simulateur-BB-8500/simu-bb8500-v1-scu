/*
 * gpio.c
 *
 *  Created on: 12 sept. 2017
 *      Author: Ludovic
 */

#include "gpio.h"
#include "gpio_reg.h"
#include "mapping.h"
#include "masks.h"
#include "types.h"

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
} GPIO_PullUpPullDown;

/*** GPIO internal functions ***/

/* SET THE MODE OF A GPIO PIN.
 * @param gpioStruct: Pointer to GPIO identifier (port + number).
 * @param mode: Desired mode ('Input', 'Output', 'AlternateFunction' or 'Analog').
 * @return: None.
 */
void GPIO_SetMode(GPIO_Struct* gpioStruct, GPIO_Mode mode) {
	// Extract port and number.
	GPIO_BaseAddress* gpioPort = gpioStruct -> GPIO_Port;
	unsigned int gpioNum = gpioStruct -> GPIO_Num;
	// Ensure GPIO exists.
	if ((gpioNum >= 0) && (gpioNum < GPIO_PER_PORT)) {
		switch(mode) {
		case Input:
			// MODERy = '00'.
			gpioPort -> MODER &= ~BIT_MASK[2*gpioNum];
			gpioPort -> MODER &= ~BIT_MASK[2*gpioNum+1];
			break;
		case Output:
			// MODERy = '01'.
			gpioPort -> MODER |= BIT_MASK[2*gpioNum];
			gpioPort -> MODER &= ~BIT_MASK[2*gpioNum+1];
			break;
		case Analog:
			// MODERy = '11'.
			gpioPort -> MODER |= BIT_MASK[2*gpioNum];
			gpioPort -> MODER |= BIT_MASK[2*gpioNum+1];
			break;
		case AlternateFunction:
			// MODERy = '10'.
			gpioPort -> MODER &= ~BIT_MASK[2*gpioNum];
			gpioPort -> MODER |= BIT_MASK[2*gpioNum+1];
			break;
		default:
			break;
		}
	}
}

/* GET THE MODE OF A GPIO PIN.
 * @param gpioStruct: Pointer to GPIO identifier (port + number).
 * @return gpioMode: Current mode of the  GPIO ('Input', 'Output', 'AlternateFunction' or 'Analog').
 */
GPIO_Mode GPIO_GetMode(GPIO_Struct* gpioStruct) {
	// Extract port and number.
	GPIO_BaseAddress* gpioPort = gpioStruct -> GPIO_Port;
	unsigned int gpioNum = gpioStruct -> GPIO_Num;
	boolean bit0 = ((gpioPort -> MODER) & BIT_MASK[2*gpioNum]) >> (2*gpioNum);
	boolean bit1 = ((gpioPort -> MODER) & BIT_MASK[2*gpioNum+1]) >> (2*gpioNum+1);
	GPIO_Mode gpioMode = (bit1 << 1) + bit0;
	return gpioMode;
}

/* SET THE OUTPUT TYPE OF A GPIO PIN.
 * @param gpioStruct: Pointer to GPIO identifier (port + number).
 * @param outputType: Desired output ('PushPull' or 'OpenDrain').
 * @return: None.
 */
void GPIO_SetOutputType(GPIO_Struct* gpioStruct, GPIO_OutputType outputType) {
	// Extract port and number.
	GPIO_BaseAddress* gpioPort = gpioStruct -> GPIO_Port;
	unsigned int gpioNum = gpioStruct -> GPIO_Num;
	// Ensure GPIO exists.
	if ((gpioNum >= 0) && (gpioNum < GPIO_PER_PORT)) {
		switch(outputType) {
		case PushPull:
			// OTy = '0'.
			gpioPort -> OTYPER &= ~BIT_MASK[gpioNum];
			break;
		case OpenDrain:
			// OTy = '1'.
			gpioPort -> OTYPER |= BIT_MASK[gpioNum];
			break;
		default:
			break;
		}
	}
}

/* SET THE OUTPUT SPEED OF A GPIO PIN.
 * @param gpioStruct: Pointer to GPIO identifier (port + number).
 * @param outputSpeed: Desired output speed ('LowSpeed', 'MediumSpeed', 'HighSpeed' or 'VeryHighSpeed').
 * @return: None.
 */
void GPIO_SetOutputSpeed(GPIO_Struct* gpioStruct, GPIO_OutputSpeed outputSpeed) {
	// Extract port and number.
	GPIO_BaseAddress* gpioPort = gpioStruct -> GPIO_Port;
	unsigned int gpioNum = gpioStruct -> GPIO_Num;
	// Ensure GPIO exists.
	if ((gpioNum >= 0) && (gpioNum < GPIO_PER_PORT)) {
		switch(outputSpeed) {
		case LowSpeed:
			// OSPEEDRy = '00'.
			gpioPort -> OSPEEDR &= ~BIT_MASK[2*gpioNum];
			gpioPort -> OSPEEDR &= ~BIT_MASK[2*gpioNum+1];
			break;
		case MediumSpeed:
			// OSPEEDRy = '01'.
			gpioPort -> OSPEEDR |= BIT_MASK[2*gpioNum];
			gpioPort -> OSPEEDR &= ~BIT_MASK[2*gpioNum+1];
			break;
		case HighSpeed:
			// OSPEEDRy = '10'.
			gpioPort -> OSPEEDR &= ~BIT_MASK[2*gpioNum];
			gpioPort -> OSPEEDR |= BIT_MASK[2*gpioNum+1];
			break;
		case VeryHighSpeed:
			// OSPEEDRy = '11'.
			gpioPort -> OSPEEDR |= BIT_MASK[2*gpioNum];
			gpioPort -> OSPEEDR |= BIT_MASK[2*gpioNum+1];
			break;
		default:
			break;
		}
	}
}

/* ENABLE OR DISABLE PULL-UP AND PULL-DOWN RESISTORS ON A GPIO PIN.
 * @param gpioStruct: Pointer to GPIO identifier (port + number).
 * @param pullUpPullDown: Desired configuration ('NoPullUpNoPullDown', 'PullUp', or 'PullDown').
 * @return: None.
 */
void GPIO_SetPullUpPullDown(GPIO_Struct* gpioStruct, GPIO_PullUpPullDown pullUpPullDown) {
	// Extract port and number.
	GPIO_BaseAddress* gpioPort = gpioStruct -> GPIO_Port;
	unsigned int gpioNum = gpioStruct -> GPIO_Num;
	// Ensure GPIO exists.
	if ((gpioNum >= 0) && (gpioNum < GPIO_PER_PORT)) {
		switch(pullUpPullDown) {
		case NoPullUpNoPullDown:
			// PUPDRy = '00'.
			gpioPort -> PUPDR &= ~BIT_MASK[2*gpioNum];
			gpioPort -> PUPDR &= ~BIT_MASK[2*gpioNum+1];
			break;
		case PullUp:
			// PUPDRy = '01'.
			gpioPort -> PUPDR |= BIT_MASK[2*gpioNum];
			gpioPort -> PUPDR &= ~BIT_MASK[2*gpioNum+1];
			break;
		case PullDown:
			// PUPDRy = '10'.
			gpioPort -> PUPDR &= ~BIT_MASK[2*gpioNum];
			gpioPort -> PUPDR |= BIT_MASK[2*gpioNum+1];
			break;
		default:
			break;
		}
	}
}

/* SELECT THE ALTERNATE FUNCTION OF A GPIO PIN (REQUIRES THE MODE 'AlternateFunction').
 * @param gpioStruct: Pointer to GPIO identifier (port + number).
 * @param AFNum: Alternate function number (0 to 15).
 * @return: None.
 */
void GPIO_SetAlternateFunction(GPIO_Struct* gpioStruct, unsigned int AFNum) {
	// Extract port and number.
	GPIO_BaseAddress* gpioPort = gpioStruct -> GPIO_Port;
	unsigned int gpioNum = gpioStruct -> GPIO_Num;
	// Ensure alternate function exists.
	if ((AFNum >= 0) && (AFNum < AF_PER_GPIO)) {
		unsigned int i = 0;
		// Select proper register to set.
		if ((gpioNum >= 0) && (gpioNum < AFRH_OFFSET)) {
			// Set AFRL register: AFRy = 'AFNum'.
			for (i=0 ; i<4 ; i++) {
				if (AFNum && BIT_MASK[i]) {
					// Bit = '1'.
					gpioPort -> AFRL |= BIT_MASK[4*gpioNum+i];
				}
				else {
					// Bit = '0'.
					gpioPort -> AFRL &= ~BIT_MASK[4*gpioNum+i];
				}
			}
		}
		else {
			if ((gpioNum >= AFRH_OFFSET) && (gpioNum < GPIO_PER_PORT)) {
				// Set AFRH register: AFRy = 'AFNum'.
				for (i=0 ; i<4 ; i++) {
					if (AFNum && BIT_MASK[i]) {
						// Bit = '1'.
						gpioPort -> AFRH |= BIT_MASK[4*(gpioNum-AFRH_OFFSET)+i];
					}
					else {
						// Bit = '0'.
						gpioPort -> AFRH &= ~BIT_MASK[4*(gpioNum-AFRH_OFFSET)+i];
					}
				}
			}
		}
	}
}

/*** GPIO functions ***/

/* CONFIGURE MCU GPIOs.
 * @param: None.
 * @return: None.
 */
void GPIO_Init(void) {

	// LED pin configured as output.
	GPIO_SetMode(LED1, Output);
	GPIO_SetOutputType(LED1, PushPull);
	GPIO_SetOutputSpeed(LED1, LowSpeed);
	GPIO_SetPullUpPullDown(LED1, NoPullUpNoPullDown);

	// LED2 pin configured as output.
	GPIO_SetMode(LED2, Output);
	GPIO_SetOutputType(LED2, PushPull);
	GPIO_SetOutputSpeed(LED2, VeryHighSpeed);
	GPIO_SetPullUpPullDown(LED2, NoPullUpNoPullDown);

	// Button pin configured as input.
	GPIO_SetMode(BUTTON, Input);
	GPIO_SetOutputType(BUTTON, PushPull);
	GPIO_SetOutputSpeed(BUTTON, LowSpeed);
	GPIO_SetPullUpPullDown(BUTTON, PullUp);

	// AMP pin configured as analog.
	GPIO_SetMode(AMP, Analog);
	GPIO_SetPullUpPullDown(AMP, NoPullUpNoPullDown);

	// ZPT pin configured as analog.
	GPIO_SetMode(ZPT, Analog);
	GPIO_SetPullUpPullDown(ZPT, NoPullUpNoPullDown);

#ifdef OUTPUT_CLOCK
	// MCO1 configured as AF0 to output HSI clock.
	GPIO_SetMode(MCO1, AlternateFunction);
	GPIO_SetAlternateFunction(MCO1, 0);

	// MCO2 configured as AF0 to output SYSCLK clock.
	GPIO_SetMode(MCO2, AlternateFunction);
	GPIO_SetAlternateFunction(MCO2, 0);
#endif
}

/* SET THE STATE OF A GPIO.
 * @param gpioStruct: Pointer to GPIO identifier (port + number).
 * @param state: Desired state of the pin ('LOW' or 'HIGH').
 * @return: None.
 */
void GPIO_Write(GPIO_Struct* gpioStruct, GPIOState state) {
	// Extract port and number.
	GPIO_BaseAddress* gpioPort = gpioStruct -> GPIO_Port;
	unsigned int gpioNum = gpioStruct -> GPIO_Num;
	// Ensure GPIO exists.
	if ((gpioNum >= 0) && (gpioNum < GPIO_PER_PORT)) {
		if (state == LOW) {
			gpioPort -> ODR &= ~BIT_MASK[gpioNum];
		}
		else {
			gpioPort -> ODR |= BIT_MASK[gpioNum];
		}
	}
}

/* READ THE STATE OF A GPIO.
 * @param gpioStruct: Pointer to GPIO identifier (port + number).
 * @return: GPIO state ('LOW' or 'HIGH').
 */
GPIOState GPIO_Read(GPIO_Struct* gpioStruct) {
	// Extract port and number.
	GPIO_BaseAddress* gpioPort = gpioStruct -> GPIO_Port;
	unsigned int gpioNum = gpioStruct -> GPIO_Num;
	GPIOState result = LOW;
	if ((gpioNum >= 0) && (gpioNum < GPIO_PER_PORT)) {
		switch (GPIO_GetMode(gpioStruct)) {
		case Input:
			// GPIO configured as input -> read IDR register.
			result = ((gpioPort -> IDR) & BIT_MASK[gpioNum]) >> gpioNum;
			break;
		case Output:
			// GPIO configured as output -> read ODR register.
			result = ((gpioPort -> ODR) & BIT_MASK[gpioNum]) >> gpioNum;
			break;
		default:
			break;
		}
	}
	return result;
}

/* INVERT THE STATE OF A GPIO.
 * @param gpioStruct: Pointer to GPIO identifier (port + number).
 * @return: None.
 */
void GPIO_Toggle(GPIO_Struct* gpioStruct) {
	// Extract number.
	unsigned int gpioNum = gpioStruct -> GPIO_Num;
	// Ensure GPIO exists.
	if ((gpioNum >= 0) && (gpioNum < GPIO_PER_PORT)) {
		if (GPIO_Read(gpioStruct)) {
			GPIO_Write(gpioStruct, LOW);
		}
		else {
			GPIO_Write(gpioStruct, HIGH);
		}
	}
}
