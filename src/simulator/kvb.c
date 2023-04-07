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
#include "sw2.h"
#include "tim.h"
#include "stdint.h"

/*** KVB local macros ***/

// KVB segments.
#define KVB_NUMBER_OF_SEGMENTS 		7 		// 7 segments.
#define KVB_NUMBER_OF_DISPLAYS 		6 		// KVB has 6 displays (3 yellow and 3 green).
// LVAL.
#define KVB_LVAL_BLINK_PERIOD_MS	900		// Period of LVAL blinking (in ms).
// LSSF.
#define KVB_LSSF_BLINK_PERIOD_MS	333		// Period of LSSF blinking (in ms).
// GPIO bits mask.
#define KVB_GPIO_MASK				0xFFFFC080
// Display messages.
#define KVB_YG_PA400				((uint8_t*) "PA 400")
#define KVB_YG_UC512				((uint8_t*) "UC 512")
#define KVB_YG_888					((uint8_t*) "888888")
#define KVB_YG_DASH					((uint8_t*) "------")
#define KVB_G_B						((uint8_t*) "    b ")
#define KVB_Y_B						((uint8_t*) " b    ")
#define KVB_G_P						((uint8_t*) "    P ")
#define KVB_Y_P						((uint8_t*) " P    ")
#define KVB_G_L						((uint8_t*) "    L ")
#define KVB_Y_L						((uint8_t*) " L    ")
#define KVB_G_00					((uint8_t*) "    00")
#define KVB_Y_00					((uint8_t*) " 00   ")
#define KVB_G_000					((uint8_t*) "   000")
#define KVB_Y_000					((uint8_t*) "000   ")
// Initialization screens duration.
#define KVB_PA400_DURATION_MS		2200
#define KVB_PA400_OFF_DURATION_MS	2200
#define KVB_UC512_DURATION_MS		2000
#define KVB_888888_DURATION_MS		3000
// Security parameters.
#define KVB_SPEED_LIMIT_MARGIN_KMH	5

/*** KVB local structures ***/

typedef enum {
	KVB_STATE_OFF,
	KVB_STATE_PA400,
	KVB_STATE_PA400_OFF,
	KVB_STATE_UC512,
	KVB_STATE_888888,
	KVB_STATE_WAIT_VALIDATION,
	KVB_STATE_IDLE,
	KVB_STATE_URGENCY
} KVB_State;

// KVB context.
typedef struct KVB_context_t {
	// State machine.
	KVB_State state;
	uint32_t state_switch_time_ms;
	// Buttons.
	SW2_context_t bpval;
	SW2_context_t bpmv;
	SW2_context_t bpfc;
	SW2_context_t bpat;
	SW2_context_t bpsf;
	SW2_context_t acsf;
	// Each display state is coded in a byte: <dot G F E D B C B A>.
	// A '1' bit means the segment is on, a '0' means the segment is off.
	uint8_t ascii_buf[KVB_NUMBER_OF_DISPLAYS];
	volatile uint32_t segment_buf[KVB_NUMBER_OF_DISPLAYS];
	volatile uint8_t display_idx; // Yellow left to green right.
	// Flags to enable LVAL and LSSF blinking.
	uint8_t lval_blink_enable;
	uint8_t lval_blinking;
	uint8_t lssf_blink_enable;
} KVB_context_t;

/*** KVB external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** KVB local global variables ***/

static KVB_context_t kvb_ctx;
static const GPIO* segment_gpio_buf[KVB_NUMBER_OF_SEGMENTS] = {&GPIO_KVB_ZSA, &GPIO_KVB_ZSB, &GPIO_KVB_ZSC, &GPIO_KVB_ZSD, &GPIO_KVB_ZSE, &GPIO_KVB_ZSF, &GPIO_KVB_ZSG};
static const GPIO* display_gpio_buf[KVB_NUMBER_OF_DISPLAYS] = {&GPIO_KVB_ZJG, &GPIO_KVB_ZJC, &GPIO_KVB_ZJD, &GPIO_KVB_ZVG, &GPIO_KVB_ZVC, &GPIO_KVB_ZVD};

/*** KVB local functions ***/

