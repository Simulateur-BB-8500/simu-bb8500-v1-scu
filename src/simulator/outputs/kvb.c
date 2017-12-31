/*
 * kvb.c
 *
 *  Created on: 26 dec. 2017
 *      Author: Ludovic
 */

#include "enum.h"
#include "gpio.h"
#include "kvb.h"
#include "mapping.h"
#include "tim.h"
#include "tim_reg.h"

/*** KVB #define ***/

// KVB segments.
#define NUMBER_OF_SEGMENTS 			8 		// 7 segments + dot.
#define NUMBER_OF_DISPLAYS 			6 		// KVB has 6 7-segments displays (3 yellow and 3 green).
#define TIM_KVB 					TIM6 	// Address of timer used for KVB displaying.
#define KVB_SWEEP_MS				2 		// Sweep period in ms.
// Display durations in ms.
#define KVB_PA400_TEXT				((unsigned char*) "PA 400")
#define KVB_PA400_DURATION_MS		2000
#define KVB_PA400_OFF_DURATION_MS	2000
#define KVB_UC512_TEXT				((unsigned char*) "UC 512")
#define KVB_UC512_DURATION_MS		2000
#define KVB_888888_TEXT				((unsigned char*) "888888")
#define KVB_888888_DURATION_MS		3000
// KVB LVAL
#define TIM_LVAL					TIM8			// Address of timer used for LVAL light PWM.
#define CHANNEL_LVAL				PWM_Channel1	// PWM channel.
#define T_PWM						900				// PWM period in µs.
#define KVB_LVAL_BLINK_PERIOD_MS	900				// Period of LVAL blinking (in ms).

/*** KVB global variables ***/

// Each display state is coded in a byte: <dot G F E D B C B A>.
// A '1' bit means the segment is on, a '0' means the segment is off.
static volatile unsigned char asciiData[NUMBER_OF_DISPLAYS];
static volatile unsigned char segmentsData[NUMBER_OF_DISPLAYS];
static volatile unsigned char displayIndex = 0;
static volatile unsigned char segmentIndex = 0;
static GPIO_Struct* segmentsGpio[NUMBER_OF_SEGMENTS] = {KVB_ZSA, KVB_ZSB, KVB_ZSC, KVB_ZSD, KVB_ZSE, KVB_ZSF, KVB_ZSG, KVB_ZD};
static GPIO_Struct* displaysGpio[NUMBER_OF_DISPLAYS] = {KVB_ZJG, KVB_ZJC, KVB_ZJD, KVB_ZVG, KVB_ZVC, KVB_ZVD};
// KVB state machine.
static KVB_State kvbState = KVB_OFF;
static unsigned int kvbSwitchStateTime = 0; // In ms.

/*** KVB internal functions ***/

/* RETURNS THE SEGMENT CONFIGURATION TO DISPLAY A GIVEN ASCII CHARACTER.
 * @param ascii:	ASCII code of the input character.
 * @param segment:	The corresponding segment configuration, coded as <dot G F E D B C B A>.
 * 					0 (all segments off) if the input character is unknown or can't be displayed with 7 segments.
 */
unsigned char KVB_AsciiTo7Segments(unsigned char ascii) {
	unsigned char segment = 0;
	switch (ascii) {
	case 'b':
		segment = 0b01111100;
		break;
	case 'c':
		segment = 0b01011000;
		break;
	case 'd':
		segment = 0b01011110;
		break;
	case 'h':
		segment = 0b01110100;
		break;
	case 'n':
		segment = 0b01010100;
		break;
	case 'o':
		segment = 0b01011100;
		break;
	case 'r':
		segment = 0b01010000;
		break;
	case 't':
		segment = 0b01111000;
		break;
	case 'u':
		segment = 0b00011100;
		break;
	case 'A':
		segment = 0b01110111;
		break;
	case 'C':
		segment = 0b00111001;
		break;
	case 'E':
		segment = 0b01111001;
		break;
	case 'F':
		segment = 0b01110001;
		break;
	case 'H':
		segment = 0b01110110;
		break;
	case 'J':
		segment = 0b00001110;
		break;
	case 'L':
		segment = 0b00111000;
		break;
	case 'P':
		segment = 0b01110011;
		break;
	case 'U':
		segment = 0b00111110;
		break;
	case 'Y':
		segment = 0b01101110;
		break;
	case '0':
		segment = 0b00111111;
		break;
	case '1':
		segment = 0b00000110;
		break;
	case '2':
		segment = 0b01011011;
		break;
	case '3':
		segment = 0b01001111;
		break;
	case '4':
		segment = 0b01100110;
		break;
	case '5':
		segment = 0b01101101;
		break;
	case '6':
		segment = 0b01111101;
		break;
	case '7':
		segment = 0b00000111;
		break;
	case '8':
		segment = 0b01111111;
		break;
	case '9':
		segment = 0b01101111;
		break;
	case '-':
		segment = 0b01000000;
		break;
	default:
		segment = 0;
		break;
	}
	return segment;
}

