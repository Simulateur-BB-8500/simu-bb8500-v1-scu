/*
 * tim.c
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludovic
 */

#include "dac.h"
#include "enum.h"
#include "gpio.h"
#include "mapping.h"
#include "nvic.h"
#include "nvic_reg.h"
#include "rcc_reg.h"
#include "tim.h"
#include "tim_reg.h"
#include "usart.h"

/*** TIM #define ***/

// Timers couple used ti count milliseconds since MCU start-up.
#define TIM_MS_MASTER	TIM1 // Only TIM1 to TIM8.
#define TIM_MS_SLAVE	TIM2 // All timer allowed except TIM6 and TIM7.

/*** TIM internal function ***/

/* RETURN THE CORRESPONDING INTERRUPT INDEX OF A GIVEN TIMER.
 * @param tim: 			Timer base address (should be 'TIM1' to 'TIM14).
 * @return itNumber:	The corresponding IT number in NVIC.
 */
unsigned int TIM_GetIT(TIM_BaseAddress* TIM) {
	unsigned int itNumber;
	// Check peripheral address.
	switch ((unsigned int) TIM) {
	case ((unsigned int) TIM1):
		itNumber = IT_TIM1_UP_TIM10;
		break;
	case ((unsigned int) TIM2):
		itNumber = IT_TIM2;
		break;
	case ((unsigned int) TIM3):
		itNumber = IT_TIM3;
		break;
	case ((unsigned int) TIM4):
		itNumber = IT_TIM4;
		break;
	case ((unsigned int) TIM5):
		itNumber = IT_TIM5;
		break;
	case ((unsigned int) TIM6):
		itNumber = IT_TIM6_DAC;
		break;
	case ((unsigned int) TIM7):
		itNumber = IT_TIM7;
		break;
	case ((unsigned int) TIM8):
		itNumber = IT_TIM8_UP_TIM13;
		break;
	case ((unsigned int) TIM9):
		itNumber = IT_TIM1_BRK_TIM9;
		break;
	case ((unsigned int) TIM10):
		itNumber = IT_TIM1_UP_TIM10;
		break;
	case ((unsigned int) TIM11):
		itNumber = IT_TIM1_TRG_COM_TIM11;
		break;
	case ((unsigned int) TIM12):
		itNumber = IT_TIM8_BRK_TIM12;
		break;
	case ((unsigned int) TIM13):
		itNumber = IT_TIM8_UP_TIM13;
		break;
	case ((unsigned int) TIM14):
		itNumber = IT_TIM8_TRG_COM_TIM14;
		break;
	}
	return itNumber;
}

/* ENABLE THE CORRESPONDING CLOCK OF AN TIM PERIPHERAL.
 * @param TIM: 	TIM base address (should be 'TIM1/2/3/6' or 'TIM4/5/7/8').
 * @return:			None.
 */
void TIM_EnableClock(TIM_BaseAddress* TIM) {
	// Check peripheral address.
	switch ((unsigned int) TIM) {
	case ((unsigned int) TIM1):
		RCC -> APB2ENR |= BIT_MASK(0); // TIM1EN = '1'.
		break;
	case ((unsigned int) TIM2):
		RCC -> APB1ENR |= BIT_MASK(0); // TIM2EN = '1'.
		break;
	case ((unsigned int) TIM3):
		RCC -> APB1ENR |= BIT_MASK(1); // TIM3EN = '1'.
		break;
	case ((unsigned int) TIM4):
		RCC -> APB1ENR |= BIT_MASK(2); // TIM4EN = '1'.
		break;
	case ((unsigned int) TIM5):
		RCC -> APB1ENR |= BIT_MASK(3); // TIM5EN = '1'.
		break;
	case ((unsigned int) TIM6):
		RCC -> APB1ENR |= BIT_MASK(4); // TIM6EN = '1'.
		break;
	case ((unsigned int) TIM7):
		RCC -> APB1ENR |= BIT_MASK(5); // TIM7EN = '1'.
		break;
	case ((unsigned int) TIM8):
		RCC -> APB2ENR |= BIT_MASK(1); // TIM8EN = '1'.
		break;
	case ((unsigned int) TIM9):
		RCC -> APB2ENR |= BIT_MASK(16); // TIM9EN = '1'.
		break;
	case ((unsigned int) TIM10):
		RCC -> APB2ENR |= BIT_MASK(17); // TIM10EN = '1'.
		break;
	case ((unsigned int) TIM11):
		RCC -> APB2ENR |= BIT_MASK(18); // TIM11EN = '1'.
		break;
	case ((unsigned int) TIM12):
		RCC -> APB1ENR |= BIT_MASK(6); // TIM12EN = '1'.
		break;
	case ((unsigned int) TIM13):
		RCC -> APB1ENR |= BIT_MASK(7); // TIM13EN = '1'.
		break;
	case ((unsigned int) TIM14):
		RCC -> APB1ENR |= BIT_MASK(8); // TIM14EN = '1'.
		break;
	}
}

