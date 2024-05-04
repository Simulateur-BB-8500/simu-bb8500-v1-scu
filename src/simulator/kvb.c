/*
 * kvb.c
 *
 *  Created on: 26 dec. 2017
 *      Author: Ludo
 */

#include "kvb.h"

#include "emergency.h"
#include "font.h"
#include "gpio.h"
#include "lsmcu.h"
#include "mapping.h"
#include "sw2.h"
#include "tim.h"
#include "stdint.h"
#include "version.h"

/*** KVB local macros ***/

// BPAT.
#define KVB_BPAT_PRESS_DURATION				2000
#define KVB_SELF_TEST_DURATION				2000
// Segments.
#define KVB_NUMBER_OF_SEGMENTS 				7
#define KVB_NUMBER_OF_DISPLAYS 				6
// LVAL.
#define KVB_LVAL_BLINK_PERIOD_MS			900
// LSSF.
#define KVB_LSSF_BLINK_PERIOD_MS			333
// GPIO bits mask.
#define KVB_GPIO_MASK						0xFFFFC080
// Display messages.
#define KVB_YG_PA400						((uint8_t*) "PA 400")
#define KVB_YG_UC512						((uint8_t*) "UC 512")
#define KVB_YG_888							((uint8_t*) "888888")
#define KVB_YG_DASH							((uint8_t*) "------")
#define KVB_G_B								((uint8_t*) "    b ")
#define KVB_Y_B								((uint8_t*) " b    ")
#define KVB_G_P								((uint8_t*) "    P ")
#define KVB_Y_P								((uint8_t*) " P    ")
#define KVB_G_L								((uint8_t*) "    L ")
#define KVB_Y_L								((uint8_t*) " L    ")
#define KVB_G_00							((uint8_t*) "    00")
#define KVB_Y_00							((uint8_t*) " 00   ")
#define KVB_G_000							((uint8_t*) "   000")
#define KVB_Y_000							((uint8_t*) "000   ")
// Initialization screens duration.
#define KVB_PA400_DURATION_MS				2200
#define KVB_PA400_OFF_DURATION_MS			2200
#define KVB_UC512_DURATION_MS				2000
#define KVB_888888_DURATION_MS				3000
// Security parameters.
#define KVB_SPEED_THRESHOLD_LV_KMH			5
#define KVB_SPEED_THRESHOLD_EMERGENCY_KMH	10

/*** KVB local structures ***/

/*******************************************************************/
typedef enum {
	KVB_STATE_OFF,
	KVB_STATE_PA400,
	KVB_STATE_PA400_OFF,
	KVB_STATE_UC512,
	KVB_STATE_888888,
	KVB_STATE_WAIT_VALIDATION,
	KVB_STATE_BPAT_PRESSED,
	KVB_STATE_SELF_TEST,
	KVB_STATE_RUNNING,
	KVB_STATE_EMERGENCY
} KVB_state_t;

