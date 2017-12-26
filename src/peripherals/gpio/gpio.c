/*
 * gpio.c
 *
 *  Created on: 12 sept. 2017
 *      Author: Ludovic
 */

#include "enum.h"
#include "gpio.h"
#include "gpio_reg.h"
#include "mapping.h"
#include "rcc.h"
#include "rcc_reg.h"

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

/*** GPIO internal functions ***/

/* SET THE MODE OF A GPIO PIN.
 * @param gpio: Pointer to GPIO identifier (port + number).
 * @param mode: Desired mode ('Input', 'Output', 'AlternateFunction' or 'Analog').
 * @return: None.
 */
void GPIO_SetMode(GPIO_Struct* gpioStruct, GPIO_Mode mode) {
	// Extract port and number.
	GPIO_BaseAddress* gpioPort = gpioStruct -> port;
	unsigned int gpioNum = gpioStruct -> num;
	// Ensure GPIO exists.
	if ((gpioNum >= 0) && (gpioNum < GPIO_PER_PORT)) {
		switch(mode) {
		case Input:
			// MODERy = '00'.
			gpioPort -> MODER &= ~BIT_MASK(2*gpioNum);
			gpioPort -> MODER &= ~BIT_MASK(2*gpioNum+1);
			break;
		case Output:
			// MODERy = '01'.
			gpioPort -> MODER |= BIT_MASK(2*gpioNum);
			gpioPort -> MODER &= ~BIT_MASK(2*gpioNum+1);
			break;
		case Analog:
			// MODERy = '11'.
			gpioPort -> MODER |= BIT_MASK(2*gpioNum);
			gpioPort -> MODER |= BIT_MASK(2*gpioNum+1);
			break;
		case AlternateFunction:
			// MODERy = '10'.
			gpioPort -> MODER &= ~BIT_MASK(2*gpioNum);
			gpioPort -> MODER |= BIT_MASK(2*gpioNum+1);
			break;
		default:
			break;
		}
	}
}

/* GET THE MODE OF A GPIO PIN.
 * @param gpio: Pointer to GPIO identifier (port + number).
 * @return gpioMode: Current mode of the  GPIO ('Input', 'Output', 'AlternateFunction' or 'Analog').
 */
GPIO_Mode GPIO_GetMode(GPIO_Struct* gpioStruct) {
	// Extract port and number.
	GPIO_BaseAddress* gpioPort = gpioStruct -> port;
	unsigned int gpioNum = gpioStruct -> num;
	boolean bit0 = ((gpioPort -> MODER) & BIT_MASK(2*gpioNum)) >> (2*gpioNum);
	boolean bit1 = ((gpioPort -> MODER) & BIT_MASK(2*gpioNum+1)) >> (2*gpioNum+1);
	GPIO_Mode gpioMode = (bit1 << 1) + bit0;
	return gpioMode;
}

/* SET THE OUTPUT TYPE OF A GPIO PIN.
 * @param gpio: Pointer to GPIO identifier (port + number).
 * @param outputType: Desired output ('PushPull' or 'OpenDrain').
 * @return: None.
 */
void GPIO_SetOutputType(GPIO_Struct* gpioStruct, GPIO_OutputType outputType) {
	// Extract port and number.
	GPIO_BaseAddress* gpioPort = gpioStruct -> port;
	unsigned int gpioNum = gpioStruct -> num;
	// Ensure GPIO exists.
	if ((gpioNum >= 0) && (gpioNum < GPIO_PER_PORT)) {
		switch(outputType) {
		case PushPull:
			// OTy = '0'.
			gpioPort -> OTYPER &= ~BIT_MASK(gpioNum);
			break;
		case OpenDrain:
			// OTy = '1'.
			gpioPort -> OTYPER |= BIT_MASK(gpioNum);
			break;
		default:
			break;
		}
	}
}

/* SET THE OUTPUT SPEED OF A GPIO PIN.
 * @param gpio: Pointer to GPIO identifier (port + number).
 * @param outputSpeed: Desired output speed ('LowSpeed', 'MediumSpeed', 'HighSpeed' or 'VeryHighSpeed').
 * @return: None.
 */
