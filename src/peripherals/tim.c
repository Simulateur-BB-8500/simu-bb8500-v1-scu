/*
 * tim.c
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludo
 */

#include "tim.h"

#include "kvb.h"
#include "lsmcu.h"
#include "manometer.h"
#include "mapping.h"
#include "nvic.h"
#include "rcc.h"
#include "rcc_reg.h"
#include "tim_reg.h"

/*** TIM external global variables ***/

extern LSMCU_Context lsmcu_ctx;

/*** TIM local functions ***/

/* TIM6 INTERRUPT HANDLER.
 * @param: 	None.
 * @return: None.
 */
void TIM6_DAC_IRQHandler(void) {
	// Perform KVB display sweep.
	KVB_Sweep();
	// Clear flag.
	TIM6 -> SR &= ~(0b1 << 0); // UIF='0'.
}

/* TIM7 INTERRUPT HANDLER.
 * @param: 	None.
 * @return: None.
 */
void TIM7_IRQHandler(void) {
	// Perform manometers needle control.
	MANOMETER_NeedleTask(lsmcu_ctx.manometer_cp);
	MANOMETER_NeedleTask(lsmcu_ctx.manometer_re);
	MANOMETER_NeedleTask(lsmcu_ctx.manometer_cg);
	MANOMETER_NeedleTask(lsmcu_ctx.manometer_cf1);
	MANOMETER_NeedleTask(lsmcu_ctx.manometer_cf2);
	// Clear flag.
	TIM7 -> SR &= ~(0b1 << 0); // UIF='0'.
}

/*** TIM functions ***/

/* CONFIGURE TIM2 TO COUNT MILLISECONDS SINCE START-UP.
 * @param:	None.
 * @return:	None.
 */
void TIM2_Init(void) {
	// Enable peripheral clock.
	RCC -> APB1ENR |= (0b1 << 0); // TIM2EN='1'.
	// Configure peripheral.
	TIM2 -> CR1 &= ~(0b1 << 0); // CEN='0'.
	TIM2 -> CNT = 0;
	TIM2 -> DIER &= ~(0b1 << 0); // // Disable interrupt (UIE='0').
	TIM2 -> SR &= ~(0b1 << 0); // UIF='0'.
	// Set PSC and ARR registers to reach 1 ms.
	TIM2 -> PSC = (2 * RCC_GetClockFrequency(RCC_CLOCK_PCLK1)) - 1; // TIM2 input clock = (2*PCLK1)/((2*PLCK1-1)+1) = 1kHz.
	TIM2 -> ARR = 0xFFFFFFFF; // No overflow (49 days).
	// Generate event to update registers.
	TIM2 -> EGR |= (0b1 << 0); // UG='1'.
	// Start counter.
	TIM2 -> CR1 |= (0b1 << 0); // CEN='1'.
}

/* RETURNS THE NUMBER OF MILLISECONDS ELLAPSED SINCE START-UP.
 * @param: 	None.
 * @return:	Number of milliseconds (32-bits word) ellapsed since start-up.
 */
unsigned int TIM2_GetMs(void) {
	return (TIM2 -> CNT);
}

/* DELAY FUNCTION.
 * @param msToWait:	Number of milliseconds to wait.
 * @return:			None.
 */
void TIM2_DelayMs(unsigned int ms_to_wait) {
	unsigned int start_ms = TIM2_GetMs();
	while (TIM2_GetMs() < (start_ms + ms_to_wait));
}

/* CONFIGURE TIM5 FOR TACHRO STEPPING.
 * @param:	None.
 * @return:	None.
 */
void TIM5_Init(void) {
	// Enable peripheral clock */
	RCC -> APB1ENR |= (0b1 << 3); // TIM5EN='1'.
	// Configure peripheral.
	TIM5 -> CR1 &= ~(0b1 << 0); // CEN='0'.
	TIM5 -> CNT = 0;
	TIM5 -> DIER &= ~(0b1 << 0); // // Disable interrupt (UIE='0').
	TIM5 -> SR &= ~(0b1 << 0); // UIF='0'.
	// Set PSC and ARR registers to reach 2 ms.
	TIM5 -> PSC = ((2 * RCC_GetClockFrequency(RCC_CLOCK_PCLK1)) / 1000) - 1; // TIM5 input clock = (2*PCLK1)/((((2*PCLK1)/1000)-1)+1) = 1MHz.
	TIM5 -> ARR = 0; // Default value.
	// Generate event to update registers.
	TIM5 -> EGR |= (0b1 << 0); // UG='1'.
}