/*******************************************************************/
typedef struct KVB_context_t {
	// State machine.
	KVB_state_t state;
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

extern LSMCU_context_t lsmcu_ctx;

/*** KVB local global variables ***/

static KVB_context_t kvb_ctx;
static const GPIO_pin_t* segment_gpio_buf[KVB_NUMBER_OF_SEGMENTS] = {&GPIO_KVB_ZSA, &GPIO_KVB_ZSB, &GPIO_KVB_ZSC, &GPIO_KVB_ZSD, &GPIO_KVB_ZSE, &GPIO_KVB_ZSF, &GPIO_KVB_ZSG};
static const GPIO_pin_t* display_gpio_buf[KVB_NUMBER_OF_DISPLAYS] = {&GPIO_KVB_ZJG, &GPIO_KVB_ZJC, &GPIO_KVB_ZJD, &GPIO_KVB_ZVG, &GPIO_KVB_ZVC, &GPIO_KVB_ZVD};

/*** KVB local functions ***/

/*******************************************************************/
static void _KVB_sweep(void) {
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

/*******************************************************************/
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

/*******************************************************************/
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

/*******************************************************************/
static void _KVB_display(uint8_t* display) {
	// Local variables.
	uint8_t idx = 0;
	uint8_t ascii_code = 0;
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
		ascii_code = kvb_ctx.ascii_buf[idx];
		kvb_ctx.segment_buf[idx] = (ascii_code < FONT_ASCII_TABLE_OFFSET) ? FONT[0] : FONT[ascii_code - FONT_ASCII_TABLE_OFFSET];
		kvb_ctx.segment_buf[idx] |= (0b1 << (8 + idx));
	}
	// Start sweep timer.
	TIM6_start();
}

/*******************************************************************/
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

/*******************************************************************/
static void _KVB_lights_off(void) {
	// Turn all GPIOs off.
	TIM8_stop();
	GPIO_write(&GPIO_KVB_LMV,  0);
	GPIO_write(&GPIO_KVB_LFC,  0);
	GPIO_write(&GPIO_KVB_LV,   0);
	GPIO_write(&GPIO_KVB_LFU,  0);
	GPIO_write(&GPIO_KVB_LPE,  0);
	GPIO_write(&GPIO_KVB_LPS,  0);
	GPIO_write(&GPIO_KVB_LSSF, 0);
	// Update flags.
	kvb_ctx.lval_blink_enable = 0;
	kvb_ctx.lssf_blink_enable = 0;
}

/*** KVB functions ***/

/*******************************************************************/
void KVB_init(void) {
	// Local variables.
	uint8_t idx = 0;
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
	// Init 7-segments displays.
	for (idx=0 ; idx<KVB_NUMBER_OF_SEGMENTS ; idx++) GPIO_configure(segment_gpio_buf[idx], GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	for (idx=0 ; idx<KVB_NUMBER_OF_DISPLAYS ; idx++) GPIO_configure(display_gpio_buf[idx], GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init lights (except LVAL configured in TIM8 driver).
	GPIO_configure(&GPIO_KVB_LMV,  GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_KVB_LFC,  GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_KVB_LV,   GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_KVB_LFU,  GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_KVB_LPE,  GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_KVB_LPS,  GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	GPIO_configure(&GPIO_KVB_LSSF, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init buttons.
	SW2_init(&kvb_ctx.bpval, &GPIO_KVB_BPVAL, 0, 100); // BPVAL active low.
	SW2_init(&kvb_ctx.bpmv,  &GPIO_KVB_BPMV,  0, 100); // BPMV active low.
	SW2_init(&kvb_ctx.bpfc,  &GPIO_KVB_BPFC,  0, 100); // BPFC active low.
	SW2_init(&kvb_ctx.bpat,  &GPIO_KVB_BPAT,  0, 100); // BPAT active low.
	SW2_init(&kvb_ctx.bpsf,  &GPIO_KVB_BPSF,  0, 100); // BPSF active low.
	SW2_init(&kvb_ctx.acsf,  &GPIO_KVB_ACSF,  0, 100); // ACSF active low.
	// Init sounds.
	GPIO_configure(&GPIO_KVB_BIP, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Init sweep and LVAL timers.
	TIM6_init(&_KVB_sweep);
	TIM8_init();
}

/*******************************************************************/
void KVB_process(void) {
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
		// Turn display off.
		_KVB_display_off();
		// Check BL.
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
		// Check BPAT.
		if (kvb_ctx.bpat.state == SW2_ON) {
			// Check press duration.
			kvb_ctx.state = KVB_STATE_BPAT_PRESSED;
			kvb_ctx.state_switch_time_ms = TIM2_get_milliseconds();
		}
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
			kvb_ctx.state = KVB_STATE_RUNNING;
		}
		break;
	case KVB_STATE_BPAT_PRESSED:
		// Check button.
		if (kvb_ctx.bpat.state == SW2_OFF) {
			kvb_ctx.state = KVB_STATE_WAIT_VALIDATION;
		}
		else {
			// Check press duration.
			if (TIM2_get_milliseconds() > (kvb_ctx.state_switch_time_ms + KVB_BPAT_PRESS_DURATION)) {
				// Start self test.
				kvb_ctx.lval_blink_enable = 0;
				TIM8_set_duty_cycle(100);
				GPIO_write(&GPIO_KVB_LMV, 1);
				GPIO_write(&GPIO_KVB_LFC, 1);
				GPIO_write(&GPIO_KVB_LV,  1);
				GPIO_write(&GPIO_KVB_LFU, 1);
				GPIO_write(&GPIO_KVB_LPE, 1);
				GPIO_write(&GPIO_KVB_LPS, 1);
				GPIO_write(&GPIO_KVB_BIP, 1);
				// Switch state.
				kvb_ctx.state = KVB_STATE_SELF_TEST;
				kvb_ctx.state_switch_time_ms = TIM2_get_milliseconds();
			}
		}
		break;
	case KVB_STATE_SELF_TEST:
		if (TIM2_get_milliseconds() > (kvb_ctx.state_switch_time_ms + KVB_SELF_TEST_DURATION)) {
			// Switch lights off.
			kvb_ctx.lval_blink_enable = 1;
			GPIO_write(&GPIO_KVB_LMV, 0);
			GPIO_write(&GPIO_KVB_LFC, 0);
			GPIO_write(&GPIO_KVB_LV,  0);
			GPIO_write(&GPIO_KVB_LFU, 0);
			GPIO_write(&GPIO_KVB_LPE, 0);
			GPIO_write(&GPIO_KVB_LPS, 0);
			GPIO_write(&GPIO_KVB_BIP, 0);
			// Emergency test.
			EMERGENCY_trigger();
			kvb_ctx.state = KVB_STATE_WAIT_VALIDATION;
		}
		break;
	case KVB_STATE_RUNNING:
		// Speed check.
		if (lsmcu_ctx.speed_kmh > (lsmcu_ctx.speed_limit_kmh + KVB_SPEED_THRESHOLD_EMERGENCY_KMH)) {
			// Trigger emergency brake.
			EMERGENCY_trigger();
			kvb_ctx.state = KVB_STATE_EMERGENCY;
		}
		break;
	case KVB_STATE_EMERGENCY:
		// Stay in this state while emergency flag is set.
		if (lsmcu_ctx.emergency == 0) {
			kvb_ctx.state = KVB_STATE_RUNNING;
		}
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
	// LVAL blinking.
	if (kvb_ctx.lssf_blink_enable != 0) {
		_KVB_blink_lssf();
	}
	// LSSF blinking.
	if (kvb_ctx.lval_blink_enable != 0) {
		_KVB_blink_lval();
	}
	// LV.
	if (kvb_ctx.state != KVB_STATE_SELF_TEST) {
		if (lsmcu_ctx.speed_kmh > (lsmcu_ctx.speed_limit_kmh + KVB_SPEED_THRESHOLD_LV_KMH)) {
			GPIO_write(&GPIO_KVB_LV, 1);
		}
		else {
			GPIO_write(&GPIO_KVB_LV, 0);
		}
	}
}

/*******************************************************************/
void KVB_print_software_version(void) {
	// Local variables.
	uint8_t sw_version[KVB_NUMBER_OF_DISPLAYS];
	uint8_t tens = 0;
	// Check dirty flag.
	if (GIT_DIRTY_FLAG != 0) {
		sw_version[0] = 'd';
		sw_version[1] = 'i';
		sw_version[2] = 'r';
		sw_version[3] = 't';
		sw_version[4] = 'y';
		sw_version[5] = ' ';
	}
	else {
		// Build string.
		tens = (GIT_MAJOR_VERSION / 10);
		sw_version[0] = ('0' + tens);
		sw_version[1] = ('0' + (GIT_MAJOR_VERSION - (tens * 10)));
		tens = (GIT_MINOR_VERSION / 10);
		sw_version[2] = ('0' + tens);
		sw_version[3] = ('0' + (GIT_MINOR_VERSION - (tens * 10)));
		tens = (GIT_COMMIT_INDEX / 10);
		sw_version[4] = ('0' + tens);
		sw_version[5] = ('0' + (GIT_COMMIT_INDEX - (tens * 10)));
	}

	// Start display.
	_KVB_display((uint8_t*) sw_version);
}