void GPIO_SetOutputSpeed(GPIO_Struct* gpioStruct, GPIO_OutputSpeed outputSpeed) {
	// Extract port and number.
	GPIO_BaseAddress* gpioPort = gpioStruct -> port;
	unsigned int gpioNum = gpioStruct -> num;
	// Ensure GPIO exists.
	if ((gpioNum >= 0) && (gpioNum < GPIO_PER_PORT)) {
		switch(outputSpeed) {
		case LowSpeed:
			// OSPEEDRy = '00'.
			gpioPort -> OSPEEDR &= ~BIT_MASK(2*gpioNum);
			gpioPort -> OSPEEDR &= ~BIT_MASK(2*gpioNum+1);
			break;
		case MediumSpeed:
			// OSPEEDRy = '01'.
			gpioPort -> OSPEEDR |= BIT_MASK(2*gpioNum);
			gpioPort -> OSPEEDR &= ~BIT_MASK(2*gpioNum+1);
			break;
		case HighSpeed:
			// OSPEEDRy = '10'.
			gpioPort -> OSPEEDR &= ~BIT_MASK(2*gpioNum);
			gpioPort -> OSPEEDR |= BIT_MASK(2*gpioNum+1);
			break;
		case VeryHighSpeed:
			// OSPEEDRy = '11'.
			gpioPort -> OSPEEDR |= BIT_MASK(2*gpioNum);
			gpioPort -> OSPEEDR |= BIT_MASK(2*gpioNum+1);
			break;
		default:
			break;
		}
	}
}

/* ENABLE OR DISABLE PULL-UP AND PULL-DOWN RESISTORS ON A GPIO PIN.
 * @param gpio: Pointer to GPIO identifier (port + number).
 * @param pullResistor: Desired configuration ('NoPullUpNoPullDown', 'PullUp', or 'PullDown').
 * @return: None.
 */
void GPIO_SetPullUpPullDown(GPIO_Struct* gpioStruct, GPIO_PullResistor pullResistor) {
	// Extract port and number.
	GPIO_BaseAddress* gpioPort = gpioStruct -> port;
	unsigned int gpioNum = gpioStruct -> num;
	// Ensure GPIO exists.
	if ((gpioNum >= 0) && (gpioNum < GPIO_PER_PORT)) {
		switch(pullResistor) {
		case NoPullUpNoPullDown:
			// PUPDRy = '00'.
			gpioPort -> PUPDR &= ~BIT_MASK(2*gpioNum);
			gpioPort -> PUPDR &= ~BIT_MASK(2*gpioNum+1);
			break;
		case PullUp:
			// PUPDRy = '01'.
			gpioPort -> PUPDR |= BIT_MASK(2*gpioNum);
			gpioPort -> PUPDR &= ~BIT_MASK(2*gpioNum+1);
			break;
		case PullDown:
			// PUPDRy = '10'.
			gpioPort -> PUPDR &= ~BIT_MASK(2*gpioNum);
			gpioPort -> PUPDR |= BIT_MASK(2*gpioNum+1);
			break;
		default:
			break;
		}
	}
}

/* SELECT THE ALTERNATE FUNCTION OF A GPIO PIN (REQUIRES THE MODE 'AlternateFunction').
 * @param gpio: Pointer to GPIO identifier (port + number).
 * @param AFNum: Alternate function number (0 to 15).
 * @return: None.
 */
void GPIO_SetAlternateFunction(GPIO_Struct* gpioStruct, unsigned int AFNum) {
	// Extract port and number.
	GPIO_BaseAddress* gpioPort = gpioStruct -> port;
	unsigned int gpioNum = gpioStruct -> num;
	// Ensure alternate function exists.
	if ((AFNum >= 0) && (AFNum < AF_PER_GPIO)) {
		unsigned int i = 0;
		// Select proper register to set.
		if ((gpioNum >= 0) && (gpioNum < AFRH_OFFSET)) {
			// Set AFRL register: AFRy = 'AFNum'.
			for (i=0 ; i<4 ; i++) {
				if (AFNum & BIT_MASK(i)) {
					// Bit = '1'.
					gpioPort -> AFRL |= BIT_MASK(4*gpioNum+i);
				}
				else {
					// Bit = '0'.
					gpioPort -> AFRL &= ~BIT_MASK(4*gpioNum+i);
				}
			}
		}
		else {
			if ((gpioNum >= AFRH_OFFSET) && (gpioNum < GPIO_PER_PORT)) {
				// Set AFRH register: AFRy = 'AFNum'.
				for (i=0 ; i<4 ; i++) {
					if (AFNum && BIT_MASK(i)) {
						// Bit = '1'.
						gpioPort -> AFRH |= BIT_MASK(4*(gpioNum-AFRH_OFFSET)+i);
					}
					else {
						// Bit = '0'.
						gpioPort -> AFRH &= ~BIT_MASK(4*(gpioNum-AFRH_OFFSET)+i);
					}
				}
			}
		}
	}
}

/* FUNCTION FOR CONFIGURING A GPIO PIN.
 * @param gpio: Pointer to GPIO identifier (port + number).
 * @param mode: Desired mode ('Input', 'Output', 'AlternateFunction' or 'Analog').
 * @param outputType: Desired output ('PushPull' or 'OpenDrain').
 * @param outputSpeed: Desired output speed ('LowSpeed', 'MediumSpeed', 'HighSpeed' or 'VeryHighSpeed').
 * @param pullResistor: Desired configuration ('NoPullUpNoPullDown', 'PullUp', or 'PullDown').
 * @param AFNum: Alternate function number (0 to 15) if 'AlternateFunction' mode is selected.
 */
