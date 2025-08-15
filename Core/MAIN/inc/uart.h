/*
 * uart.h
 *
 *  Created on: Dec 10, 2022
 *      Author: mkpk
 */

#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include <stm32f1xx_ll_usart.h>

void UART1_Init(void);

void USART2_Init(void);

void USARTx_Set_BaudRate(USART_TypeDef *USARTx, uint32_t baudRate);

void USARTx_Tx(USART_TypeDef *USARTx, uint8_t *data, uint8_t n);

void USARTx_Rx(USART_TypeDef *USARTx, uint8_t *data, uint8_t n);

#ifdef __cplusplus
}
#endif
#endif /* __UART_H__ */
