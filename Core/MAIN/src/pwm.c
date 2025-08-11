/*
 * pwm.c
 *
 *  Created on: Jan 22, 2023
 *      Author: mkpk
 */

#include "pwm.h"

#include <stdio.h>

#include <stm32f1xx_ll_bus.h>
#include <stm32f1xx_ll_gpio.h>
#include <stm32f1xx_ll_tim.h>

#include <delay.h>

static uint32_t TimOutClock = 1;

void PWM_Init(void)
{
   LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA); /* Enable the peripheral clock of GPIOs */

   /* GPIO TIM2_CH1 configuration */
   LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_0, LL_GPIO_PULL_DOWN);
   LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_HIGH);

   LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_1, LL_GPIO_PULL_DOWN);
   LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_1, LL_GPIO_SPEED_FREQ_HIGH);

   LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_DOWN);
   LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_HIGH);

   LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_DOWN);
   LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_HIGH);

   LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_6, LL_GPIO_PULL_DOWN);
   LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_HIGH);

   LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);
   LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_7, LL_GPIO_PULL_DOWN);
   LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_HIGH);

   NVIC_SetPriority(TIM2_IRQn, 0); /* Configure the NVIC to handle TIM2 interrupt */
   NVIC_EnableIRQ(TIM2_IRQn);

   NVIC_SetPriority(TIM3_IRQn, 0); /* Configure the NVIC to handle TIM2 interrupt */
   NVIC_EnableIRQ(TIM3_IRQn);

   LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2); /* Enable the timer peripheral clock */
   LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3); /* Enable the timer peripheral clock */

   /***************************/
   /* Time base configuration */
   /***************************/
   /* Set counter mode */
   LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP); /* Reset value is LL_TIM_COUNTERMODE_UP */
   LL_TIM_SetCounterMode(TIM3, LL_TIM_COUNTERMODE_UP); /* Reset value is LL_TIM_COUNTERMODE_UP */

   /* Set the pre-scaler value to have TIM2 counter clock equal to 10 kHz */
   LL_TIM_SetPrescaler(TIM2, __LL_TIM_CALC_PSC(SystemCoreClock, 10000));
   LL_TIM_SetPrescaler(TIM3, __LL_TIM_CALC_PSC(SystemCoreClock, 10000));

   /* Enable TIM2_ARR register preload. Writing to or reading from the         */
   /* auto-reload register accesses the preload register. The content of the   */
   /* preload register are transferred into the shadow register at each update */
   /* event (UEV).                                                             */
   LL_TIM_EnableARRPreload(TIM2);
   LL_TIM_EnableARRPreload(TIM3);

   /* Set the auto-reload value to have a counter frequency of 100 Hz */
   /* TIM2CLK = SystemCoreClock / (APB prescaler & multiplier)               */
   TimOutClock = SystemCoreClock / 1;
   LL_TIM_SetAutoReload(TIM2, __LL_TIM_CALC_ARR(TimOutClock, LL_TIM_GetPrescaler(TIM2), 100));
   LL_TIM_SetAutoReload(TIM3, __LL_TIM_CALC_ARR(TimOutClock, LL_TIM_GetPrescaler(TIM3), 100));

   /*********************************/
   /* Output waveform configuration */
   /*********************************/
   /* Set output mode */
   /* Reset value is LL_TIM_OCMODE_FROZEN */
   LL_TIM_OC_SetMode(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
   LL_TIM_OC_SetMode(TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_OCMODE_PWM1);
   LL_TIM_OC_SetMode(TIM2, LL_TIM_CHANNEL_CH3, LL_TIM_OCMODE_PWM1);
   LL_TIM_OC_SetMode(TIM2, LL_TIM_CHANNEL_CH4, LL_TIM_OCMODE_PWM1);

   LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
   LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH2, LL_TIM_OCMODE_PWM1);

   /* Set output channel polarity */
   /* Reset value is LL_TIM_OCPOLARITY_HIGH */
   // LL_TIM_OC_SetPolarity(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_HIGH);

   /* Set compare value to half of the counter period (50% duty cycle ) */
   LL_TIM_OC_SetCompareCH1(TIM2, ((LL_TIM_GetAutoReload(TIM2) + 1) / 2));
   LL_TIM_OC_SetCompareCH2(TIM2, ((LL_TIM_GetAutoReload(TIM2) + 1) / 2));
   LL_TIM_OC_SetCompareCH3(TIM2, ((LL_TIM_GetAutoReload(TIM2) + 1) / 2));
   LL_TIM_OC_SetCompareCH4(TIM2, ((LL_TIM_GetAutoReload(TIM2) + 1) / 2));

   LL_TIM_OC_SetCompareCH1(TIM3, ((LL_TIM_GetAutoReload(TIM3) + 1) / 2));
   LL_TIM_OC_SetCompareCH2(TIM3, ((LL_TIM_GetAutoReload(TIM3) + 1) / 2));

   /* Enable TIM2_CCR1 register preload. Read/Write operations access the      */
   /* preload register. TIM2_CCR1 preload value is loaded in the active        */
   /* at each update event.                                                    */
   LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH1);
   LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH2);
   LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH3);
   LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH4);

   LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH1);
   LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH2);

   /* TIM2 interrupts set-up */
   // LL_TIM_EnableIT_CC1(TIM2);  /* Enable the capture/compare interrupt for channel 1*/
   // LL_TIM_EnableIT_CC2(TIM2);  /* Enable the capture/compare interrupt for channel 2*/
   // LL_TIM_EnableIT_CC3(TIM2);  /* Enable the capture/compare interrupt for channel 3*/
   // LL_TIM_EnableIT_CC4(TIM2);  /* Enable the capture/compare interrupt for channel 4*/

   /* Start output signal generation */
   //   LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1); /* Enable output channel 1 */
   //   LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH2); /* Enable output channel 2 */
   //   LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH3); /* Enable output channel 3 */
   //   LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH4); /* Enable output channel 4 */
   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH1);
   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH2);
   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH3);
   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH4);
   LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH1);
   LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH2);

   LL_TIM_EnableCounter(TIM2); /* Enable counter */
   // LL_TIM_EnableCounter(TIM3);                        /* Enable counter */
   LL_TIM_GenerateEvent_UPDATE(TIM2); /* Force update generation */
   // LL_TIM_GenerateEvent_UPDATE(TIM3);                 /* Force update generation */
}

