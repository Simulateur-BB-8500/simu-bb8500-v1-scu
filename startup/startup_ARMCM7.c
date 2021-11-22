/* File: startup_ARMCM7.c
 * Purpose: startup file for Cortex-M7 devices.
 *          Should be used with GCC 'GNU Tools ARM Embedded'
 * Version: V1.01
 * Date: 22 August 2014
 *
 */
/* Copyright (c) 2011 - 2014 ARM LIMITED
   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

#include <stdint.h>


/*----------------------------------------------------------------------------
  Linker generated Symbols
 *----------------------------------------------------------------------------*/
extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __copy_table_start__;
extern uint32_t __copy_table_end__;
extern uint32_t __zero_table_start__;
extern uint32_t __zero_table_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __StackTop;

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler Function Prototype
 *----------------------------------------------------------------------------*/
typedef void( *pFunc )( void );


/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
#ifndef __START
extern void  _start(void) __attribute__((noreturn));    /* PreeMain (C library entry point) */
#else
extern int  __START(void) __attribute__((noreturn));    /* main entry point */
#endif

#ifndef __NO_SYSTEM_INIT
extern void SystemInit (void);            /* CMSIS System Initialization      */
#endif


/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
void Default_Handler(void);                          /* Default empty handler */
void Reset_Handler(void);                            /* Reset Handler */


/*----------------------------------------------------------------------------
  User Initial Stack & Heap
 *----------------------------------------------------------------------------*/
#ifndef __STACK_SIZE
  #define	__STACK_SIZE  0x00000400
#endif
static uint8_t stack[__STACK_SIZE] __attribute__ ((aligned(8), used, section(".stack")));

#ifndef __HEAP_SIZE
  #define	__HEAP_SIZE   0x00000C00
#endif
#if __HEAP_SIZE > 0
static uint8_t heap[__HEAP_SIZE]   __attribute__ ((aligned(8), used, section(".heap")));
#endif


