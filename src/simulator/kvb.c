/*
 * kvb.c
 *
 *  Created on: 26 dec. 2017
 *      Author: Ludo
 */

#include "kvb.h"

#include "gpio.h"
#include "lsmcu.h"
#include "mapping.h"
#include "tim.h"

/*** KVB local macros ***/

// KVB segments.
#define KVB_NUMBER_OF_SEGMENTS 		8 		// 7 segments + dot.
#define KVB_NUMBER_OF_DISPLAYS 		6 		// KVB has 6 displays (3 yellow and 3 green).
#define KVB_DISPLAY_SWEEP_MS		2 		// Display sweep period in ms.
// LVAL.
#define KVB_LVAL_BLINK_PERIOD_MS	900		// Period of LVAL blinking (in ms).
// LSSF.
#define KVB_LSSF_BLINK_PERIOD_MS	333		// Period of LSSF blinking (in ms).

/*** KVB local structures ***/

// KVB context.
typedef struct KVB_Context {
	unsigned char ascii_buf[KVB_NUMBER_OF_DISPLAYS];
	// Each display state is coded in a byte: <dot G F E D B C B A>.
	// A '1' bit means the segment is on, a '0' means the segment is off.
	unsigned char segment_buf[KVB_NUMBER_OF_DISPLAYS];
	unsigned char segment_idx; // A to dot.
	unsigned char display_idx; // yel0 left to green right.
	// Flags to enable LVAL and LSSF blinking.
	unsigned char lval_blink_enable;
	unsigned char lval_blinking;
	unsigned char lssf_blink_enable;
} KVB_Context;

/*** KVB external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** KVB local global variables ***/

static KVB_Context kvb_ctx;
static const GPIO* segment_gpio_buf[KVB_NUMBER_OF_SEGMENTS] = {&GPIO_KVB_ZSA, &GPIO_KVB_ZSB, &GPIO_KVB_ZSC, &GPIO_KVB_ZSD, &GPIO_KVB_ZSE, &GPIO_KVB_ZSF, &GPIO_KVB_ZSG, &GPIO_KVB_ZDOT};
static const GPIO* display_gpio_buf[KVB_NUMBER_OF_DISPLAYS] = {&GPIO_KVB_ZJG, &GPIO_KVB_ZJC, &GPIO_KVB_ZJD, &GPIO_KVB_ZVG, &GPIO_KVB_ZVC, &GPIO_KVB_ZVD};

/*** KVB local functions ***/

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

/* COMPUTE THE DUTY CYCLE TO MAKE LVAL BLINK.
 * @param:	None.
 * @return:	None.
 */
void KVB_BlinkLVAL(void) {
	// TBC: add time offset to start at 0%.
	unsigned int t = TIM2_GetMs() % KVB_LVAL_BLINK_PERIOD_MS;
	unsigned int lvalDutyCycle = 0;
	// Triangle wave equation.
	if (t <= (KVB_LVAL_BLINK_PERIOD_MS / 2)) {
		lvalDutyCycle = (200 * t) / KVB_LVAL_BLINK_PERIOD_MS;
	}
	else {
		lvalDutyCycle = 200 - ((200 * t) / KVB_LVAL_BLINK_PERIOD_MS);
	}
	// Set duty cycle.
	TIM8_SetDutyCycle(lvalDutyCycle);
}

/* COMPUTE THE DUTY CYCLE TO MAKE LVAL BLINK.
 * @param:	None.
 * @return:	None.
 */
void KVB_BlinkLSSF(void) {
	// TBC: add time offset to start at 0.
	unsigned int t = TIM2_GetMs() % KVB_LSSF_BLINK_PERIOD_MS;
	// Square wave equation.
	if (t <= (KVB_LSSF_BLINK_PERIOD_MS / 2)) {
		GPIO_Write(&GPIO_KVB_LSSF, 0);
	}
	else {
		GPIO_Write(&GPIO_KVB_LSSF, 1);
	}
}

/*** KVB functions ***/

/* INITIALISE KVB MODULE.
 * @param:	None.
 * @return:	None.
 */