void PWM_Update(void)
{
   //   uint32_t maxValue = LL_TIM_GetAutoReload(TIM2) + 1;
   //   static uint32_t value = 0;
   //
   //   if (value < maxValue)
   //   {
   //      LL_TIM_OC_SetCompareCH1(TIM2, value);
   //      LL_TIM_OC_SetCompareCH2(TIM2, value);
   //      LL_TIM_OC_SetCompareCH3(TIM2, value);
   //      LL_TIM_OC_SetCompareCH4(TIM2, value);
   //      value++;
   //      TS_Delay_ms(10);
   //   }
   //   else
   //   {
   //      value = 0;
   //   }

   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH1);
   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH2);
   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH3);
   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH4);
   LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH1);
   LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH2);
   TS_Delay_ms(700);
   LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH2);
   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH3);
   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH4);
   LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH1);
   LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH2);
   TS_Delay_ms(700);

   //   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH1);
   //   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH2);
   //   LL_TIM_CC_EnableChannel (TIM2, LL_TIM_CHANNEL_CH3);
   //   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH4);
   //   LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH1);
   //   LL_TIM_CC_EnableChannel (TIM3, LL_TIM_CHANNEL_CH2);
   //   TS_Delay_ms(700);
   //   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH1);
   //   LL_TIM_CC_EnableChannel (TIM2, LL_TIM_CHANNEL_CH2);
   //   LL_TIM_CC_EnableChannel (TIM2, LL_TIM_CHANNEL_CH3);
   //   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH4);
   //   LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH1);
   //   LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH2);
   //   TS_Delay_ms(700);

   //   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH1);
   //   LL_TIM_CC_EnableChannel (TIM2, LL_TIM_CHANNEL_CH2);
   //   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH3);
   //   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH4);
   //   LL_TIM_CC_EnableChannel (TIM3, LL_TIM_CHANNEL_CH1);
   //   LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH2);
   //   TS_Delay_ms(700);
   //   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH1);
   //   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH2);
   //   LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH3);
   //   LL_TIM_CC_EnableChannel (TIM2, LL_TIM_CHANNEL_CH4);
   //   LL_TIM_CC_EnableChannel (TIM3, LL_TIM_CHANNEL_CH1);
   //   LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH2);
   //   TS_Delay_ms(700);
}
