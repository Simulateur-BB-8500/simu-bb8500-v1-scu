/*
 * nvic.h
 *
 *  Created on: 16 sep. 2017
 *      Author: Ludo
 */

#ifndef __NVIC_H__
#define __NVIC_H__

#include "stdint.h"

/*** NVIC interrupts vector ***/

/*!******************************************************************
 * \enum NVIC_interrupt_t
 * \brief NVIC interrupt vector.
 *******************************************************************/
typedef enum {
	NVIC_INTERRUPT_WWDG = 0,
	NVIC_INTERRUPT_PVD = 1,
	NVIC_INTERRUPT_TAMP_STAMP = 2,
	NVIC_INTERRUPT_RTC_WKUP = 3,
	NVIC_INTERRUPT_FLASH = 4,
	NVIC_INTERRUPT_RCC = 5,
	NVIC_INTERRUPT_EXTI0 = 6,
	NVIC_INTERRUPT_EXTI1 = 7,
	NVIC_INTERRUPT_EXTI2 = 8,
	NVIC_INTERRUPT_EXTI3 = 9,
	NVIC_INTERRUPT_EXTI4 = 10,
	NVIC_INTERRUPT_DMA_Stream0 = 11,
	NVIC_INTERRUPT_DMA_Stream1 = 12,
	NVIC_INTERRUPT_DMA_Stream2 = 13,
	NVIC_INTERRUPT_DMA_Stream3 = 14,
	NVIC_INTERRUPT_DMA_Stream4 = 15,
	NVIC_INTERRUPT_DMA_Stream5 = 16,
	NVIC_INTERRUPT_DMA_Stream6 = 17,
	NVIC_INTERRUPT_ADC = 18,
	NVIC_INTERRUPT_CAN1_TX = 19,
	NVIC_INTERRUPT_CAN1_RX0 = 20,
	NVIC_INTERRUPT_CAN1_RX1 = 21,
	NVIC_INTERRUPT_CAN1_SCE = 22,
	NVIC_INTERRUPT_EXTI9_5 = 23,
	NVIC_INTERRUPT_TIM1_BRK_TIM9 = 24,
	NVIC_INTERRUPT_TIM1_UP_TIM10 = 25,
	NVIC_INTERRUPT_TIM1_TRG_COM_TIM11 = 26,
	NVIC_INTERRUPT_TIM1_CC = 27,
	NVIC_INTERRUPT_TIM2 = 28,
	NVIC_INTERRUPT_TIM3 = 29,
	NVIC_INTERRUPT_TIM4 = 30,
	NVIC_INTERRUPT_I2C1_EV = 31,
	NVIC_INTERRUPT_I2C1_ER = 32,
	NVIC_INTERRUPT_I2C2_EV = 33,
	NVIC_INTERRUPT_I2C2_ER = 34,
	NVIC_INTERRUPT_SPI1 = 35,
	NVIC_INTERRUPT_SPI2 = 36,
	NVIC_INTERRUPT_USART1 = 37,
	NVIC_INTERRUPT_USART2 = 38,
	NVIC_INTERRUPT_USART3 = 39,
	NVIC_INTERRUPT_EXTI15_10 = 40,
	NVIC_INTERRUPT_RTC_Alarm = 41,
	NVIC_INTERRUPT_OTG_FS_WKUP = 42,
	NVIC_INTERRUPT_TIM8_BRK_TIM12 = 43,
	NVIC_INTERRUPT_TIM8_UP_TIM13 = 44,
	NVIC_INTERRUPT_TIM8_TRG_COM_TIM14 = 45,
	NVIC_INTERRUPT_TIM8_CC = 46,
	NVIC_INTERRUPT_DMA1_Stream7 = 47,
	NVIC_INTERRUPT_FSMC = 48,
	NVIC_INTERRUPT_SDMMC1 = 49,
	NVIC_INTERRUPT_TIM5 = 50,
	NVIC_INTERRUPT_SPI3 = 51,
	NVIC_INTERRUPT_UART4 = 52,
	NVIC_INTERRUPT_UART5 = 53,
	NVIC_INTERRUPT_TIM6_DAC = 54,
	NVIC_INTERRUPT_TIM7 = 55,
	NVIC_INTERRUPT_DMA2_Stream0 = 56,
	NVIC_INTERRUPT_DMA2_Stream1 = 57,
	NVIC_INTERRUPT_DMA2_Stream2 = 58,
	NVIC_INTERRUPT_DMA2_Stream3 = 59,
	NVIC_INTERRUPT_DMA2_Stream4 = 60,
	NVIC_INTERRUPT_ETH = 61,
	NVIC_INTERRUPT_ETH_WKUP = 62,
	NVIC_INTERRUPT_CAN2_TX = 63,
	NVIC_INTERRUPT_CAN2_RX0 = 64,
	NVIC_INTERRUPT_CAN2_RX1 = 65,
	NVIC_INTERRUPT_CAN2_SCE = 66,
	NVIC_INTERRUPT_OTG_FS = 67,
	NVIC_INTERRUPT_DMA2_Stream5 = 68,
	NVIC_INTERRUPT_DMA2_Stream6 = 69,
	NVIC_INTERRUPT_DMA2_Stream7 = 70,
	NVIC_INTERRUPT_USART6 = 71,
	NVIC_INTERRUPT_I2C3_EV = 72,
	NVIC_INTERRUPT_I2C3_ER = 73,
	NVIC_INTERRUPT_OTG_HS_EP1_OUT = 74,
	NVIC_INTERRUPT_OTG_HS_EP1_IN = 75,
	NVIC_INTERRUPT_OTG_HS_WKUP = 76,
	NVIC_INTERRUPT_OTG_HS = 77,
	NVIC_INTERRUPT_DCMI = 78,
	NVIC_INTERRUPT_CRYP = 79,
	NVIC_INTERRUPT_HASH_RNG = 80,
	NVIC_INTERRUPT_FPU = 81,
	NVIC_INTERRUPT_UART7 = 82,
	NVIC_INTERRUPT_UART8 = 83,
	NVIC_INTERRUPT_SPI4 = 84,
	NVIC_INTERRUPT_SPI5 = 85,
	NVIC_INTERRUPT_SPI6 = 86,
	NVIC_INTERRUPT_SAI1 = 87,
	NVIC_INTERRUPT_LCD_TFT = 88,
	NVIC_INTERRUPT_LCD_TFT_ERR = 89,
	NVIC_INTERRUPT_DMA2D = 90,
	NVIC_INTERRUPT_SAI2 = 91,
	NVIC_INTERRUPT_QuadSPI = 92,
	NVIC_INTERRUPT_LPTIM1 = 93,
	NVIC_INTERRUPT_HDMI_CEC = 94,
	NVIC_INTERRUPT_I2C4_EV = 95,
	NVIC_INTERRUPT_I2C4_ER = 96,
	NVIC_INTERRUPT_SPDIFRX = 97
} NVIC_interrupt_t;