/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Cortex-M7 Processor Exceptions */
void NMI_Handler         			(void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler   			(void) __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler   			(void) __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler    			(void) __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler  			(void) __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler         			(void) __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler    			(void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler      			(void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler     			(void) __attribute__ ((weak, alias("Default_Handler")));

/* ARMCM7 Specific Interrupts */

void WWDG_IRQHandler	 			(void) __attribute__ ((weak, alias("Default_Handler")));
void PVD_IRQHandler		 			(void) __attribute__ ((weak, alias("Default_Handler")));
void TAMP_STAMP_IRQHandler			(void) __attribute__ ((weak, alias("Default_Handler")));
void RTC_WKUP_IRQHandler			(void) __attribute__ ((weak, alias("Default_Handler")));
void FLASH_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void RCC_IRQHandler					(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI0_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI1_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI2_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI3_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI4_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream0_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream1_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream2_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream3_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream4_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream5_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream6_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void ADC_IRQHandler					(void) __attribute__ ((weak, alias("Default_Handler")));
void CAN1_TX_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void CAN1_RX0_IRQHandler			(void) __attribute__ ((weak, alias("Default_Handler")));
void CAN1_RX1_IRQHandler			(void) __attribute__ ((weak, alias("Default_Handler")));
void CAN1_SCE_IRQHandler			(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_BRK_TIM9_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_UP_TIM10_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_TRG_COM_TIM11_IRQHandler	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM2_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM3_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM4_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void SPI1_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void SPI2_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void USART1_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void USART2_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void USART3_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler			(void) __attribute__ ((weak, alias("Default_Handler")));
void RTC_ALARM_IRQHandler			(void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_FS_WKUP_IRQHandler			(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_BRK_TIM12_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_UP_TIM13_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_TRG_COM_TIM14_IRQHandler	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM8_CC_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Stream7_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void FSMC_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void SDMMC1_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM5_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void SPI3_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void UART4_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void UART5_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM6_DAC_IRQHandler			(void) __attribute__ ((weak, alias("Default_Handler")));
void TIM7_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream0_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream1_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream2_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream3_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream4_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_FS_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream5_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream6_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA2_Stream7_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void USART6_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C3_EV_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void I2C3_ER_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_HS_EP1_OUT_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_HS_EP1_IN_IRQHandler		(void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_HS_WKUP_IRQHandler			(void) __attribute__ ((weak, alias("Default_Handler")));
void OTG_HS_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void AES_IRQHandler					(void) __attribute__ ((weak, alias("Default_Handler")));
void RNG_IRQHandler					(void) __attribute__ ((weak, alias("Default_Handler")));
void FPU_IRQHandler					(void) __attribute__ ((weak, alias("Default_Handler")));
void UART7_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void UART8_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void SPI4_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void SPI5_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void SAI1_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void SAI2_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void QuadSPI_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));
void LP_Timer1_IRQHandler			(void) __attribute__ ((weak, alias("Default_Handler")));
void SDMMC2_IRQHandler				(void) __attribute__ ((weak, alias("Default_Handler")));

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/
const pFunc __Vectors[] __attribute__ ((section(".vectors"))) = {
  /* Cortex-M7 Exceptions Handler */
  (pFunc)&__StackTop,                       /*      Initial Stack Pointer     */
  Reset_Handler,                            /*      Reset Handler             */
  NMI_Handler,                              /*      NMI Handler               */
  HardFault_Handler,                        /*      Hard Fault Handler        */
  MemManage_Handler,                        /*      MPU Fault Handler         */
  BusFault_Handler,                         /*      Bus Fault Handler         */
  UsageFault_Handler,                       /*      Usage Fault Handler       */
  0,                                        /*      Reserved                  */
  0,                                        /*      Reserved                  */
  0,                                        /*      Reserved                  */
  0,                                        /*      Reserved                  */
  SVC_Handler,                              /*      SVCall Handler            */
  DebugMon_Handler,                         /*      Debug Monitor Handler     */
  0,                                        /*      Reserved                  */
  PendSV_Handler,                           /*      PendSV Handler            */
  SysTick_Handler,                          /*      SysTick Handler           */

  /* External interrupts */
  WWDG_IRQHandler,              			/* Window Watchdog interrupt                                           */
  PVD_IRQHandler,               			/* PVD through EXTI line detection interrupt                           */
  TAMP_STAMP_IRQHandler,        			/* Tamper and TimeStamp interrupts through the EXTI line               */
  RTC_WKUP_IRQHandler,          			/* RTC Tamper or TimeStamp /CSS on LSE through EXTI line 19 interrupts */
  FLASH_IRQHandler,             			/* Flash global interrupt                                              */
  RCC_IRQHandler,               			/* RCC global interrupt                                                */
  EXTI0_IRQHandler,             			/* EXTI Line0 interrupt                                                */
  EXTI1_IRQHandler,             			/* EXTI Line1 interrupt                                                */
  EXTI2_IRQHandler,             			/* EXTI Line2 interrupt                                                */
  EXTI3_IRQHandler,             			/* EXTI Line3 interrupt                                                */
  EXTI4_IRQHandler,             			/* EXTI Line4 interrupt                                                */
  DMA1_Stream0_IRQHandler,      			/* DMA1 Stream0 global interrupt                                       */
  DMA1_Stream1_IRQHandler,      			/* DMA1 Stream1 global interrupt                                       */
  DMA1_Stream2_IRQHandler,      			/* DMA1 Stream2 global interrupt                                       */
  DMA1_Stream3_IRQHandler,      			/* DMA1 Stream3 global interrupt                                       */
  DMA1_Stream4_IRQHandler,      			/* DMA1 Stream4 global interrupt                                       */
  DMA1_Stream5_IRQHandler,      			/* DMA1 Stream5 global interrupt                                       */
  DMA1_Stream6_IRQHandler,      			/* DMA1 Stream6 global interrupt                                       */
  ADC_IRQHandler,               			/* ADC1 global interrupt                                               */
  CAN1_TX_IRQHandler,           			/* CAN1 TX interrupts                                                  */
  CAN1_RX0_IRQHandler,          			/* CAN1 RX0 interrupts                                                 */
  CAN1_RX1_IRQHandler,          			/* CAN1 RX1 interrupts                                                 */
  CAN1_SCE_IRQHandler,          			/* CAN1 SCE interrupt                                                  */
  EXTI9_5_IRQHandler,           			/* EXTI Line[9:5] interrupts                                           */
  TIM1_BRK_TIM9_IRQHandler,     			/* TIM1 Break interrupt and TIM9 global interrupt                      */
  TIM1_UP_TIM10_IRQHandler,     			/* TIM1 Update interrupt and TIM10                                     */
  TIM1_TRG_COM_TIM11_IRQHandler,			/* TIM1 Trigger and Commutation interrupts and TIM11 global interrupt  */
  TIM1_CC_IRQHandler,           			/* TIM1 Capture Compare interrupt                                      */
  TIM2_IRQHandler,              			/* TIM2 global interrupt                                               */
  TIM3_IRQHandler,              			/* TIM3 global interrupt                                               */
  TIM4_IRQHandler,              			/* TIM4 global interrupt                                               */
  I2C1_EV_IRQHandler,           			/* I2C1 event interrupt                                                */
  I2C1_ER_IRQHandler,           			/* I2C1 error interrupt                                                */
  I2C2_EV_IRQHandler,           			/* I2C2 event interrupt                                                */
  I2C2_ER_IRQHandler,           			/* I2C2 error interrupt                                                */
  SPI1_IRQHandler,              			/* SPI1 global interrupt                                               */
  SPI2_IRQHandler,              			/* SPI2 global interrupt                                               */
  USART1_IRQHandler,            			/* USART1 global interrupt                                             */
  USART2_IRQHandler,            			/* USART2 global interrupt                                             */
  USART3_IRQHandler,            			/* USART3 global interrupt                                             */
  EXTI15_10_IRQHandler,         			/* EXTI Line[15:10] interrupts                                         */
  RTC_ALARM_IRQHandler,         			/* RTC alarms through EXTI line 18 interrupts                          */
  OTG_FS_WKUP_IRQHandler,       			/* USB On-The-Go FS Wakeup through EXTI line interrupt                 */
  TIM8_BRK_TIM12_IRQHandler,    			/* TIM8 Break interrupt and TIM12 global interrupt                     */
  TIM8_UP_TIM13_IRQHandler,     			/* TIM8 Update interrupt and TIM13 global interrupt                    */
  TIM8_TRG_COM_TIM14_IRQHandler,			/* TIM8 Trigger and Commutation interrupts and TIM14 global interrupt  */
  TIM8_CC_IRQHandler,           			/* TIM8 Capture Compare interrupt                                      */
  DMA1_Stream7_IRQHandler,      			/* DMA1 Stream7 global interrupt                                       */
  FSMC_IRQHandler,              			/* FMC global interrupt                                                */
  SDMMC1_IRQHandler,            			/* SDMMC1 global interrupt                                             */
  TIM5_IRQHandler,              			/* TIM5 global interrupt                                               */
  SPI3_IRQHandler,              			/* SPI3 global interrupt                                               */
  UART4_IRQHandler,             			/* UART4 global interrupt                                              */
  UART5_IRQHandler,             			/* UART5 global interrupt                                              */
  TIM6_DAC_IRQHandler,          			/* TIM6 global interrupt, DAC1 and DAC2 underrun error interrupt       */
  TIM7_IRQHandler,              			/* TIM7 global interrupt                                               */
  DMA2_Stream0_IRQHandler,      			/* DMA2 Stream0 global interrupt                                       */
  DMA2_Stream1_IRQHandler,      			/* DMA2 Stream1 global interrupt                                       */
  DMA2_Stream2_IRQHandler,      			/* DMA2 Stream2 global interrupt                                       */
  DMA2_Stream3_IRQHandler,      			/* DMA2 Stream3 global interrupt                                       */
  DMA2_Stream4_IRQHandler,      			/* DMA2 Stream4 global interrupt                                       */
  0,                            			/* Reserved                                                            */
  0,                            			/* Reserved                                                            */
  0,                            			/* Reserved                                                            */
  0,                            			/* Reserved                                                            */
  0,                            			/* Reserved                                                            */
  0,                            			/* Reserved                                                            */
  OTG_FS_IRQHandler,            			/* USB On The Go FS global interrupt                                   */
  DMA2_Stream5_IRQHandler,      			/* DMA2 Stream5 global interrupt                                       */
  DMA2_Stream6_IRQHandler,      			/* DMA2 Stream6 global interrupt                                       */
  DMA2_Stream7_IRQHandler,      			/* DMA2 Stream7 global interrupt                                       */
  USART6_IRQHandler,            			/* USART6 global interrupt                                             */
  I2C3_EV_IRQHandler,           			/* I2C3 event interrupt                                                */
  I2C3_ER_IRQHandler,           			/* I2C3 error interrupt                                                */
  OTG_HS_EP1_OUT_IRQHandler,    			/* USB On The Go HS End Point 1 Out                                    */
  OTG_HS_EP1_IN_IRQHandler,     			/* USB On The Go HS End Point 1 In                                     */
  OTG_HS_WKUP_IRQHandler,       			/* USB On The Go HS Wakeup through                                     */
  OTG_HS_IRQHandler,            			/* USB On The Go HS global interrupt                                   */
  0,                            			/* Reserved                                                            */
  AES_IRQHandler,               			/* AES global interrupt                                                */
  RNG_IRQHandler,               			/* Rng global interrupt                                                */
  FPU_IRQHandler,               			/* Floating point unit interrupt                                       */
  UART7_IRQHandler,             			/* UART7 global interrupt                                              */
  UART8_IRQHandler,             			/* UART 8 global interrupt                                             */
  SPI4_IRQHandler,              			/* SPI 4 global interrupt                                              */
  SPI5_IRQHandler,              			/* SPI 5 global interrupt                                              */
  0,                            			/* Reserved                                                            */
  SAI1_IRQHandler,              			/* SAI1 global interrupt                                               */
  0,                            			/* Reserved                                                            */
  0,                            			/* Reserved                                                            */
  0,                            			/* Reserved                                                            */
  SAI2_IRQHandler,              			/* SAI2 global interrupt                                               */
  QuadSPI_IRQHandler,           			/* QuadSPI global interrupt                                            */
  LP_Timer1_IRQHandler,         			/* LP Timer1 global interrupt                                          */
  0,                            			/* Reserved                                                            */
  0,                            			/* Reserved                                                            */
  0,                            			/* Reserved                                                            */
  0,                            			/* Reserved                                                            */
  0,                            			/* Reserved                                                            */
  0,                            			/* Reserved                                                            */
  0,                            			/* Reserved                                                            */
  0,                            			/* Reserved                                                            */
  0,                            			/* Reserved                                                            */
  SDMMC2_IRQHandler,            			/* SDMMC2 global interrupt                                             */
};


/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void) {
  uint32_t *pSrc, *pDest;
  uint32_t *pTable __attribute__((unused));

/*  Firstly it copies data from read only memory to RAM. There are two schemes
 *  to copy. One can copy more than one sections. Another can only copy
 *  one section.  The former scheme needs more instructions and read-only
 *  data to implement than the latter.
 *  Macro __STARTUP_COPY_MULTIPLE is used to choose between two schemes.  */

#ifdef __STARTUP_COPY_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of triplets, each of which specify:
 *    offset 0: LMA of start of a section to copy from
 *    offset 4: VMA of start of a section to copy to
 *    offset 8: size of the section to copy. Must be multiply of 4
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
  pTable = &__copy_table_start__;

  for (; pTable < &__copy_table_end__; pTable = pTable + 3) {
		pSrc  = (uint32_t*)*(pTable + 0);
		pDest = (uint32_t*)*(pTable + 1);
		for (; pDest < (uint32_t*)(*(pTable + 1) + *(pTable + 2)) ; ) {
      *pDest++ = *pSrc++;
		}
	}
#else
/*  Single section scheme.
 *
 *  The ranges of copy from/to are specified by following symbols
 *    __etext: LMA of start of the section to copy from. Usually end of text
 *    __data_start__: VMA of start of the section to copy to
 *    __data_end__: VMA of end of the section to copy to
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
  pSrc  = &__etext;
  pDest = &__data_start__;

  for ( ; pDest < &__data_end__ ; ) {
    *pDest++ = *pSrc++;
  }
#endif /*__STARTUP_COPY_MULTIPLE */

/*  This part of work usually is done in C library startup code. Otherwise,
 *  define this macro to enable it in this startup.
 *
 *  There are two schemes too. One can clear multiple BSS sections. Another
 *  can only clear one section. The former is more size expensive than the
 *  latter.
 *
 *  Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 *  Otherwise efine macro __STARTUP_CLEAR_BSS to choose the later.
 */
#ifdef __STARTUP_CLEAR_BSS_MULTIPLE
/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of tuples specifying:
 *    offset 0: Start of a BSS section
 *    offset 4: Size of this BSS section. Must be multiply of 4
 */
  pTable = &__zero_table_start__;

  for (; pTable < &__zero_table_end__; pTable = pTable + 2) {
		pDest = (uint32_t*)*(pTable + 0);
		for (; pDest < (uint32_t*)(*(pTable + 0) + *(pTable + 1)) ; ) {
      *pDest++ = 0;
		}
	}
#elif defined (__STARTUP_CLEAR_BSS)
/*  Single BSS section scheme.
 *
 *  The BSS section is specified by following symbols
 *    __bss_start__: start of the BSS section.
 *    __bss_end__: end of the BSS section.
 *
 *  Both addresses must be aligned to 4 bytes boundary.
 */
  pDest = &__bss_start__;

  for ( ; pDest < &__bss_end__ ; ) {
    *pDest++ = 0ul;
  }
#endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */

#ifndef __NO_SYSTEM_INIT
	SystemInit();
#endif

#ifndef __START
#define __START _start
#endif
	__START();

}


/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
void Default_Handler(void) {

	while(1);
}