void KVB_Init(void) {
	// Init GPIOs.
	unsigned int idx = 0;
	for (idx=0 ; idx<KVB_NUMBER_OF_SEGMENTS ; idx++) GPIO_Configure(segment_gpio_buf[idx], GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	for (idx=0 ; idx<KVB_NUMBER_OF_DISPLAYS ; idx++) GPIO_Configure(display_gpio_buf[idx], GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// LVAL is configured in TIM8_Init() function since it is linked to TIM8 channel 1.
	GPIO_Configure(&GPIO_KVB_LSSF, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init context.
	for (idx=0 ; idx<KVB_NUMBER_OF_DISPLAYS ; idx++) {
		kvb_ctx.ascii_buf[idx] = 0;
		kvb_ctx.segment_buf[idx] = 0;
	}
	kvb_ctx.display_idx = 0;
	kvb_ctx.segment_idx = 0;
	kvb_ctx.lssf_blink_enable = 1;
	kvb_ctx.lval_blink_enable = 0;
	kvb_ctx.lval_blinking = 0;
	// Init global context.
	lsmcu_ctx.urgency = 0;
}

/* ENABLE KVB DISPLAY SWEEP TIMER.
 * @param:	None.
 * @return:	None.
 */
void KVB_StartSweepTimer(void) {
	// Start sweep timer.
	TIM6_Start();
}

/* DISABLE KVB DISPLAY SWEEP TIMER.
 * @param:	None.
 * @return:	None.
 */
void KVB_StopSweepTimer(void) {
	// Stop sweep timer.
	TIM6_Stop();
}

/* FILL KVB ASCII BUFFER FOR FUTURE DISPLAYING.
 * @param display:	String to display (cut if too long, padded with null character if too short).
 * @return:			None.
 */
void KVB_Display(unsigned char* display) {
	unsigned char charIndex = 0;
	// Copy message into ascii_buf.
	while (*display) {
		kvb_ctx.ascii_buf[charIndex] = *display++;
		charIndex++;
		if (charIndex == KVB_NUMBER_OF_DISPLAYS) {
			// Number of displays reached.
			break;
		}
	}
	// Add null characters if necessary.
	for (; charIndex<KVB_NUMBER_OF_DISPLAYS ; charIndex++) {
		kvb_ctx.ascii_buf[charIndex] = 0;
	}
	// Convert ASCII characters to segment configurations.
	for (charIndex=0 ; charIndex<KVB_NUMBER_OF_DISPLAYS ; charIndex++) {
		kvb_ctx.segment_buf[charIndex] = KVB_AsciiTo7Segments(kvb_ctx.ascii_buf[charIndex]);
	}
}

/* SWITCH OFF ALL KVB PANEL.
 * @param:	None.
 * @return:	None.
 */
void KVB_DisplayOff(void) {
	unsigned int i = 0;
	// Flush buffers and switch off GPIOs.
	for (i=0 ; i<KVB_NUMBER_OF_DISPLAYS ; i++) {
		GPIO_Write(display_gpio_buf[i], 0);
		kvb_ctx.ascii_buf[i] = 0;
		kvb_ctx.segment_buf[i] = 0;
	}
	for (i=0 ; i<KVB_NUMBER_OF_SEGMENTS ; i++) {
		GPIO_Write(segment_gpio_buf[i], 0);
	}
}

/* ENABLE OR DISABLE LVAL BLINKING.
 * @param blinkEnabled:		New state.
 * @return:					None.
 */
void KVB_EnableBlinkLVAL(unsigned char blink_enabled) {
	kvb_ctx.lval_blink_enable = blink_enabled;
}

/* ENABLE OR DISABLE LSSF BLINKING.
 * @param blinkEnabled:		New state.
 * @return:					None.
 */
void KVB_EnableBlinkLSSF(unsigned char blink_enabled) {
	kvb_ctx.lssf_blink_enable = blink_enabled;
}

/* PROCESS KVB DISPLAY (CALLED BY TIM6 INTERRUPT HANDLER EVERY 2ms).
 * @param:	None.
 * @return:	None.
 */
void KVB_Sweep(void) {
	// Switch off previous display.
	GPIO_Write(display_gpio_buf[kvb_ctx.display_idx], 0);
	// Increment and manage index.
	kvb_ctx.display_idx++;
	if (kvb_ctx.display_idx > (KVB_NUMBER_OF_DISPLAYS-1)) {
		kvb_ctx.display_idx = 0;
	}
	// Process display only if a character is present.
	if (kvb_ctx.segment_buf[kvb_ctx.display_idx] != 0) {
		// Switch on and off the segments of the current display.
		for (kvb_ctx.segment_idx=0 ; kvb_ctx.segment_idx<KVB_NUMBER_OF_SEGMENTS ; kvb_ctx.segment_idx++) {
			GPIO_Write(segment_gpio_buf[kvb_ctx.segment_idx], kvb_ctx.segment_buf[kvb_ctx.display_idx] & (0b1 << kvb_ctx.segment_idx));
		}
		// Finally switch on current display.
		GPIO_Write(display_gpio_buf[kvb_ctx.display_idx], 1);
	}
}

/* MAIN ROUTINE OF KVB.
 * @param:	None.
 * @return:	None.
 */
void KVB_Task(void) {
	// LVAL.
	if (kvb_ctx.lval_blink_enable != 0) {
		if (kvb_ctx.lval_blinking == 0) {
			TIM8_Start();
			kvb_ctx.lval_blinking = 1;
		}
		KVB_BlinkLVAL();
	}
	else {
		TIM8_Stop();
		kvb_ctx.lval_blinking = 0;
	}
	// LSSF
	if (kvb_ctx.lssf_blink_enable != 0) {
		KVB_BlinkLSSF();
	}
}