/*** KVB functions ***/

/* INITIALISE KVB MODULE.
 * @param:	None.
 * @return:	None.
 */
void KVB_Init(void) {
	// Init display arrays.
	unsigned int i = 0;
	for (i=0 ; i<NUMBER_OF_DISPLAYS ; i++) {
		asciiData[i] = 0;
		segmentsData[i] = 0;
	}
	// Init KVB GPIOs.
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
	GPIO_Configure(KVB_LVAL, AlternateFunction, PushPull, HighSpeed, NoPullUpNoPullDown, 3); // TIM8_CH1.
	// Init and start KVB timer.
	TIM_Init(TIM_KVB, KVB_SWEEP_MS, milliseconds, true);
	TIM_Start(TIM_KVB, true);
	// Init and start LVAL PWM timer.
	PWM_Init(TIM_LVAL, T_PWM, microseconds);
	PWM_SetDutyCycle(TIM_LVAL, CHANNEL_LVAL, 0);
	PWM_Start(TIM_LVAL, true);
}

/* DISPLAY A STRING ON KVB PANEL.
 * @param display:	String to display (cut if too long, padded with null character is too short).
 * @return:			None.
 */
void KVB_Display(unsigned char* display) {
	unsigned char charIndex = 0;
	// Copy message into asciiData.
	while (*display) {
		asciiData[charIndex] = *display++;
		charIndex++;
		if (charIndex == NUMBER_OF_DISPLAYS) {
			// Number of displays reached.
			break;
		}
	}
	// Add null characters if necessary.
	for (; charIndex<NUMBER_OF_DISPLAYS ; charIndex++) {
		asciiData[charIndex] = 0;
	}
	// Convert ASCII characters to segment configurations.
	for (charIndex=0 ; charIndex<NUMBER_OF_DISPLAYS ; charIndex++) {
		segmentsData[charIndex] = KVB_AsciiTo7Segments(asciiData[charIndex]);
	}
}

/* COMPUTE THE DUTY CYCLE TO MAKE LVAL BLINK.
 * @param:	None.
 * @return:	None.
 */
void KVB_BlinkLVAL(void) {
	// TBC: add time offset to start at 0%.
	unsigned int t = TIM_GetMs() % KVB_LVAL_BLINK_PERIOD_MS;
	unsigned int lvalDutyCycle = 0;
	// Triangle wave equation.
	if (t <= (KVB_LVAL_BLINK_PERIOD_MS/2)) {
		lvalDutyCycle = (200*t)/KVB_LVAL_BLINK_PERIOD_MS;
	}
	else {
		lvalDutyCycle = 200-((200*t)/KVB_LVAL_BLINK_PERIOD_MS);
	}
	// Set duty cycle.
	PWM_SetDutyCycle(TIM_LVAL, CHANNEL_LVAL, lvalDutyCycle);
}

/* SWITCH OFF ALL KVB PANEL.
 * @param:	None.
 * @return:	None.
 */
