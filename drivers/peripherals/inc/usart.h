/*
 * usart.h
 *
 *  Created on: 28 sep. 2017
 *      Author: Ludo
 */

#ifndef __USART_H__
#define __USART_H__

#include "stdint.h"

/*** USART structures ***/

/*!******************************************************************
 * \fn USART_rx_irq_cb_t
 * \brief USART RX interrupt callback.
 *******************************************************************/
typedef void (*USART_rx_irq_cb_t)(uint8_t data);

/*** USART functions ***/

/*!******************************************************************
 * \fn void USART1_init(USART_rx_irq_cb_t irq_callback)
 * \brief Init USART1 peripheral.
 * \param[in]   irq_callback: Function to call on interrupt.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void USART1_init(USART_rx_irq_cb_t irq_callback);

/*!******************************************************************
 * \fn USART_status_t USART1_write(uint8_t* data, uint8_t data_size_bytes)
 * \brief Send data over USART1.
 * \param[in]   data: Byte array to send.
 * \param[in]   data_size_bytes: Number of bytes to send.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void USART1_write(uint8_t* data, uint8_t data_size_bytes);

/*!******************************************************************
 * \fn void USART1_process(void)
 * \brief Process USART1 peripheral.
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
void USART1_process(void);

#endif /* __USART_H__ */