/* RETURNS THE SEGMENT CONFIGURATION TO DISPLAY A GIVEN ASCII CHARACTER.
 * @param ascii:	ASCII code of the input character.
 * @param segment:	The corresponding segment configuration, coded as <dot G F E D B C B A>.
 * 					0 (all segments off) if the input character is unknown or can't be displayed with 7 segments.
 */
static uint8_t _KVB_ascii_to_7_segments(uint8_t ascii) {
	uint8_t segment = 0;
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
static void _KVB_blink_lval(void) {
	// TBC: add time offset to start at 0%.
	uint32_t t = TIM2_get_milliseconds() % KVB_LVAL_BLINK_PERIOD_MS;
	uint32_t lvalDutyCycle = 0;
	// Triangle wave equation.
	if (t <= (KVB_LVAL_BLINK_PERIOD_MS / 2)) {
		lvalDutyCycle = (200 * t) / KVB_LVAL_BLINK_PERIOD_MS;
	}
	else {
		lvalDutyCycle = 200 - ((200 * t) / KVB_LVAL_BLINK_PERIOD_MS);
	}
	// Set duty cycle.
	TIM8_set_duty_cycle(lvalDutyCycle);
}

/* COMPUTE THE DUTY CYCLE TO MAKE LVAL BLINK.
 * @param:	None.
 * @return:	None.
 */
static void _KVB_blink_lssf(void) {
	// TBC: add time offset to start at 0.
	uint32_t t = TIM2_get_milliseconds() % KVB_LSSF_BLINK_PERIOD_MS;
	// Square wave equation.
	if (t <= (KVB_LSSF_BLINK_PERIOD_MS / 2)) {
		GPIO_write(&GPIO_KVB_LSSF, 0);
	}
	else {
		GPIO_write(&GPIO_KVB_LSSF, 1);
	}
}

/* FILL KVB ASCII BUFFER FOR FUTURE DISPLAYING.
 * @param display:	String to display (cut if too long, padded with null character if too short).
 * @return:			None.
 */
void _KVB_display(uint8_t* display) {
	// Local variables.
	uint8_t idx = 0;
	// Copy message into ascii_buf.
	while (*display) {
		kvb_ctx.ascii_buf[idx] = *display++;
		idx++;
		if (idx == KVB_NUMBER_OF_DISPLAYS) {
			// Number of displays reached.
			break;
		}
	}
	// Add null characters if necessary.
	for (; idx<KVB_NUMBER_OF_DISPLAYS ; idx++) {
		kvb_ctx.ascii_buf[idx] = 0;
	}
	// Convert ASCII characters to segment configurations.
	for (idx=0 ; idx<KVB_NUMBER_OF_DISPLAYS ; idx++) {
		kvb_ctx.segment_buf[idx] = 0;
		kvb_ctx.segment_buf[idx] = (0b1 << (8 + idx)) | _KVB_ascii_to_7_segments(kvb_ctx.ascii_buf[idx]);
	}
	// Start sweep timer.
	TIM6_start();
}

/* TURN ALL KVB DISPLAYS OFF.
 * @param:	None.
 * @return:	None.
 */
static void _KVB_display_off(void) {
	uint32_t i = 0;
	// Flush buffers and switch off GPIOs.
	for (i=0 ; i<KVB_NUMBER_OF_DISPLAYS ; i++) {
		GPIO_write(display_gpio_buf[i], 0);
		kvb_ctx.ascii_buf[i] = 0;
		kvb_ctx.segment_buf[i] = 0;
	}
	for (i=0 ; i<KVB_NUMBER_OF_SEGMENTS ; i++) {
		GPIO_write(segment_gpio_buf[i], 0);
	}
	// Stop sweep timer.
	TIM6_stop();
}

/* TURN ALL KVB LIGHTS OFF.
 * @param:	None.
 * @return:	None.
 */
static void _KVB_lights_off(void) {
	// Turn all GPIOs off.
	TIM8_stop();
	GPIO_write(&GPIO_KVB_LMV, 0);
	GPIO_write(&GPIO_KVB_LFC, 0);
	GPIO_write(&GPIO_KVB_LV, 0);
	GPIO_write(&GPIO_KVB_LFU, 0);
	GPIO_write(&GPIO_KVB_LPE, 0);
	GPIO_write(&GPIO_KVB_LPS, 0);
	GPIO_write(&GPIO_KVB_LSSF, 0);
	// Update flags.
	kvb_ctx.lval_blink_enable = 0;
	kvb_ctx.lssf_blink_enable = 0;

}

/*** KVB functions ***/

/* INITIALISE KVB MODULE.
 * @param:	None.
 * @return:	None.
 */
void KVB_init(void) {
	// Init 7-segments displays.
	uint32_t idx = 0;
	for (idx=0 ; idx<KVB_NUMBER_OF_SEGMENTS ; idx++) GPIO_configure(segment_gpio_buf[idx], GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	for (idx=0 ; idx<KVB_NUMBER_OF_DISPLAYS ; idx++) GPIO_configure(display_gpio_buf[idx], GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init lights (except LVAL configured in TIM8 driver).
	GPIO_configure(&GPIO_KVB_LMV, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_KVB_LFC, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_KVB_LV, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_KVB_LFU, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_KVB_LPE, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_KVB_LPS, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_KVB_LSSF, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init buttons.
	SW2_init(&kvb_ctx.bpval, &GPIO_KVB_BPVAL, 0, 100); // BPVAL active low.
	SW2_init(&kvb_ctx.bpmv, &GPIO_KVB_BPMV, 0, 100); // BPMV active low.
	SW2_init(&kvb_ctx.bpfc, &GPIO_KVB_BPFC, 0, 100); // BPFC active low.
	SW2_init(&kvb_ctx.bpat, &GPIO_KVB_BPAT, 0, 100); // BPAT active low.
	SW2_init(&kvb_ctx.bpsf, &GPIO_KVB_BPSF, 0, 100); // BPSF active low.
	SW2_init(&kvb_ctx.acsf, &GPIO_KVB_ACSF, 0, 100); // ACSF active low.
	// Init context.
	kvb_ctx.state = KVB_STATE_OFF;
	kvb_ctx.state_switch_time_ms = 0;
	for (idx=0 ; idx<KVB_NUMBER_OF_DISPLAYS ; idx++) {
		kvb_ctx.ascii_buf[idx] = 0;
		kvb_ctx.segment_buf[idx] = 0;
	}
	kvb_ctx.display_idx = 0;
	kvb_ctx.lssf_blink_enable = 0;
	kvb_ctx.lval_blink_enable = 0;
	kvb_ctx.lval_blinking = 0;
	// Init global context.
	lsmcu_ctx.speed_limit_kmh = 0;
}

/* PROCESS KVB DISPLAY (CALLED BY TIM6 INTERRUPT HANDLER EVERY 2ms).
 * @param:	None.
 * @return:	None.
 */
void __attribute__((optimize("-O0"))) KVB_sweep(void) {
	// Read register.
	volatile uint32_t reg_value = (GPIOG -> ODR);
	// Set bits.
	reg_value &= KVB_GPIO_MASK;
	reg_value |= kvb_ctx.segment_buf[kvb_ctx.display_idx];
	// Write register.
	GPIOG -> ODR = reg_value;
	// Increment and manage index.
	kvb_ctx.display_idx = (kvb_ctx.display_idx + 1) % KVB_NUMBER_OF_DISPLAYS;
}

/* MAIN ROUTINE OF KVB.
 * @param:	None.
 * @return:	None.
 */
void KVB_task(void) {
	// Update buttons state.
	SW2_update_state(&kvb_ctx.bpval);
	SW2_update_state(&kvb_ctx.bpmv);
	SW2_update_state(&kvb_ctx.bpfc);
	SW2_update_state(&kvb_ctx.bpat);
	SW2_update_state(&kvb_ctx.bpsf);
	SW2_update_state(&kvb_ctx.acsf);
	// Perform internal state machine.
	switch (kvb_ctx.state) {
	case KVB_STATE_OFF:
		if (lsmcu_ctx.bl_unlocked != 0) {
			// Start KVB init.
			_KVB_display(KVB_YG_PA400);
			kvb_ctx.lssf_blink_enable = 1;
			kvb_ctx.state = KVB_STATE_PA400;
			kvb_ctx.state_switch_time_ms = TIM2_get_milliseconds();
		}
		break;
	case KVB_STATE_PA400:
		// Wait PA400 display duration.
		if (TIM2_get_milliseconds() > (kvb_ctx.state_switch_time_ms + KVB_PA400_DURATION_MS)) {
			_KVB_display_off();
			GPIO_write(&GPIO_KVB_LPS, 1);
			kvb_ctx.state = KVB_STATE_PA400_OFF;
			kvb_ctx.state_switch_time_ms = TIM2_get_milliseconds();
		}
		break;
	case KVB_STATE_PA400_OFF:
		// Wait transition duration.
		if (TIM2_get_milliseconds() > (kvb_ctx.state_switch_time_ms + KVB_PA400_OFF_DURATION_MS)) {
			_KVB_display(KVB_YG_UC512);
			GPIO_write(&GPIO_KVB_LPS, 0);
			kvb_ctx.state = KVB_STATE_UC512;
			kvb_ctx.state_switch_time_ms = TIM2_get_milliseconds();
		}
		break;
	case KVB_STATE_UC512:
		// Wait for UC512 display duration.
		if (TIM2_get_milliseconds() > (kvb_ctx.state_switch_time_ms + KVB_UC512_DURATION_MS)) {
			_KVB_display(KVB_YG_888);
			GPIO_write(&GPIO_KVB_LMV, 1);
			GPIO_write(&GPIO_KVB_LFC, 1);
			TIM8_start();
			kvb_ctx.lval_blink_enable = 1;
			kvb_ctx.state = KVB_STATE_888888;
			kvb_ctx.state_switch_time_ms = TIM2_get_milliseconds();
		}
		break;
	case KVB_STATE_888888:
		// Wait for 888888 display duration.
		if (TIM2_get_milliseconds() > (kvb_ctx.state_switch_time_ms + KVB_888888_DURATION_MS)) {
			_KVB_display_off();
			GPIO_write(&GPIO_KVB_LMV, 0);
			GPIO_write(&GPIO_KVB_LFC, 0);
			kvb_ctx.state = KVB_STATE_WAIT_VALIDATION;
			kvb_ctx.state_switch_time_ms = TIM2_get_milliseconds();
		}
		break;
	case KVB_STATE_WAIT_VALIDATION:
		// Check BPVAL.
		if (kvb_ctx.bpval.state == SW2_ON) {
			// Parameters validated, go to idle state.
			TIM8_stop();
			kvb_ctx.lval_blink_enable = 0;
		}
		if ((kvb_ctx.bpsf.state == SW2_ON) || (kvb_ctx.acsf.state == SW2_ON)) {
			kvb_ctx.lssf_blink_enable = 0;
			GPIO_write(&GPIO_KVB_LSSF, 0);
		}
		if ((kvb_ctx.lssf_blink_enable == 0) && (kvb_ctx.lval_blink_enable == 0)) {
			// KVB ready.
			kvb_ctx.state = KVB_STATE_IDLE;
		}
		break;
	case KVB_STATE_IDLE:
		// Speed check.
		if (lsmcu_ctx.speed_kmh > (lsmcu_ctx.speed_limit_kmh + KVB_SPEED_LIMIT_MARGIN_KMH)) {
			// Trigger urgency brake.
			lsmcu_ctx.urgency = 1;
			kvb_ctx.state = KVB_STATE_URGENCY;
		}
		break;
	case KVB_STATE_URGENCY:
		break;
	default:
		break;
	}
	// Force OFF state if BL is locked.
	if (lsmcu_ctx.bl_unlocked == 0) {
		_KVB_display_off();
		_KVB_lights_off();
		kvb_ctx.state = KVB_STATE_OFF;
	}
	// Manage LVAL and LSSF blinking.
	if (kvb_ctx.lssf_blink_enable != 0) {
		_KVB_blink_lssf();
	}
	if (kvb_ctx.lval_blink_enable != 0) {
		_KVB_blink_lval();
	}
}
