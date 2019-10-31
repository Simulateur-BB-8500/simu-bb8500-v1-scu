/**
  ******************************************************************************
  * @file      startup_stm32.s
  * @author    Ac6
  * @version   V1.0.0
  * @date      12-June-2014
  ******************************************************************************
  */

  .syntax unified
  .cpu cortex-m7
  .thumb

.global	g_pfnVectors
.global	Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word	_sidata
/* start address for the .data section. defined in linker script */
.word	_sdata
/* end address for the .data section. defined in linker script */
.word	_edata
/* start address for the .bss section. defined in linker script */
.word	_sbss
/* end address for the .bss section. defined in linker script */
.word	_ebss

.equ  BootRAM,        0xF1E0F85F
/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called.
 * @param  None
 * @retval : None
*/

    .section	.text.Reset_Handler
	.weak	Reset_Handler
	.type	Reset_Handler, %function
Reset_Handler:

/* Copy the data segment initializers from flash to SRAM */
  movs	r1, #0
  b	LoopCopyDataInit

CopyDataInit:
	ldr	r3, =_sidata
	ldr	r3, [r3, r1]
	str	r3, [r0, r1]
	adds	r1, r1, #4

LoopCopyDataInit:
	ldr	r0, =_sdata
	ldr	r3, =_edata
	adds	r2, r0, r1
	cmp	r2, r3
	bcc	CopyDataInit
	ldr	r2, =_sbss
	b	LoopFillZerobss
/* Zero fill the bss segment. */
FillZerobss:
	movs r3, #0
 	str  r3, [r2]
	adds r2, r2, #4

LoopFillZerobss:
	ldr	r3, = _ebss
	cmp	r2, r3
	bcc	FillZerobss

/* Call the clock system intitialization function.*/
    bl  SystemInit
/* Call static constructors */
    bl __libc_init_array
/* Call the application's entry point.*/
	bl	main

LoopForever:
    b LoopForever

