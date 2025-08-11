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

void UART1_Init(void);
void UART1_Tx(uint8_t *data, uint8_t n);
void UART1_Rx(uint8_t *data, uint8_t n);

#ifdef __cplusplus
}
#endif
#endif /* __UART_H__ */
