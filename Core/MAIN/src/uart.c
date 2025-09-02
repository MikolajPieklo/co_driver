/*
 * uart.c
 *
 *  Created on: Dec 18, 2022
 *      Author: mkpk
 */
#include "uart.h"

#include <stdint.h>

#include <stm32f1xx_ll_bus.h>
#include <stm32f1xx_ll_gpio.h>
#include <stm32f1xx_ll_rcc.h>

#include <circual_buffer.h>

extern volatile CirBuff_T cb_uart1_tx;
extern volatile CirBuff_T cb_uart1_rx;

void UART1_Init(void)
{
   LL_USART_InitTypeDef USART_InitStruct = {0};
   LL_GPIO_InitTypeDef  GPIO_InitStruct = {0};

   /* Peripheral clock enable */
   LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

   LL_AHB1_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
   /**USART1 GPIO Configuration
   PA9   ------> USART1_TX
   PA10   ------> USART1_RX
   */
   GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
   GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
   GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
   GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
   GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
   // GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
   LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

   /* USART1 interrupt Init */
   NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
   NVIC_EnableIRQ(USART1_IRQn);

   /* USER CODE BEGIN USART1_Init 1 */

   /* USER CODE END USART1_Init 1 */
   LL_USART_Disable(USART1);
   USART_InitStruct.BaudRate = 115200;
   USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
   USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
   USART_InitStruct.Parity = LL_USART_PARITY_NONE;
   USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
   USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
   USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
   LL_USART_Init(USART1, &USART_InitStruct);
   LL_USART_ConfigAsyncMode(USART1);
   LL_USART_Enable(USART1);

   // LL_USART_EnableIT_TXE(USART1);
   LL_USART_EnableIT_RXNE(USART1);
   LL_USART_EnableIT_ERROR(USART1);
   // LL_USART_EnableIT_TC(USART1);
}

void USART2_Init(void)
{
   LL_USART_InitTypeDef USART_InitStruct = {0};
   LL_GPIO_InitTypeDef  GPIO_InitStruct = {0};

   /* Peripheral clock enable */
   LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
   LL_AHB1_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

   GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
   GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
   GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
   GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
   GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;

   LL_GPIO_Init(GPIOA, &GPIO_InitStruct);


   LL_USART_Disable(USART2);
   USART_InitStruct.BaudRate = 115200;
   USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
   USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
   USART_InitStruct.Parity = LL_USART_PARITY_NONE;
   USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
   USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
   USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
   LL_USART_Init(USART2, &USART_InitStruct);
   LL_USART_ConfigAsyncMode(USART2);
   LL_USART_EnableHalfDuplex(USART2);
   LL_USART_Enable(USART2);
}

void USARTx_Set_BaudRate(USART_TypeDef *USARTx, uint32_t baudRate)
{
   LL_RCC_ClocksTypeDef all_clk;
   uint32_t             clock = 0U;

   LL_RCC_GetSystemClocksFreq(&all_clk);
   if (USARTx == USART1)
   {
      clock = all_clk.PCLK1_Frequency;
   }
   else if (USARTx == USART2 || USARTx == USART3)
   {
      clock = all_clk.PCLK2_Frequency;
   }

   LL_USART_SetBaudRate(USARTx, clock, baudRate);
}

void USARTx_Tx(USART_TypeDef *USARTx, uint8_t *data, uint8_t n)
{
   uint32_t i = 0U;
   for (i = 0; i < n; i++)
   {
      LL_USART_TransmitData8(USARTx, data[i]);
      while (!LL_USART_IsActiveFlag_TC(USARTx))
      {
      }
   }
}

void USARTx_Rx(USART_TypeDef *USARTx, uint8_t *data, uint8_t n)
{
   uint32_t i = 0U;
   for (i = 0; i < n; i++)
   {
      while (!LL_USART_IsActiveFlag_RXNE(USARTx))
      {
      }
      data[i] = LL_USART_ReceiveData8(USARTx);
   }
}

void USART1_IRQHandler(void)
{
   if (LL_USART_IsActiveFlag_RXNE(USART1))
   {

      if (cb_uart1_rx.head == cb_uart1_rx.size)
      {
         cb_uart1_rx.head = 0;
      }
      cb_uart1_rx.data[cb_uart1_rx.head] = LL_USART_ReceiveData8(USART1);
      cb_uart1_rx.head++;
   }

   if (LL_USART_IsActiveFlag_TXE(USART1))
   {
   }

   if (LL_USART_IsActiveFlag_TC(USART1))
   {
      if (cb_uart1_tx.tail != cb_uart1_tx.head)
      {
         LL_USART_TransmitData8(USART1, cb_uart1_tx.data[cb_uart1_tx.tail]);
         cb_uart1_tx.tail++;
      }

      if (cb_uart1_tx.tail == CIRCUAL_BUFFER_SIZE)
      {
         cb_uart1_tx.tail = 0;
      }

      if (cb_uart1_tx.tail == cb_uart1_tx.head)
      {
         LL_USART_DisableIT_TC(USART1);
      }
   }
}