/*** TIM functions ***/

/* CONFIGURE A TIMER.
 * @param TIM: 				Timer address ('TIM1' to 'TIM14').
 * @param duration: 		Timer overflow period :	(1 -> 999) 탎, (1 -> 999) ms and (1 -> 262) s.
 * @param unit: 			Unit of parameter 'duration' ('탎', 'ms' or 's').
 * @param interruptEnable: 	Indicate if interrupt is enabled for this timer.
 * @return: 				None.
 */
void TIM_Init(TIM_BaseAddress* TIM, unsigned int duration, Time_Unit unit, boolean interruptEnable) {
	// Enable peripheral clock.
	TIM_EnableClock(TIM);
	// Stop and reset.
	TIM_Stop(TIM, true);
	// Disable interrupt.
	TIM -> DIER &= ~BIT_MASK(0); // UIE = '0'.
	// Clear update event flag (SR = '0').
	TIM_ClearFlag(TIM);
	// Set PSC and ARR registers to reach duration.
	TIM_SetDuration(TIM, duration, unit);
	// Enable interrupt.
	TIM -> DIER |= BIT_MASK(0); // UIE = '1'.
	if (interruptEnable) {
		NVIC_EnableInterrupt(TIM_GetIT(TIM));
	}
}

/* SET ARR AND PSC REGISTER TO REACH A DURATION.
 * @param TIM: 			Timer address ('TIM1' to 'TIM14').
 * @param duration: 	Timer overflow period :	(1 -> 999) 탎, (1 -> 999) ms and (1 -> 262) s.
 * @param unit: 		Unit of parameter 'duration' ('탎', 'ms' or 's').
 */
void TIM_SetDuration(TIM_BaseAddress* TIM, unsigned int duration, Time_Unit unit) {
	// Select prescaler and counter value depending on unit.
	switch(unit) {
	case microseconds:
		TIM -> PSC = 1; // Timer clock = SYSCLK/(1+1) = 8MHz.
		// Ensure duration is reachable.
		if ((duration >= TIM_MIN_MICROSECONDS) && (duration <= TIM_MAX_MICROSECONDS)) {
			TIM -> ARR = 8*duration; /* 8 fronts at 8MHz = 1 탎. */
		}
		break;
	case milliseconds:
		TIM -> PSC = 1599; // Timer clock = SYSCLK/(1599+1) = 10kHz.
		// Ensure duration is reachable.
		if ((duration >= TIM_MIN_MILLISECONDS) && (duration <= TIM_MAX_MILLISECONDS)) {
			TIM -> ARR = 10*duration; // 10 fronts at 10kHz = 1 ms.
		}
		break;
	case seconds:
		TIM -> PSC = 63999; // Timer clock = SYSCLK/(63999+1) = 250Hz.
		// Ensure duration is reachable.
		if ((duration >= TIM_MIN_SECONDS) && (duration <= TIM_MAX_SECONDS)) {
			TIM -> ARR = 250*duration; // 250 fronts at 250Hz = 1 s.
		}
		break;
	default:
		// Unknown unit.
		break;
	}
}

/* START A TIMER.
 * @param TIM: 		Timer address ('TIM1' to 'TIM14').
 * @param reset: 	'true' = reset counter to 0.
 * 					'false' = keep counter current value.
 * @return: 		None.
 */
void TIM_Start(TIM_BaseAddress* TIM, boolean reset) {
	TIM -> CR1 |= BIT_MASK(0); // Enable counter (CEN = '1').
	if (reset) {
		TIM -> CNT = 0;
	}
}

/* STOP A TIMER.
 * @param TIM: 		Timer address ('TIM1' to 'TIM14').
 * @param reset: 	'true' = reset counter to 0.
 * 					'false' = keep counter current value.
 * @return: 		None.
 */
void TIM_Stop(TIM_BaseAddress* TIM, boolean reset) {
	TIM -> CR1 &= ~BIT_MASK(0); // Disable counter (CEN = '0').
	if (reset) {
		TIM -> CNT = 0;
	}
}

/* CLEAR A TIMER UPDATE EVENT FLAG.
 * @param TIM: Timer address ('TIM1' to 'TIM14').
 * @return: None.
 */
void TIM_ClearFlag(TIM_BaseAddress* TIM) {
	TIM -> SR &= ~BIT_MASK(0); // Clear flag (UIF = '0').
}

/* CONFIGURE A TIMER IN PWM MODE.
 * @param TIM: 				Timer address ('TIM1' to 'TIM14').
 * @param period: 			PWM period :	(1 -> 999) 탎, (1 -> 999) ms and (1 -> 262) s.
 * @param unit: 			Unit of parameter 'period' ('탎', 'ms' or 's').
 * @return: 				None.
 */