/* START TIM5.
 * @param:	None.
 * @return: None.
 */
void TIM5_Start(void) {
	// Enable counter.
	TIM5 -> CR1 |= (0b1 << 0); // CEN='1'.
}

/* STOP TIM5.
 * @param: 	None.
 * @return:	None.
 */
void TIM5_Stop(void) {
	// Disable and reset counter.
	TIM5 -> CR1 &= ~(0b1 << 0); // CEN='0'.
	TIM5 -> CNT = 0;
	TIM5 -> SR &= ~(0b1 << 0);
}

/* SET TIM5 ARR REGISTER VALUE TO CHANGE OVERFLOW PERIOD.
 * @param arr_value:	New value of ARR register.
 * @return:				None.
 */
void TIM5_SetDelayUs(unsigned int delay_us) {
	TIM5 -> ARR = delay_us; // TIM5 clock is 1MHz.
}

/* GET TIM5 UPDATE EVENT FLAG.
 * @param:	None.
 * @return:	UIF flag status (0/1).
 */
unsigned char TIM5_GetUifFlag(void) {
	return ((TIM5 -> SR) & (0b1 << 0));
}

/* CLEAR TIM5 UPDATE EVENT FLAG.
 * @param:	None.
 * @return:	None.
 */
void TIM5_ClearUifFlag(void) {
	TIM5 -> SR &= ~(0b1 << 0);
}

/* CONFIGURE TIM6 FOR KVB DISPLAY.
 * @param:	None.
 * @return:	None.
 */
void TIM6_Init(void) {
	// Enable peripheral clock.
	RCC -> APB1ENR |= (0b1 << 4); // TIM6EN='1'.
	// Configure peripheral.
	TIM6 -> CR1 &= ~(0b1 << 0); // CEN='0'.
	TIM6 -> CNT = 0;
	TIM6 -> DIER &= ~(0b1 << 0); // // Disable interrupt (UIE='0').
	TIM6 -> SR &= ~(0b1 << 0); // UIF='0'.
	// Set PSC and ARR registers to reach 2ms.
	TIM6 -> PSC = ((2 * RCC_GetClockFrequency(RCC_CLOCK_PCLK1)) / 50) - 1; // TIM6 input clock = (2*PCLK1)/((((2*PCLK1)/50)-1)+1) = 50kHz.
	TIM6 -> ARR = 100; // 100 fronts @ 50kHz = 2ms.
	// Generate event to update registers.
	TIM6 -> EGR |= (0b1 << 0); // UG='1'.
	// Enable interrupt.
	TIM6 -> DIER |= (0b1 << 0); // UIE='1'.
	TIM6 -> SR &= ~(0b1 << 0); // UIF='0'.
}

/* START TIM6.
 * @param:	None.
 * @return: None.
 */
void TIM6_Start(void) {
	// Enable counter.
	TIM6 -> CR1 |= (0b1 << 0); // CEN='1'.
	NVIC_EnableInterrupt(NVIC_IT_TIM6_DAC);
}

/* STOP TIM6.
 * @param: 	None.
 * @return:	None.
 */
void TIM6_Stop(void) {
	// Disable and reset counter.
	TIM6 -> CR1 &= ~(0b1 << 0); // CEN='0'.
	TIM6 -> CNT = 0;
}

/* CONFIGURE TIM7 FOR MANOMETERS.
 * @param:	None.
 * @return:	None.
 */
