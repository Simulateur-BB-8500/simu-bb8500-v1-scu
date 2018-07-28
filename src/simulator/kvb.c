/*
 * kvb.c
 *
 *  Created on: 26 dec. 2017
 *      Author: Ludovic
 */

#include "gpio.h"
#include "kvb.h"
#include "mapping.h"
#include "tim.h"

/*** KVB local macros ***/

// KVB segments.
#define KVB_NUMBER_OF_SEGMENTS 		8 		// 7 segments + dot.
#define KVB_NUMBER_OF_DISPLAYS 		6 		// KVB has 6 displays (3 yellow and 3 green).
#define KVB_DISPLAY_SWEEP_MS		2 		// Display sweep period in ms.
// Display durations in ms.
#define KVB_PA400_DURATION_MS		2000
#define KVB_PA400_OFF_DURATION_MS	2000
#define KVB_UC512_DURATION_MS		2000
#define KVB_888888_DURATION_MS		3000
// LVAL.
#define KVB_LVAL_BLINK_PERIOD_MS	900		// Period of LVAL blinking (in ms).
// LSSF.
#define KVB_LSSF_BLINK_PERIOD_MS	333		// Period of LSSF blinking (in ms).

/*** KVB local structures ***/

// Internal state machine.
typedef enum {
	KVB_STATE_OFF,
	KVB_STATE_PA400,
	KVB_STATE_PA400_OFF,
	KVB_STATE_UC512,
	KVB_STATE_888888,
	KVB_STATE_888888_OFF,
	KVB_STATE_SLAVE_MODE
} KVB_InternalState;

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
	// State machine.
	KVB_InternalState kvb_internal_state;
	unsigned int switch_state_time; // In ms.
} KVB_Context;

/*** KVB global variables ***/

static KVB_Context kvb_ctx;
static GPIO_Periph segment_gpio_buf[KVB_NUMBER_OF_SEGMENTS] = {KVB_ZSA_GPIO, KVB_ZSB_GPIO, KVB_ZSC_GPIO, KVB_ZSD_GPIO, KVB_ZSE_GPIO, KVB_ZSF_GPIO, KVB_ZSG_GPIO, KVB_ZD_GPIO};
static GPIO_Periph display_gpio_buf[KVB_NUMBER_OF_DISPLAYS] = {KVB_ZJG_GPIO, KVB_ZJC_GPIO, KVB_ZJD_GPIO, KVB_ZVG_GPIO, KVB_ZVC_GPIO, KVB_ZVD_GPIO};

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
	if (t <= (KVB_LVAL_BLINK_PERIOD_MS/2)) {
		lvalDutyCycle = (200*t)/KVB_LVAL_BLINK_PERIOD_MS;
	}
	else {
		lvalDutyCycle = 200-((200*t)/KVB_LVAL_BLINK_PERIOD_MS);
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
	if (t <= (KVB_LSSF_BLINK_PERIOD_MS/2)) {
		GPIO_Write(KVB_LSSF_GPIO, 0);
	}
	else {
		GPIO_Write(KVB_LSSF_GPIO, 1);
	}
}

/*** KVB functions ***/

/* INITIALISE KVB MODULE.
 * @param:	None.
 * @return:	None.
 */