void GPIO_Configure(GPIO_Struct* gpioStruct, GPIO_Mode mode, GPIO_OutputType outputType, GPIO_OutputSpeed outputSpeed, GPIO_PullResistor pullResistor, unsigned int AFNum) {
	GPIO_SetMode(gpioStruct, mode);
	if (mode == AlternateFunction) {
		GPIO_SetAlternateFunction(gpioStruct, AFNum);
	}
	GPIO_SetOutputType(gpioStruct, outputType);
	GPIO_SetOutputSpeed(gpioStruct, outputSpeed);
	GPIO_SetPullUpPullDown(gpioStruct, pullResistor);
}

/*** GPIO functions ***/

/* CONFIGURE MCU GPIOs.
 * @param: None.
 * @return: None.
 */
void GPIO_Init(void) {

	// Enable all GPIOx clocks.
	RCC -> AHB1ENR |= 0x000007FF; // Enable GPIOx clock (GPIOxEN = '1').

	// Debug.
	GPIO_Configure(LED1, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(LED2, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(LED3, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(LED4, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(BUTTON, Input, OpenDrain, LowSpeed, PullUp, 0);
	// DAC.
	GPIO_Configure(AM_OUTPUT, Analog, OpenDrain, LowSpeed, NoPullUpNoPullDown, 0);
	// ADC.
	GPIO_Configure(ZPT, Analog, OpenDrain, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(PBL2, Analog, OpenDrain, LowSpeed, NoPullUpNoPullDown, 0);
	// SGKCU UART pins configured as AF7 for using USART2.
	GPIO_Configure(SGKCU_TX, AlternateFunction, OpenDrain, LowSpeed, NoPullUpNoPullDown, 7);
	GPIO_Configure(SGKCU_RX, AlternateFunction, OpenDrain, LowSpeed, NoPullUpNoPullDown, 7);
	// KVB.
	GPIO_Configure(KVB_ZSA, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(KVB_ZSB, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(KVB_ZSC, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(KVB_ZSD, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(KVB_ZSE, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(KVB_ZSF, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(KVB_ZSG, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(KVB_ZJG, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(KVB_ZJC, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(KVB_ZJD, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(KVB_ZVG, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(KVB_ZVC, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);
	GPIO_Configure(KVB_ZVD, Output, PushPull, LowSpeed, NoPullUpNoPullDown, 0);

#ifdef OUTPUT_CLOCK
	// MCO1 configured as AF0 to output HSI clock.
	GPIO_Configure(MCO1, AlternateFunction, PushPull, VeryHighSpeed, NoPullUpNoPullDown, 0);
	// MCO2 configured as AF0 to output SYSCLK clock.
	GPIO_Configure(MCO2, AlternateFunction, PushPull, VeryHighSpeed, NoPullUpNoPullDown, 0);
#endif
}

/* SET THE STATE OF A GPIO.
 * @param gpioStruct: Pointer to GPIO identifier (port + number).
 * @param state: Desired state of the pin ('LOW' or 'HIGH').
 * @return: None.
 */
void GPIO_Write(GPIO_Struct* gpioStruct, GPIO_State state) {
	// Extract port and number.
	GPIO_BaseAddress* gpioPort = gpioStruct -> port;
	unsigned int gpioNum = gpioStruct -> num;
	// Ensure GPIO exists.
	if ((gpioNum >= 0) && (gpioNum < GPIO_PER_PORT)) {
		switch (state) {
		case LOW:
			gpioPort -> ODR &= ~BIT_MASK(gpioNum);
			break;
		case HIGH:
			gpioPort -> ODR |= BIT_MASK(gpioNum);
			break;
		default:
			break;
		}
	}
}

/* READ THE STATE OF A GPIO.
 * @param gpioStruct: Pointer to GPIO identifier (port + number).
 * @return: GPIO state ('LOW' or 'HIGH').
 */
GPIO_State GPIO_Read(GPIO_Struct* gpioStruct) {
	// Extract port and number.
	GPIO_BaseAddress* gpioPort = gpioStruct -> port;
	unsigned int gpioNum = gpioStruct -> num;
	GPIO_State result = LOW;
	if ((gpioNum >= 0) && (gpioNum < GPIO_PER_PORT)) {
		switch (GPIO_GetMode(gpioStruct)) {
		case Input:
			// GPIO configured as input -> read IDR register.
			result = (((gpioPort -> IDR) & BIT_MASK(gpioNum)) >> gpioNum) + GPIO_STATE_ENUM_OFFSET;
			break;
		case Output:
			// GPIO configured as output -> read ODR register.
			result = (((gpioPort -> ODR) & BIT_MASK(gpioNum)) >> gpioNum) + GPIO_STATE_ENUM_OFFSET;
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
	unsigned int gpioNum = gpioStruct -> num;
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