.size	Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
    .section	.text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
	b	Infinite_Loop
	.size	Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex-M.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
 	.section	.isr_vector,"a",%progbits
	.type	g_pfnVectors, %object
	.size	g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
	.word	_estack
	.word	Reset_Handler
	.word	NMI_Handler
	.word	HardFault_Handler
	.word	MemManage_Handler
	.word	BusFault_Handler
	.word	UsageFault_Handler
	.word	0
	.word	0
	.word	0
	.word	0
	.word	SVC_Handler
	.word	DebugMon_Handler
	.word	0
	.word	PendSV_Handler
	.word	SysTick_Handler
	.word	0 // 0 = WWDG.
	.word	0 // 1 = PVD.
	.word	0 // 2 = TAMP_STAMP.
	.word	0 // 3 = RTC_WKUP.
	.word	0 // 4 = FLASH.
	.word	0 // 5 = RCC.
	.word	0 // 6 = EXTI0.
	.word	0 // 7 = EXTI1.
	.word	0 // 8 = EXTI2.
	.word	0 // 9 = EXTI3
	.word	0 // 10 = EXTI4.
	.word	0 // 11 = DMA_Stream0.
	.word	0 // 12 = DMA_Stream1.
	.word	0 // 13 = DMA_Stream2.
	.word	0 // 14 = DMA_Stream3.
	.word	0 // 15 = DMA_Stream4.
	.word	0 // 16 = DMA_Stream5.
	.word	0 // 17 = DMA_Stream6.
	.word	ADC_InterruptHandler // 18 = ADC.
	.word	0 // 19 = CAN1_TX.
	.word	0 // 20 = CAN1_RX0.
	.word	0 // 21 = CAN1_RX1.
	.word	0 // 22 = CAN1_SCE.
	.word	0 // 23 = EXTI9_5.
	.word	0 // 24 = TIM1_BRK_TIM9.
	.word	0 // 25 = TIM1_UP_TIM10.
	.word	0 // 26 = TIM1_TRG_COM_TIM11.
	.word	0 // 27 = TIM1_CC.
	.word	0 // 28 = TIM2.
	.word	0 // 29 = TIM3.
	.word	0 // 30 = TIM4.
	.word	0 // 31 = I2C1_EV.
	.word	0 // 32 = I2C1_ER.
	.word	0 // 33 = I2C2_EV.
	.word	0 // 34 = I2C2_ER.
	.word	0 // 35 = SPI1.
	.word	0 // 36 = SPI2.
	.word 	USART1_InterruptHandler // 37 = USART1.
	.word	0 // 38 = USART2.
	.word	0 // 39 = USART3.
	.word	0 // 40 = EXTI15_10.
	.word	0 // 41 = RTC_Alarm.
	.word	0 // 42 = OTG_FS_WKUP.
	.word	0 // 43 = TIM8_BRK_TIM12.
	.word	0 // 44 = TIM8_UP_TIM13.
	.word	0 // 45 = TIM8_TRG_COM_TIM14.
	.word	0 // 46 = TIM8_CC.
	.word	0 // 47 = DMA1_Stream7
	.word	0 // 48 = FSMC.
	.word	0 // 49 = SDMMC1.
	.word	0 // 50 = TIM5.
	.word	0 // 51 = SPI3.
	.word	0 // 52 = UART4.
	.word	0 // 53 = UART5.
	.word	TIM6_DAC_InterruptHandler // 54 = TIM6_DAC.
	.word	TIM7_InterruptHandler // 55 = TIM7.
	.word	0 // 56 = DMA2_Stream0.
	.word	0 // 57 = DMA2_Stream1.
	.word	0 // 58 = DMA2_Stream2.
	.word	0 // 59 = DMA2_Stream3.
	.word	0 // 60 = DMA2_Stream4.
	.word	0 // 61 = ETH.
	.word	0 // 62 = ETH_WKUP.
	.word	0 // 63 = CAN2_TX.
	.word	0 // 64 = CAN2_RX0.
	.word	0 // 65 = CAN2_RX1.
	.word	0 // 66 = CAN2_SCE.
	.word	0 // 67 = OTG_FS.
	.word	0 // 68 = DMA2_Stream5.
	.word	0 // 69 = DMA2_Stream6.
	.word	0 // 70 = DMA2_Stream7.
	.word	0 // 71 = USART6.
	.word	0 // 72 = I2C3_EV.
	.word	0 // 73 = I2C3_ER.
	.word	0 // 74 = OTG_HS_EP1_OUT.
	.word	0 // 75 = OTG_HS_EP1_IN.
	.word	0 // 76 = OTG_HS_WKUP.
	.word	0 // 77 = OTG_HS.
	.word	0 // 78 = DCMI.
	.word	0 // 79 = CRYP.
	.word	0 // 80 = HASH_RNG.
	.word	0 // 81 = FPU.
	.word	0 // 82 = UART7.
	.word	0 // 83 = UART8.
	.word	0 // 84 = SPI4.
	.word	0 // 85 = SPI5.
	.word	0 // 86 = SPI6.
	.word	0 // 87 = SAI1.
	.word	0 // 88 = LCD_TFT.
	.word	0 // 89 = LCD_TFT_ERR.
	.word	0 // 90 = DMA2D.
	.word	0 // 91 = SAI2.
	.word	0 // 92 = QuadSPI.
	.word	0 // 93 = LPTIM1.
	.word	0 // 94 = HDMI_CEC.
	.word	0 // 95 = I2C4_EV.
	.word	0 // 96 = I2C4_ER.
	.word	0 // 97 = SPDIFRX.

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

  	.weak	NMI_Handler
	.thumb_set NMI_Handler,Default_Handler

  	.weak	HardFault_Handler
	.thumb_set HardFault_Handler,Default_Handler

  	.weak	MemManage_Handler
	.thumb_set MemManage_Handler,Default_Handler

  	.weak	BusFault_Handler
	.thumb_set BusFault_Handler,Default_Handler

	.weak	UsageFault_Handler
	.thumb_set UsageFault_Handler,Default_Handler

	.weak	SVC_Handler
	.thumb_set SVC_Handler,Default_Handler

	.weak	DebugMon_Handler
	.thumb_set DebugMon_Handler,Default_Handler

	.weak	PendSV_Handler
	.thumb_set PendSV_Handler,Default_Handler

	.weak	SysTick_Handler
	.thumb_set SysTick_Handler,Default_Handler

	.weak	SystemInit
	
	.weak	ADC_InterruptHandler
	.thumb_set ADC_InterruptHandler,Default_Handler
	
	.weak	USART1_InterruptHandler
	.thumb_set USART1_InterruptHandler,Default_Handler

	.weak	TIM6_DAC_InterruptHandler
	.thumb_set TIM6_DAC_InterruptHandler,Default_Handler

	.weak	TIM7_InterruptHandler
	.thumb_set TIM7_InterruptHandler,Default_Handler

/************************ (C) COPYRIGHT Ac6 *****END OF FILE****/
