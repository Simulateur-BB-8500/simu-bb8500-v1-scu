/*
 * tim.h
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_TIME_TIM_H_
#define PERIPHERALS_TIME_TIM_H_

#include "masks.h"
#include "tim_reg.h"

/*** TIM #define ***/

#define TIM_MIN_MICROSECONDS 1
#define TIM_MAX_MICROSECONDS 999
#define TIM_MIN_MILLISECONDS 1
#define TIM_MAX_MILLISECONDS 999
#define TIM_MIN_SECONDS 1
#define TIM_MAX_SECONDS 262 // 263*250 > 2^(16)-1.

/* TIM functions */

/* CLEAR A TIMER UPDATE EVENT FLAG.
 * #define is required to make the function generic, because timers have different register maps and hence distinct types.
 * This function is thus defined in the header file to be visible.
 * @param TIM: Timer address.
 * @return: None.
 */
#define TIM_ClearFlag(TIM) ({ \
	TIM -> SR &= ~BIT_MASK[0]; /* Clear flag (UIF = '0'). */ \
})

/* CONFIGURE A TIMER.
 * #define is required to make the function generic, because timers have different register maps and hence distinct types.
 * This function is thus defined in the header file to be visible.
 * @param TIM: Timer address.
 * @param duration: Timer overflow period :	(1 -> 999) µs, (1 -> 999) ms and (1 -> 262) s.
 * @param unit: Unit of parameter 'duration' ('µs', 'ms' or 's').
 * @return: None.
 */
#define TIM_Set(TIM, duration, unit) ({ \
	/* Clear update event flag (SR = '0'). */ \
	TIM_ClearFlag(TIM); \
	/* Select prescaler and counter value depending on unit. */ \
	switch(unit) { \
	case microseconds: \
		TIM -> PSC = 1; /* Timer clock = SYSCLK/(1+1) = 8MHz. */ \
		/* Ensure duration is reachable. */ \
		if ((duration >= TIM_MIN_MICROSECONDS) && (duration <= TIM_MAX_MICROSECONDS)) { \
			TIM -> ARR = 8*duration; /* 8 fronts at 8MHz = 1 µs. */ \
		} \
		break; \
	case milliseconds: \
		TIM -> PSC = 1599; /* Timer clock = SYSCLK/(1599+1) = 10kHz. */ \
		/* Ensure duration is reachable. */ \
		if ((duration >= TIM_MIN_MILLISECONDS) && (duration <= TIM_MAX_MILLISECONDS)) { \
			TIM -> ARR = 10*duration; /* 10 fronts at 10kHz = 1 ms. */ \
		} \
		break; \
	case seconds: \
		TIM -> PSC = 63999; /* Timer clock = SYSCLK/(63999+1) = 250Hz. */ \
		/* Ensure duration is reachable. */ \
		if ((duration >= TIM_MIN_SECONDS) && (duration <= TIM_MAX_SECONDS)) { \
			TIM -> ARR = 250*duration; /* 250 fronts at 250Hz = 1 s. */ \
		} \
		break; \
	default: \
		break; \
	} \
})

/* ENABLE A TIMER INTERRUPT.
 * #define is required to make the function generic, because timers have different register maps and hence distinct types.
 * This function is thus defined in the header file to be visible.
 * @param TIM: Timer address.
 * @return: None.
 */
#define TIM_EnableInterrupt(TIM) ({ \
	TIM -> DIER |= BIT_MASK[0]; /* Enable interrupt (UIE = '1'). */ \
})

/* START A TIMER.
 * #define is required to make the function generic, because timers have different register maps and hence distinct types.
 * This function is thus defined in the header file to be visible.
 * @param TIM: Timer address.
 * @param reset: 	'true' = reset counter to 0.
 * 					'false' = keep counter current value.
 * @return: None.
 */
#define TIM_Start(TIM, reset) ({ \
	TIM -> CR1 |= BIT_MASK[0]; /* Enable counter (CEN = '1'). */ \
	if (reset) { \
		TIM -> CNT = 0; \
	} \
})

/* STOP A TIMER.
 * #define is required to make the function generic, because timers have different register maps and hence distinct types.
 * This function is thus defined in the header file to be visible.
 * @param TIM: Timer address.
 * @param reset: 	'true' = reset counter to 0.
 * 					'false' = keep counter current value.
 * @return: None.
 */
#define TIM_Stop(TIM, reset) ({ \
	TIM -> CR1 &= ~BIT_MASK[0]; /* Disable counter (CEN = '0'). */ \
	if (reset) { \
		TIM -> CNT = 0; \
	} \
})

#endif /* PERIPHERALS_TIME_TIM_H_ */