void PWM_Init(TIM_BaseAddress* TIM, unsigned int period, Time_Unit unit) {
	// TBC: check if selected timer has PWM feature and the required registers.
	TIM_Init(TIM, period, unit, false);
	TIM -> DIER &= ~BIT_MASK(0); // UIE = '0'.
	// Configure all channels in PWM mode 1.
	TIM -> CCMR1 &= 0xFEFEEFEF; // 0CxM = '0xx0'.
	TIM -> CCMR1 |= 0x00006868; // 0CxM = '0110' and OCxPE = '1'.
	TIM -> CCMR2 &= 0xFEFEEFEF; // 0CxM = '0xx0'.
	TIM -> CCMR2 |= 0x00006868; // 0CxM = '0110' and OCxPE = '1'.
	// All channels in output mode.
	TIM -> CCMR1 &= 0xFFFFFCFC; // CCxS = '00'.
	TIM -> CCMR2 &= 0xFFFFFCFC; // CCxS = '00'.
	// Enable output compare.
	TIM -> CCER |= 0x00001111; // CCxE = '1'.
	// Reset registers.
	TIM -> EGR |= BIT_MASK(0); // UG = '1'.
	// Enable output.
	TIM -> BDTR |= BIT_MASK(15); // MOE = '1'.
}

/* SET THE DUTY CYCLE OF A PWM CHANNEL.
 * @param TIM: 			Timer address ('TIM1' to 'TIM14').
 * @param dutyCycke: 	PWM duty cycle in % (0 to 100).
 */
void PWM_SetDutyCycle(TIM_BaseAddress* TIM, PWM_Channel pwmChannel, unsigned int dutyCycle) {
	unsigned int realDutyCycle = (((dutyCycle) % 101)*(TIM -> ARR))/100; // % 101 because dutyCycle ranges from 0 to 100 included.
	// TBC: check if pwmChannel exists for this timer.
	switch (pwmChannel) {
	case PWM_Channel1:
		TIM -> CCR1 = realDutyCycle;
		break;
	case PWM_Channel2:
		TIM -> CCR2 = realDutyCycle;
		break;
	case PWM_Channel3:
		TIM -> CCR3 = realDutyCycle;
		break;
	case PWM_Channel4:
		TIM -> CCR4 = realDutyCycle;
		break;
	}
}

/* START A PWM.
 * @param TIM: 		Timer address ('TIM1' to 'TIM14').
 * @param reset: 	'true' = reset counter to 0.
 * 					'false' = keep counter current value.
 * @return: 		None.
 */
void PWM_Start(TIM_BaseAddress* TIM, boolean reset) {
	TIM -> CR1 |= BIT_MASK(0); // Enable counter (CEN = '1').
	if (reset) {
		TIM -> CNT = 0;
	}
}

/* STOP A PWM.
 * @param TIM: 		Timer address ('TIM1' to 'TIM14').
 * @param reset: 	'true' = reset counter to 0.
 * 					'false' = keep counter current value.
 * @return: 		None.
 */
void PWM_Stop(TIM_BaseAddress* TIM, boolean reset) {
	TIM -> CR1 &= ~BIT_MASK(0); // Disable counter (CEN = '0').
	if (reset) {
		TIM -> CNT = 0;
	}
}

/* CONFIGURE TIM13 AND TIM14 TO COUNT MILLISECONDS SINCE START-UP.
 * @param:	None.
 * @return:	None.
 */
void TIM_InitMs(void) {
	// Configure master timer.
	TIM_Init(TIM_MS_MASTER, 1, milliseconds, false); // Overflows every millisecond.
	// Master mode and update interrupt selected as trigger output.
	TIM_MS_MASTER -> CR2 &= 0xFFFFFFAF; // MMS = '0x0'.
	TIM_MS_MASTER -> CR2 |= BIT_MASK(5); // MMS = '010'.
	// Enable slave timer clock.
	TIM_EnableClock(TIM_MS_SLAVE);
	// Configure slave timer.
	TIM_MS_SLAVE -> CNT = 0;
	TIM_MS_SLAVE -> SMCR &= 0xFFFFFF8F; // TS = '000' to select ITR0 = TIM1 as trigger input.
	TIM_MS_SLAVE -> SMCR |= 0x00000007; // SMS = '111' to enable slave mode with external clock.
	// Start timers.
	TIM_Start(TIM_MS_SLAVE, true);
	TIM_Start(TIM_MS_MASTER, true);
}

/* RETURNS THE NUMBER OF MILLISECONDS ELLAPSED SINCE START-UP.
 * @param: 					None.
 * @return milliseconds: 	Number of milliseconds (32-bits word) ellapsed since start-up.
 */
unsigned int TIM_GetMs(void) {
	return (TIM_MS_SLAVE -> CNT);
}

/* DELAY FUNCTION.
 * @param msToWait:	Number of milliseconds to wait.
 * @return:			None.
 */
void TIM_DelayMs(unsigned int msToWait) {
	unsigned int now = TIM_GetMs();
	while (TIM_GetMs() < (now + msToWait));
}
