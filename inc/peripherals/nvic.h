/*
 * nvic.h
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludovic
 */

#ifndef PERIPHERALS_NVIC_H
#define PERIPHERALS_NVIC_H

#include "nvic_reg.h"

/*** NVIC interrupts vector ***/

typedef enum {
	IT_WWDG = 0,
	IT_PVD = 1,
	IT_TAMP_STAMP = 2,
	IT_RTC_WKUP = 3,
	IT_FLASH = 4,
	IT_RCC = 5,
	IT_EXTI0 = 6,
	IT_EXTI1 = 7,
	IT_EXTI2 = 8,
	IT_EXTI3 = 9,
	IT_EXTI4 = 10,
	IT_DMA_Stream0 = 11,
	IT_DMA_Stream1 = 12,
	IT_DMA_Stream2 = 13,
	IT_DMA_Stream3 = 14,
	IT_DMA_Stream4 = 15,
	IT_DMA_Stream5 = 16,
	IT_DMA_Stream6 = 17,
	IT_ADC = 18,
	IT_CAN1_TX = 19,
	IT_CAN1_RX0 = 20,
	IT_CAN1_RX1 = 21,
	IT_CAN1_SCE = 22,
	IT_EXTI9_5 = 23,
	IT_TIM1_BRK_TIM9 = 24,
	IT_TIM1_UP_TIM10 = 25,
	IT_TIM1_TRG_COM_TIM11 = 26,
	IT_TIM1_CC = 27,
	IT_TIM2 = 28,
	IT_TIM3 = 29,
	IT_TIM4 = 30,
	IT_I2C1_EV = 31,
	IT_I2C1_ER = 32,
	IT_I2C2_EV = 33,
	IT_I2C2_ER = 34,
	IT_SPI1 = 35,
	IT_SPI2 = 36,
	IT_USART1 = 37,
	IT_USART2 = 38,
	IT_USART3 = 39,
	IT_EXTI15_10 = 40,
	IT_RTC_Alarm = 41,
	IT_OTG_FS_WKUP = 42,
	IT_TIM8_BRK_TIM12 = 43,
	IT_TIM8_UP_TIM13 = 44,
	IT_TIM8_TRG_COM_TIM14 = 45,
	IT_TIM8_CC = 46,
	IT_DMA1_Stream7 = 47,
	IT_FSMC = 48,
	IT_SDMMC1 = 49,
	IT_TIM5 = 50,
	IT_SPI3 = 51,
	IT_UART4 = 52,
	IT_UART5 = 53,
	IT_TIM6_DAC = 54,
	IT_TIM7 = 55,
	IT_DMA2_Stream0 = 56,
	IT_DMA2_Stream1 = 57,
	IT_DMA2_Stream2 = 58,
	IT_DMA2_Stream3 = 59,
	IT_DMA2_Stream4 = 60,
	IT_ETH = 61,
	IT_ETH_WKUP = 62,
	IT_CAN2_TX = 63,
	IT_CAN2_RX0 = 64,
	IT_CAN2_RX1 = 65,
	IT_CAN2_SCE = 66,
	IT_OTG_FS = 67,
	IT_DMA2_Stream5 = 68,
	IT_DMA2_Stream6 = 69,
	IT_DMA2_Stream7 = 70,
	IT_USART6 = 71,
	IT_I2C3_EV = 72,
	IT_I2C3_ER = 73,
	IT_OTG_HS_EP1_OUT = 74,
	IT_OTG_HS_EP1_IN = 75,
	IT_OTG_HS_WKUP = 76,
	IT_OTG_HS = 77,
	IT_DCMI = 78,
	IT_CRYP = 79,
	IT_HASH_RNG = 80,
	IT_FPU = 81,
	IT_UART7 = 82,
	IT_UART8 = 83,
	IT_SPI4 = 84,
	IT_SPI5 = 85,
	IT_SPI6 = 86,
	IT_SAI1 = 87,
	IT_LCD_TFT = 88,
	IT_LCD_TFT_ERR = 89,
	IT_DMA2D = 90,
	IT_SAI2 = 91,
	IT_QuadSPI = 92,
	IT_LPTIM1 = 93,
	IT_HDMI_CEC = 94,
	IT_I2C4_EV = 95,
	IT_I2C4_ER = 96,
	IT_SPDIFRX = 97
} InterruptVector;

/*** NVIC functions ***/

void NVIC_EnableInterrupt(InterruptVector it_num);
void NVIC_DisableInterrupt(InterruptVector it_num);
void NVIC_SetPriority(InterruptVector it_num, unsigned char priority);

#endif /* PERIPHERALS_NVIC_H */