void TIM7_Init(void) {
	// Enable peripheral clock.
	RCC -> APB1ENR |= (0b1 << 5); // TIM7EN='1'.
	// Configure peripheral.
	TIM7 -> CR1 &= ~(0b1 << 0); // CEN='0'.
	TIM7 -> CNT = 0;
	TIM7 -> DIER &= ~(0b1 << 0); // // Disable interrupt (UIE='0').
	TIM7 -> SR &= ~(0b1 << 0); // UIF='0'.
	// Set PSC and ARR registers to reach 1ms.
	TIM7 -> PSC = ((2 * RCC_GetClockFrequency(RCC_CLOCK_PCLK1)) / 1000) - 1; // TIM7 input clock = (2*PCLK1)/((((2*PCLK1)/1000)-1)+1) = 1MHz.
	TIM7 -> ARR = 100; // 100 fronts @ 1MHz = 100us.
	// Generate event to update registers.
	TIM7 -> EGR |= (0b1 << 0); // UG='1'.
	// Enable interrupt.
	TIM7 -> DIER |= (0b1 << 0); // UIE='1'.
	TIM7 -> SR &= ~(0b1 << 0); // UIF='0'.
}

/* START TIM7.
 * @param:	None.
 * @return: None.
 */
void TIM7_Start(void) {
	// Enable counter.
	TIM7 -> CR1 |= (0b1 << 0); // CEN='1'.
	NVIC_EnableInterrupt(NVIC_IT_TIM7);
}

/* STOP TIM6.
 * @param: 	None.
 * @return:	None.
 */
void TIM7_Stop(void) {
	// Disable and reset counter.
	TIM7 -> CR1 &= ~(0b1 << 0); // CEN='0'.
	TIM7 -> CNT = 0;
	NVIC_DisableInterrupt(NVIC_IT_TIM7);
}

/* CONFIGURE TIM8 IN PWM MODE FOR LVAL BLINKING.
 * @param:	None.
 * @return: None.
 */
void TIM8_Init(void) {
	// Enable peripheral clock.
	RCC -> APB2ENR |= (0b1 << 1); // TIM8EN='1'.
	// Configure peripheral.
	TIM8 -> CR1 &= ~(0b1 << 0); // CEN='0'.
	TIM8 -> CNT = 0;
	TIM8 -> DIER &= ~(0b1 << 0); // // Disable interrupt (UIE='0').
	TIM8 -> SR &= ~(0b1 << 0); // UIF='0'.
	// Set PSC and ARR registers to set PWM frequency to 2kHz.
	TIM8 -> PSC = ((2 * RCC_GetClockFrequency(RCC_CLOCK_PCLK2)) / 1000) - 1;; // TIM8 input clock = (2*PCLK2)/((((2*PCLK2)/1000)-1)+1) = 1MHz.
	TIM8 -> ARR = 250; // 250 fronts @ 1MHz = 4kHz.
	// Configure channel 1 in PWM mode 1.
	TIM8 -> CCMR1 &= 0xFFFFFF00; // Reset bits 0-7 and output mode (CC1S='00').
	TIM8 -> CCMR1 |= (0b001101000 << 0); // 0C1M='0110' and OC1PE='1'.
	TIM8 -> CCER |= (0b1 << 0); // // Enable channel 1 (CC1E='1').
	// Generate event to update registers.
	TIM8 -> EGR |= (0b1 << 0); // UG='1'.
	// Enable output.
	TIM8 -> BDTR |= (0b1 << 15); // MOE='1'.
}

/* SET TIM8 CHANNEL 1 DUTY CYCLE.
 * @param dutyCycke: 	PWM duty cycle in % (0 to 100).
 * @return:				None;
 */
void TIM8_SetDutyCycle(unsigned char duty_cycle) {
	// Set channel 1 duty cycle.
	TIM8 -> CCR1 = (((duty_cycle) % 101) * (TIM8 -> ARR)) / 100; // % 101 because duty cycle ranges from 0 to 100 included.
}

/* START TIM8.
 * @param:	None.
 * @return: None.
 */
void TIM8_Start(void) {
	// Link GPIO to timer.
	GPIO_Configure(&GPIO_KVB_LVAL, GPIO_MODE_ALTERNATE_FUNCTION, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Enable counter.
	TIM8 -> CR1 |= (0b1 << 0); // CEN='1'.
}

/* STOP TIM8.
 * @param: 	None.
 * @return:	None.
 */
void TIM8_Stop(void) {
	// Configure GPIO as output.
	GPIO_Configure(&GPIO_KVB_LVAL, GPIO_MODE_OUTPUT, GPIO_TYPE_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_NONE);
	// Disable and reset counter.
	TIM8 -> CR1 &= ~(0b1 << 0); // CEN='0'.
	TIM8 -> CNT = 0;
}