/*!******************************************************************
 * \enum NVIC_priority_list_t
 * \brief NVIC interrupt priorities list.
 *******************************************************************/
typedef enum {
	// Timers.
	NVIC_PRIORITY_TIM7 = 0,
	NVIC_PRIORITY_TIM6,
	// SGDU interface.
	NVIC_PRIORITY_USART1
} NVIC_priority_list_t;

/*** NVIC functions ***/

/*!******************************************************************
 * \fn void NVIC_init(void)
 * \brief Init interrupts vector.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void NVIC_init(void);

/*!******************************************************************
 * \fn void NVIC_enable_interrupt(NVIC_interrupt_t irq_index, uint8_t priority)
 * \brief Enable interrupt.
 * \param[in]  	irq_index: Interrupt to enable.
 * \param[in]	priority: Interrupt priority to set.
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void NVIC_enable_interrupt(NVIC_interrupt_t irq_index, uint8_t priority);

/*!******************************************************************
 * \fn void NVIC_disable_interrupt(NVIC_interrupt_t irq_index)
 * \brief Disable interrupt.
 * \param[in]  	irq_index: Interrupt to enable.
 * \param[out] 	none
 * \retval		none
 *******************************************************************/
void NVIC_disable_interrupt(NVIC_interrupt_t irq_index);

#endif /* __NVIC_H__ */