void KVB_Init(void) {

	/* Init context */

	// Init display arrays.
	unsigned int i = 0;
	for (i=0 ; i<KVB_NUMBER_OF_DISPLAYS ; i++) {
		kvb_ctx.ascii_buf[i] = 0;
		kvb_ctx.segment_buf[i] = 0;
	}
	kvb_ctx.display_idx = 0;
	kvb_ctx.segment_idx = 0;
	// Init state machine.
	kvb_ctx.kvb_internal_state = KVB_STATE_OFF;
	kvb_ctx.switch_state_time = 0;
	// Blinks.
	kvb_ctx.lssf_blink_enable = 1;
	kvb_ctx.lval_blink_enable = 0;
	kvb_ctx.lval_blinking = 0;

	/* Init GPIOs */

	for (i=0 ; i<KVB_NUMBER_OF_SEGMENTS ; i++) GPIO_Configure(segment_gpio_buf[i], Output, PushPull, LowSpeed, NoPullUpNoPullDown);
	for (i=0 ; i<KVB_NUMBER_OF_DISPLAYS ; i++) GPIO_Configure(display_gpio_buf[i], Output, PushPull, LowSpeed, NoPullUpNoPullDown);
	// LVAL is configured in TIM8_Init() function since it is linked to TIM8 channel 1.
	GPIO_Configure(KVB_LSSF_GPIO, Output, PushPull, LowSpeed, NoPullUpNoPullDown);

	/* Init timers */

	// Init LVAL PWM timer.
	TIM8_Init();
	// Init and start KVB sweep timer.
	TIM6_Init();
	TIM6_Start();
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

/* PROCESS KVB DISPLAY (CALLED BY TIM6 INTERRUPT HANDLER).
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
 * @param bl_unlocked:	Indicates if the BL is unlocked ('1') or not ('0').
 * @return:				None.
 */
void KVB_Routine(unsigned char bl_unlocked) {
	switch (kvb_ctx.kvb_internal_state) {
	case KVB_STATE_OFF:
		KVB_DisplayOff();
		if (bl_unlocked == 1) {
			kvb_ctx.kvb_internal_state = KVB_STATE_PA400;
			kvb_ctx.switch_state_time = TIM2_GetMs();
		}
		break;
	case KVB_STATE_PA400:
		KVB_Display(KVB_PA400_TEXT);
		KVB_BlinkLSSF();
		if (bl_unlocked == 0) {
			kvb_ctx.kvb_internal_state = KVB_STATE_OFF;
		}
		else {
			if (TIM2_GetMs() > (kvb_ctx.switch_state_time + KVB_PA400_DURATION_MS)) {
				kvb_ctx.kvb_internal_state = KVB_STATE_PA400_OFF;
				kvb_ctx.switch_state_time = TIM2_GetMs();
			}
		}
		break;
	case KVB_STATE_PA400_OFF:
		KVB_DisplayOff();
		KVB_BlinkLSSF();
		if (bl_unlocked == 0) {
			kvb_ctx.kvb_internal_state = KVB_STATE_OFF;
		}
		else {
			if (TIM2_GetMs() > (kvb_ctx.switch_state_time + KVB_PA400_OFF_DURATION_MS)) {
				kvb_ctx.kvb_internal_state = KVB_STATE_UC512;
				kvb_ctx.switch_state_time = TIM2_GetMs();
			}
		}
		break;
	case KVB_STATE_UC512:
		KVB_Display(KVB_UC512_TEXT);
		KVB_BlinkLSSF();
		if (bl_unlocked == 0) {
			kvb_ctx.kvb_internal_state = KVB_STATE_OFF;
		}
		else {
			if (TIM2_GetMs() > (kvb_ctx.switch_state_time + KVB_UC512_DURATION_MS)) {
				kvb_ctx.kvb_internal_state = KVB_STATE_888888;
				kvb_ctx.switch_state_time = TIM2_GetMs();
				// Init LVAL PWM before calling blink function.
				TIM8_Start();
				kvb_ctx.lval_blink_enable = 1;
				kvb_ctx.lval_blinking = 1;
			}
		}
		break;
	case KVB_STATE_888888:
		KVB_Display(KVB_888888_TEXT);
		KVB_BlinkLVAL();
		KVB_BlinkLSSF();
		if (bl_unlocked == 0) {
			kvb_ctx.kvb_internal_state = KVB_STATE_OFF;
		}
		else {
			if (TIM2_GetMs() > (kvb_ctx.switch_state_time + KVB_888888_DURATION_MS)) {
				kvb_ctx.kvb_internal_state = KVB_STATE_888888_OFF;
				kvb_ctx.switch_state_time = TIM2_GetMs();
			}
		}
		break;
	case KVB_STATE_888888_OFF:
		KVB_DisplayOff();
		KVB_BlinkLVAL();
		KVB_BlinkLSSF();
		if (bl_unlocked == 0) {
			kvb_ctx.kvb_internal_state = KVB_STATE_OFF;
		}
		else {
			kvb_ctx.kvb_internal_state = KVB_STATE_SLAVE_MODE;
		}
		break;
	case KVB_STATE_SLAVE_MODE:
		// LVAL.
		if (kvb_ctx.lval_blink_enable == 1) {
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
		if (kvb_ctx.lssf_blink_enable == 1) {
			KVB_BlinkLSSF();
		}
		// All other functions are directly called by the AT manager.
		break;
	default:
		// Unknown state.
		break;
	}
}
