/*
 * tim.c
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludovic
 */

#include "dac.h"
#include "gpio.h"
#include "mapping.h"
#include "masks.h"
#include "tim.h"
#include "tim_reg.h"
#include "usart.h"
#include "usart_reg.h"

/*** TIM functions ***/

/* CONFIGURE A TIMER.
 * @param TIM: Timer address ('TIM1' to 'TIM14').
 * @param duration: Timer overflow period :	(1 -> 999) µs, (1 -> 999) ms and (1 -> 262) s.
 * @param unit: Unit of parameter 'duration' ('µs', 'ms' or 's').
 * @return: None.
 */
void TIM_Set(TIM_BaseAddress* TIM, unsigned int duration, TimeUnit unit) {
	// Stop and reset.
	TIM_Stop(TIM, true);
	// Clear update event flag (SR = '0').
	TIM_ClearFlag(TIM);
	/* Select prescaler and counter value depending on unit. */
	switch(unit) {
	case microseconds:
		TIM -> PSC = 1; /* Timer clock = SYSCLK/(1+1) = 8MHz. */
		/* Ensure duration is reachable. */
		if ((duration >= TIM_MIN_MICROSECONDS) && (duration <= TIM_MAX_MICROSECONDS)) {
			TIM -> ARR = 8*duration; /* 8 fronts at 8MHz = 1 µs. */
		}
		break;
	case milliseconds:
		TIM -> PSC = 1599; /* Timer clock = SYSCLK/(1599+1) = 10kHz. */
		/* Ensure duration is reachable. */
		if ((duration >= TIM_MIN_MILLISECONDS) && (duration <= TIM_MAX_MILLISECONDS)) {
			TIM -> ARR = 10*duration; /* 10 fronts at 10kHz = 1 ms. */
		}
		break;
	case seconds:
		TIM -> PSC = 63999; /* Timer clock = SYSCLK/(63999+1) = 250Hz. */
		/* Ensure duration is reachable. */
		if ((duration >= TIM_MIN_SECONDS) && (duration <= TIM_MAX_SECONDS)) {
			TIM -> ARR = 250*duration; /* 250 fronts at 250Hz = 1 s. */
		}
		break;
	default:
		break;
	}
}

/* START A TIMER.
 * @param TIM: Timer address ('TIM1' to 'TIM14').
 * @param reset: 	'true' = reset counter to 0.
 * 					'false' = keep counter current value.
 * @return: None.
 */
void TIM_Start(TIM_BaseAddress* TIM, boolean reset) {
	TIM -> CR1 |= BIT_MASK[0]; // Enable counter (CEN = '1').
	if (reset) {
		TIM -> CNT = 0;
	}
}

/* STOP A TIMER.
 * @param TIM: Timer address ('TIM1' to 'TIM14').
 * @param reset: 	'true' = reset counter to 0.
 * 					'false' = keep counter current value.
 * @return: None.
 */
void TIM_Stop(TIM_BaseAddress* TIM, boolean reset) {
	TIM -> CR1 &= ~BIT_MASK[0]; // Disable counter (CEN = '0').
	if (reset) {
		TIM -> CNT = 0;
	}
}

/* ENABLE A TIMER INTERRUPT.
 * @param TIM: Timer address ('TIM1' to 'TIM14').
 * @return: None.
 */
void TIM_EnableInterrupt(TIM_BaseAddress* TIM) {
	TIM -> DIER |= BIT_MASK[0]; // Enable interrupt (UIE = '1').
}

/* CLEAR A TIMER UPDATE EVENT FLAG.
 * @param TIM: Timer address ('TIM1' to 'TIM14').
 * @return: None.
 */
void TIM_ClearFlag(TIM_BaseAddress* TIM) {
	TIM -> SR &= ~BIT_MASK[0]; // Clear flag (UIF = '0').
}

/* CONFIGURE TIM1 AND TIM2 TO COUNT MILLISECONDS SINCE START-UP.
 * @param:	None.
 * @return:	None.
 */
void TIM_InitMs(void) {
	// Configure TIM1 as master.
	TIM_Set(TIM1, 1, milliseconds); // Overflows every millisecond.
	TIM_EnableInterrupt(TIM1);
	// Master mode and update interrupt selected as trigger output.
	TIM1 -> CR2 &= 0xFFFFFFAF; // MMS = '0x0'.
	TIM1 -> CR2 |= BIT_MASK[5]; // MMS = '010'.
	// Configure TIM2 as slave.
	TIM2 -> CNT = 0;
	TIM2 -> SMCR &= 0xFFFFFF8F; // TS = '000' to select ITR0 = TIM1 as trigger input.
	TIM2 -> SMCR |= 0x00000007; // SMS = '111' to enable slave mode with external clock.
	// Start timers.
	TIM_Start(TIM2, true);
	TIM_Start(TIM1, true);
}

/* RETURNS THE NUMBER OF MILLISECONDS ELLAPSED SINCE START-UP.
 * @param: None.
 * @return milliseconds: number of milliseconds (32-bits word) ellapsed since start-up.
 */
unsigned int TIM_GetMs(void) {
	return (TIM2 -> CNT);
}

/* DELAY FUNCTION.
 * @param msToWait:	Number of milliseconds to wait.
 * @return:			None.
 */
void TIM_DelayMs(unsigned int msToWait) {
	unsigned int now = TIM_GetMs();
	while (TIM_GetMs() < (now + msToWait));
}

/*** TIM interrupt handlers ***/

/* TIM6 INTERRUPT HANDLER.
 * @param: None.
 * @return: None.
 */
void TIM6_Handler(void) {
	USART_Send(USART2, 0x46);
	GPIO_Toggle(LED1);
	TIM_ClearFlag(TIM6);
}

/* TIM7 INTERRUPT HANDLER.
 * @param: None.
 * @return: None.
 */
void TIM7_Handler(void) {
	TIM_ClearFlag(TIM7);
	// Get DAC current voltage.
	unsigned int currentVoltage = DAC_GetVoltage(DACChannel1);
	if (currentVoltage == DAC_FULL_SCALE) {
		currentVoltage = 0;
	}
	// Increase DAC output voltage.
	currentVoltage++;
	DAC_SetVoltage(DACChannel1, currentVoltage);
}
