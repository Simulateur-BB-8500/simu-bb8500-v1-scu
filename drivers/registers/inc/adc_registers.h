/*
 * adc_registers.h
 *
 *  Created on: 19 sep. 2017
 *      Author: Ludo
 */

#ifndef __ADC_REGISTERS_H__
#define __ADC_REGISTERS_H__

#include "stdint.h"

/*** ADC REGISTERS macros ***/

// Peripherals base address.
#define ADC1    ((ADC_registers_t*) ((uint32_t) 0x40012000))
#define ADC2    ((ADC_registers_t*) ((uint32_t) 0x40012100))
#define ADC3    ((ADC_registers_t*) ((uint32_t) 0x40012200))
#define ADCCR   ((ADC_common_registers_t*) ((uint32_t) 0x40012300))

/*** ADC REGISTERS structures ***/

/*!******************************************************************
 * \enum ADC_registers_t
 * \brief ADC registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t SR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMPR1;
    volatile uint32_t SMPR2;
    volatile uint32_t JOFR1;
    volatile uint32_t JOFR2;
    volatile uint32_t JOFR3;
    volatile uint32_t JOFR4;
    volatile uint32_t HTR;
    volatile uint32_t LTR;
    volatile uint32_t SQR1;
    volatile uint32_t SQR2;
    volatile uint32_t SQR3;
    volatile uint32_t JSQR;
    volatile uint32_t JDR1;
    volatile uint32_t JDR2;
    volatile uint32_t JDR3;
    volatile uint32_t JDR4;
    volatile uint32_t DR;
} ADC_registers_t;

/*!******************************************************************
 * \enum ADC_common_registers_t
 * \brief ADC common registers map.
 *******************************************************************/
typedef struct {
    volatile uint32_t CSR;
    volatile uint32_t CCR;
    volatile uint32_t CDR;
} ADC_common_registers_t;

#endif /* __ADC_REGISTERS_H__ */
