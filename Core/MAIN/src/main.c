#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include <stm32f1xx_ll_gpio.h>
#include <stm32f1xx_ll_spi.h>

#include <WS25Qxx.h>
#include <cc1101.h>
#include <circual_buffer.h>
#include <delay.h>
#include <device_info.h>
#include <ds18b20.h>
#include <gpio.h>
#include <hw_monitor.h>
#include <i2c.h>
#include <log.h>
#include <one_wire.h>
#include <pwm.h>
#include <rtc.h>
#include <spi.h>
#include <uart.h>


/* Dummy device */
static const struct device main_dev = {
   .name = "MAIN",
};


uint8_t address[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};

volatile uint32_t  irq_aux_nr = 0;
volatile uint32_t  irq_uart_nr = 0;
uint8_t            irq_buff[10];
static uint32_t    old_ts_ms = 0;
volatile CirBuff_T cb_uart1_tx = {.tail = 0,
                                  .head = 0,
                                  .size = CIRCUAL_BUFFER_SIZE,
                                  .USARTx = USART1};

volatile CirBuff_T cb_uart1_rx = {.tail = 0,
                                  .head = 0,
                                  .size = CIRCUAL_BUFFER_SIZE,
                                  .USARTx = USART1};

void SystemClock_Config(void);

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
   /* Reset of all peripherals, Initializes the Flash interface and the Systick.
    */
   LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
   LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

   /* System interrupt init*/
   NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

   /* SysTick_IRQn interrupt configuration */
   NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));

   /* NOJTAG: JTAG-DP Disabled and SW-DP Enabled */
   LL_GPIO_AF_Remap_SWJ_NOJTAG();

   /* Configure the system clock */
   SystemClock_Config();
   SysTick_Config(SystemCoreClock / 1000);
   LL_SYSTICK_EnableIT();

   /* Initialize all configured peripherals */
   HW_PVD_Start();
   MX_GPIO_Init();
   // PWM_Init();
   UART1_Init();
   TS_Delay_us_Init();
   RTC_Init();
   Device_Info();
   OneWire_Init();
   // WS25Qxx_Init();
   // if (I2C_DRV_STATUS_SUCCESS == I2C_Init(I2C2))
   // {
   //    log_info(&main_dev, "I2C OK\r\n");
   // }
   // else
   // {
   //    log_info(&main_dev, "I2C NOK\r\n");
   // }

   // #if defined(CC1101_TX)
   //    log_info(&main_dev, "CC1101 Tx\r\n");
   //    CC1101_Init(CC1101_TX_ADDRESS);
   // #endif

   // #if defined(CC1101_RX)
   //    log_info(&main_dev, "CC1101 Rx\r\n");
   //    CC1101_Init(CC1101_RX_ADDRESS);
   // #endif

   while (1)
   {

#ifdef CC1101_TX
      CC1101_Check_State();
      CC1101_Tx_Debug();
#endif

#ifdef CC1101_RX
      CC1101_Check_State();
      CC1101_Rx_Debug();
#endif

      if (TS_Get_ms() >= old_ts_ms + 500)
      {
         LL_GPIO_TogglePin(LED_Port, LED_Pin);
         DS18B20_Init();
         old_ts_ms = TS_Get_ms();
      }

      // Simple CMD
      if (cb_uart1_rx.head != cb_uart1_rx.tail)
      {
         if (cb_uart1_rx.data[cb_uart1_rx.tail] == 0x00)
         {
            WS25Qxx_Erase_Chip();
         }
         cb_uart1_rx.tail++;
      }
   }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
   LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
   while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
   {
   }
   LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
   while (LL_RCC_HSE_IsReady() != 1)
   {
   }
   LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
   LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
   while (LL_RCC_PLL_IsReady() != 1)
   {
   }
   LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
   LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
   LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
   LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
   while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
   {
   }
   LL_Init1msTick(72000000);
   LL_SetSystemCoreClock(72000000);
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
   /* USER CODE BEGIN Error_Handler_Debug */
   /* User can add his own implementation to report the HAL error return state */
   __disable_irq();
   while (1)
   {
   }
   /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
   (void) file;
   (void) line;
   // log_info(&main_dev, "Wrong parameters value: file %s on line %ld\r\n", file, line);
}
#endif /* USE_FULL_ASSERT */