void KVB_DisplayOff(void) {
	unsigned int i = 0;
	// Flush buffers and switch off GPIOs.
	for (i=0 ; i<NUMBER_OF_DISPLAYS ; i++) {
		GPIO_Write(displaysGpio[i], LOW);
		asciiData[i] = 0;
		segmentsData[i] = 0;
	}
	for (i=0 ; i<NUMBER_OF_SEGMENTS ; i++) {
		GPIO_Write(segmentsGpio[i], LOW);
	}
}

/* MAIN ROUTINE OF KVB.
 * @param blUnlocked:	Indicates if the BL is unlocked ('true') or not ('false').
 * @return:				None.
 */
void KVB_Routine(boolean blUnlocked) {
	switch (kvbState) {
	case KVB_OFF:
		KVB_DisplayOff();
		if (blUnlocked == true) {
			kvbState = KVB_PA400;
			kvbSwitchStateTime = TIM_GetMs();
		}
		break;
	case KVB_PA400:
		KVB_Display(KVB_PA400_TEXT);
		if (blUnlocked == false) {
			kvbState = KVB_OFF;
		}
		else {
			if (TIM_GetMs() > (kvbSwitchStateTime + KVB_PA400_DURATION_MS)) {
				kvbState = KVB_PA400_OFF;
				kvbSwitchStateTime = TIM_GetMs();
			}
		}
		break;
	case KVB_PA400_OFF:
		KVB_DisplayOff();
		if (blUnlocked == false) {
			kvbState = KVB_OFF;
		}
		else {
			if (TIM_GetMs() > (kvbSwitchStateTime + KVB_PA400_OFF_DURATION_MS)) {
				kvbState = KVB_UC512;
				kvbSwitchStateTime = TIM_GetMs();
			}
		}
		break;
	case KVB_UC512:
		KVB_Display(KVB_UC512_TEXT);
		if (blUnlocked == false) {
			kvbState = KVB_OFF;
		}
		else {
			if (TIM_GetMs() > (kvbSwitchStateTime + KVB_UC512_DURATION_MS)) {
				kvbState = KVB_888888;
				kvbSwitchStateTime = TIM_GetMs();
			}
		}
		break;
	case KVB_888888:
		KVB_BlinkLVAL();
		KVB_Display(KVB_888888_TEXT);
		if (blUnlocked == false) {
			kvbState = KVB_OFF;
		}
		else {
			if (TIM_GetMs() > (kvbSwitchStateTime + KVB_888888_DURATION_MS)) {
				kvbState = KVB_888888_OFF;
				kvbSwitchStateTime = TIM_GetMs();
			}
		}
		break;
	case KVB_888888_OFF:
		KVB_BlinkLVAL();
		KVB_DisplayOff();
		if (blUnlocked == false) {
			kvbState = KVB_OFF;
		}
		else {
			kvbState = KVB_SLAVE_MODE;
		}
		break;
	case KVB_SLAVE_MODE:
		KVB_BlinkLVAL();
		// TBC.
		break;
	default:
		break;
	}
}

/* KVB TIMER INTERRUPT HANDLER.
 * @param: 	None.
 * @return: None.
 */
void TIM_KVB_Handler(void) {
	TIM_Stop(TIM_KVB, true);
	TIM_ClearFlag(TIM_KVB);
	// Switch off previous display.
	GPIO_Write(displaysGpio[displayIndex], LOW);
	// Increment and manage index.
	displayIndex++;
	if (displayIndex > (NUMBER_OF_DISPLAYS-1)) {
		displayIndex = 0;
	}
	// Process display only if a character is present.
	if (segmentsData[displayIndex] != 0) {
		// Switch on and off the segments of the current display.
		for (segmentIndex=0 ; segmentIndex<NUMBER_OF_SEGMENTS ; segmentIndex++) {
			if ((segmentsData[displayIndex] & BIT_MASK(segmentIndex)) != 0) {
				GPIO_Write(segmentsGpio[segmentIndex], HIGH);
			}
			else {
				GPIO_Write(segmentsGpio[segmentIndex], LOW);
			}
		}
		// Finally switch on current display.
		GPIO_Write(displaysGpio[displayIndex], HIGH);
	}
	TIM_Start(TIM_KVB, true);
}
