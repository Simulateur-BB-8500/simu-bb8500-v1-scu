/*
 * nvic_reg.h
 *
 *  Created on: 16 sept. 2017
 *      Author: Ludovic
 */

#ifndef REGISTERS_NVIC_REG_H_
#define REGISTERS_NVIC_REG_H_

/*** Interrupts vector ***/

typedef enum {
	WWDG = 0,
	PVD = 1,
	TAMP_STAMP = 2,
	RTC_WKUP = 3,
	FLASH = 4,
	RCC = 5,
	EXTI0 = 6,
	EXTI1 = 7,
	EXTI2 = 8,
	EXTI3 = 9,
	EXTI4 = 10,
	DMA_Stream0 = 11,
	DMA_Stream1 = 12,
	DMA_Stream2 = 13,
	DMA_Stream3 = 14,
	DMA_Stream4 = 15,
	DMA_Stream5 = 16,
	DMA_Stream6 = 17,
	ADC = 18,
	CAN1_TX = 19,
	CAN1_RX0 = 20,
	CAN1_RX1 = 21,
	CAN1_SCE = 22,
	EXTI9_5 = 23,
	TIM1_BRK_RIM9 = 24,
	TIM1_UP_TIM10 = 25,
	TIM1_TRG_COM_TIM11 = 26,
	TIM1_CC = 27,
	TIM2 = 28,
	TIM3 = 29,
	TIM4 = 30,
	I2C1_EV = 31,
	I2C1_ER = 32,
	I2C2_EV = 33,
	I2C2_ER = 34,
	SPI1 = 35,
	SPI2 = 36,
	USART1 = 37,
	USART2 = 38,
	USART3 = 39,
	EXTI15_10 = 40,
	RTC_Alarm = 41,
	OTG_FS_WKUP = 42,
	TIM8_BRK_TIM12 = 43,
	TIM8_UP_TIM13 = 44,
	TIM8_TRG_COM_TIM14 = 45,
	TIM8_CC = 46,
	DMA1_Stream7 = 47,
	FSMC = 48,
	SDMMC1 = 49,
	TIM5 = 50,
	SPI3 = 51,
	UART4 = 52,
	UART5 = 53,
	TIM6_DAC = 54,
	TIM7 = 55,
	DMA2_Stream0 = 56,
	DMA2_Stream1 = 57,
	DMA2_Stream2 = 58,
	DMA2_Stream3 = 59,
	DMA2_Stream4 = 60,
	ETH = 61,
	ETH_WKUP = 62,
	CAN2_TX = 63,
	CAN2_RX0 = 64,
	CAN2_RX1 = 65,
	CAN2_SCE = 66,
	OTG_FS = 67,
	DMA2_Stream5 = 68,
	DMA2_Stream6 = 69,
	DMA2_Stream7 = 70,
	USART6 = 71,
	I2C3_EV = 72,
	I2C3_ER = 73,
	OTG_HS_EP1_OUT = 74,
	OTG_HS_EP1_IN = 75,
	OTG_HS_WKUP = 76,
	OTG_HS = 77,
	DCMI = 78,
	CRYP = 79,
	HASH_RNG = 80,
	FPU = 81,
	UART7 = 82,
	UART8 = 83,
	SPI4 = 84,
	SPI5 = 85,
	SPI6 = 86,
	SAI1 = 87,
	LCD_TFT = 88,
	LCD_TFT_ERR = 89,
	DMA2D = 90,
	SAI2 = 91,
	QuadSPI = 92,
	LPTIM1 = 93,
	HDMI_CEC = 94,
	I2C4_EV = 95,
	I2C4_ER = 96,
	SPDIFRX = 97
} Interrupt;

/*** NVIC registers ***/

typedef struct {
	volatile unsigned int ISER[8];	// Interrupt set-enable registers 0 to 7.
	unsigned int RESERVED0[24];		// Reserved 0xE000E130.
	volatile unsigned int ICER[8];	// Interrupt clear-enable registers 0 to 7.
	unsigned int RESERVED1[24];		// Reserved 0xE000E1A0.
	volatile unsigned int ISPR[8];	// Interrupt set-pending registers 0 to 7.
	unsigned int RESERVED2[24];		// Reserved 0xE000E220.
	volatile unsigned int ICPR;    	// Interrupt clear-pending registers 0 to 7.
	unsigned int RESERVED3[24];		// Reserved 0xE000E300.
	volatile unsigned int IABR[8];	// Interrupt active bit registers 0 to 7.
	unsigned int RESERVED4[56];		// Reserved 0xE000E320.
	volatile unsigned int IPR[60];	// Interrupt active bit registers 0 to 7.
	unsigned int RESERVED5[644];	// Reserved 0xE000E4F0.
	volatile unsigned int STIR;    	// Interrupt software trigger register.
} NVIC_BaseAddress;

/*** NVIC base address ***/

#define NVIC	((NVIC_BaseAddress*) ((unsigned int) 0xE000E100))

#endif /* REGISTERS_NVIC_REG_H_ */
