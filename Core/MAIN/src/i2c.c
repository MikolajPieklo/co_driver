/**
 ********************************************************************************
 * @file    i2c.c
 * @author  Mikolaj Pieklo
 * @date    12.10.2023
 * @brief
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "i2c.h"

#include <stm32f1xx_ll_bus.h>
#include <stm32f1xx_ll_gpio.h>
#include <stm32f1xx_ll_i2c.h>
#include <stm32f1xx_ll_rcc.h>

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define I2C2_SCL_PIN    LL_GPIO_PIN_10
#define I2C2_SDA_PIN    LL_GPIO_PIN_11
#define I2C2_SPEEDCLOCK 400000
#define I2C2_DUTYCYCLE  LL_I2C_DUTYCYCLE_2

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
I2c_Drv_Status_T I2C_Init(I2C_TypeDef *dev)
{
   I2c_Drv_Status_T     status = I2C_DRV_STATUS_SUCCESS;
   LL_RCC_ClocksTypeDef rcc_clocks;

   do
   {
      if (I2C2 == dev)
      {
         LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
         LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);

         LL_GPIO_SetPinMode(GPIOB, I2C2_SCL_PIN, LL_GPIO_MODE_ALTERNATE);
         LL_GPIO_SetPinSpeed(GPIOB, I2C2_SCL_PIN, LL_GPIO_SPEED_FREQ_HIGH);
         LL_GPIO_SetPinOutputType(GPIOB, I2C2_SCL_PIN, LL_GPIO_OUTPUT_OPENDRAIN);
         LL_GPIO_SetPinPull(GPIOB, I2C2_SCL_PIN, LL_GPIO_PULL_UP);

         LL_GPIO_SetPinMode(GPIOB, I2C2_SDA_PIN, LL_GPIO_MODE_ALTERNATE);
         LL_GPIO_SetPinSpeed(GPIOB, I2C2_SDA_PIN, LL_GPIO_SPEED_FREQ_HIGH);
         LL_GPIO_SetPinOutputType(GPIOB, I2C2_SDA_PIN, LL_GPIO_OUTPUT_OPENDRAIN);
         LL_GPIO_SetPinPull(GPIOB, I2C2_SDA_PIN, LL_GPIO_PULL_UP);
      }

      LL_I2C_Disable(dev);
      LL_RCC_GetSystemClocksFreq(&rcc_clocks);
      LL_I2C_ConfigSpeed(dev, rcc_clocks.PCLK1_Frequency, I2C2_SPEEDCLOCK, I2C2_DUTYCYCLE);
      LL_I2C_SetClockSpeedMode(dev, LL_I2C_CLOCK_SPEED_STANDARD_MODE);
      LL_I2C_SetMode(dev, LL_I2C_MODE_I2C);
      LL_I2C_Enable(dev);
   } while (0);

   return status;
}

bool I2C_Master_Reg8_Transmit_Byte(I2C_TypeDef *dev, uint8_t address, uint8_t reg, uint8_t data)
{
   bool status = true;

   // start
   LL_I2C_GenerateStartCondition(dev);
   while (!LL_I2C_IsActiveFlag_SB(dev))
   {
   }

   // address
   LL_I2C_TransmitData8(dev, address | 0);
   while (!LL_I2C_IsActiveFlag_ADDR(dev))
   {
   }
   LL_I2C_ClearFlag_ADDR(dev);

   // reg
   LL_I2C_TransmitData8(dev, reg);
   while (!LL_I2C_IsActiveFlag_TXE(dev))
   {
   }

   // data
   LL_I2C_TransmitData8(dev, data);
   while (!LL_I2C_IsActiveFlag_TXE(dev))
   {
   }

   LL_I2C_GenerateStopCondition(dev);
   while (LL_I2C_IsActiveFlag_STOP(dev))
   {
   }

   return status;
}

bool I2C_Master_Reg16_Transmit_Byte(I2C_TypeDef *dev, uint8_t address, uint16_t reg, uint8_t data)
{
   bool status = true;

   // start
   LL_I2C_GenerateStartCondition(dev);
   while (!LL_I2C_IsActiveFlag_SB(dev))
   {
   }

   // address
   LL_I2C_TransmitData8(dev, address | 0);
   while (!LL_I2C_IsActiveFlag_ADDR(dev))
   {
   }
   LL_I2C_ClearFlag_ADDR(dev);

   // reg
   LL_I2C_TransmitData8(dev, (uint8_t)(reg >> 8));
   while (!LL_I2C_IsActiveFlag_TXE(dev))
   {
   }
   LL_I2C_TransmitData8(dev, (uint8_t)(reg & 0xFF));
   while (!LL_I2C_IsActiveFlag_TXE(dev))
   {
   }

   // data
   LL_I2C_TransmitData8(dev, data);
   while (!LL_I2C_IsActiveFlag_TXE(dev))
   {
   }

   LL_I2C_GenerateStopCondition(dev);
   while (LL_I2C_IsActiveFlag_STOP(dev))
   {
   }

   return status;
}

bool I2C_Master_Reg8_Transmit_Bytes(I2C_TypeDef *dev, uint8_t address, uint8_t reg, uint8_t *data,
                                    uint8_t len)
{
   bool    status = true;
   uint8_t idx = 0;

   // start
   LL_I2C_GenerateStartCondition(dev);
   while (!LL_I2C_IsActiveFlag_SB(dev))
   {
   }

   // address
   LL_I2C_TransmitData8(dev, address | 0);
   while (!LL_I2C_IsActiveFlag_ADDR(dev))
   {
   }
   LL_I2C_ClearFlag_ADDR(dev);

   // reg
   LL_I2C_TransmitData8(dev, reg);
   while (!LL_I2C_IsActiveFlag_TXE(dev))
   {
   }

   // data
   for (idx = 0; idx < len; idx++)
   {
      LL_I2C_TransmitData8(dev, *(data + idx));
      while (!LL_I2C_IsActiveFlag_TXE(dev))
      {
      }
   }

   // Stop
   LL_I2C_GenerateStopCondition(dev);
   while (LL_I2C_IsActiveFlag_STOP(dev))
   {
   }

   return status;
}

bool I2C_Master_Reg16_Transmit_Bytes(I2C_TypeDef *dev, uint8_t address, uint16_t reg, uint8_t *data,
                                     uint8_t len)
{
   bool    status = true;
   uint8_t idx = 0;

   // start
   LL_I2C_GenerateStartCondition(dev);
   while (!LL_I2C_IsActiveFlag_SB(dev))
   {
   }

   // address
   LL_I2C_TransmitData8(dev, address | 0);
   while (!LL_I2C_IsActiveFlag_ADDR(dev))
   {
   }
   LL_I2C_ClearFlag_ADDR(dev);

   // reg
   LL_I2C_TransmitData8(dev, (uint8_t)(reg >> 8));
   while (!LL_I2C_IsActiveFlag_TXE(dev))
   {
   }
   LL_I2C_TransmitData8(dev, (uint8_t)(reg & 0xFF));
   while (!LL_I2C_IsActiveFlag_TXE(dev))
   {
   }

   // data
   for (idx = 0; idx < len; idx++)
   {
      LL_I2C_TransmitData8(dev, *(data + idx));
      while (!LL_I2C_IsActiveFlag_TXE(dev))
      {
      }
   }

   // Stop
   LL_I2C_GenerateStopCondition(dev);
   while (LL_I2C_IsActiveFlag_STOP(dev))
   {
   }

   return status;
}

bool I2C_Master_Reg8_Recessive_Bytes(I2C_TypeDef *dev, uint8_t address, uint8_t reg, uint8_t *data,
                                     uint8_t len)
{
   bool    status = true;
   uint8_t idx = 0;

   LL_I2C_AcknowledgeNextData(dev, LL_I2C_ACK);

   // start
   LL_I2C_GenerateStartCondition(dev);
   while (!LL_I2C_IsActiveFlag_SB(dev))
   {
   }

   // address
   LL_I2C_TransmitData8(dev, address | 0);
   while (!LL_I2C_IsActiveFlag_ADDR(dev))
   {
   }
   LL_I2C_ClearFlag_ADDR(dev);

   // reg
   LL_I2C_TransmitData8(dev, reg);
   while (!LL_I2C_IsActiveFlag_TXE(dev))
   {
   }

   // Stop
   LL_I2C_GenerateStopCondition(dev);
   while (LL_I2C_IsActiveFlag_STOP(dev))
   {
   }

   // second start
   LL_I2C_GenerateStartCondition(dev);
   while (!LL_I2C_IsActiveFlag_SB(dev))
   {
   }

   // address
   LL_I2C_TransmitData8(dev, address | 1); // Read
   while (!LL_I2C_IsActiveFlag_ADDR(dev))
   {
   }
   LL_I2C_ClearFlag_ADDR(dev);

   // LL_I2C_AcknowledgeNextData(dev, LL_I2C_ACK);

   // data
   for (idx = 0; idx < len; idx++)
   {
      if (idx == len - 1)
      {
         LL_I2C_AcknowledgeNextData(dev, LL_I2C_NACK);
      }
      else
      {
         LL_I2C_AcknowledgeNextData(dev, LL_I2C_ACK);
      }

      while (!LL_I2C_IsActiveFlag_RXNE(dev))
      {
      }
      *(data + idx) = LL_I2C_ReceiveData8(dev);
   }

   // Stop
   LL_I2C_GenerateStopCondition(dev);
   while (LL_I2C_IsActiveFlag_STOP(dev))
   {
   }

   return status;
}

bool I2C_Master_Reg16_Recessive_Bytes(I2C_TypeDef *dev, uint8_t address, uint16_t reg,
                                      uint8_t *data, uint8_t len)
{
   bool    status = true;
   uint8_t idx = 0;

   LL_I2C_AcknowledgeNextData(dev, LL_I2C_ACK);

   // start
   LL_I2C_GenerateStartCondition(dev);
   while (!LL_I2C_IsActiveFlag_SB(dev))
   {
   }

   // address
   LL_I2C_TransmitData8(dev, address | 0);
   while (!LL_I2C_IsActiveFlag_ADDR(dev))
   {
   }
   LL_I2C_ClearFlag_ADDR(dev);

   // reg
   LL_I2C_TransmitData8(dev, (uint8_t)(reg >> 8));
   while (!LL_I2C_IsActiveFlag_TXE(dev))
   {
   }
   LL_I2C_TransmitData8(dev, (uint8_t)(reg & 0xFF));
   while (!LL_I2C_IsActiveFlag_TXE(dev))
   {
   }

   // Stop
   LL_I2C_GenerateStopCondition(dev);
   while (LL_I2C_IsActiveFlag_STOP(dev))
   {
   }

   // Second start
   LL_I2C_GenerateStartCondition(dev);
   while (!LL_I2C_IsActiveFlag_SB(dev))
   {
   }

   // Address
   LL_I2C_TransmitData8(dev, address | 1); // Read
   while (!LL_I2C_IsActiveFlag_ADDR(dev))
   {
   }
   LL_I2C_ClearFlag_ADDR(dev);

   // LL_I2C_AcknowledgeNextData(dev, LL_I2C_ACK);

   // data
   for (idx = 0; idx < len; idx++)
   {
      if (idx == len - 1)
      {
         LL_I2C_AcknowledgeNextData(dev, LL_I2C_NACK);
      }
      else
      {
         LL_I2C_AcknowledgeNextData(dev, LL_I2C_ACK);
      }

      while (!LL_I2C_IsActiveFlag_RXNE(dev))
      {
      }
      *(data + idx) = LL_I2C_ReceiveData8(dev);
      idx++;
   }

   // Stop
   LL_I2C_GenerateStopCondition(dev);
   while (LL_I2C_IsActiveFlag_STOP(dev))
   {
   }

   return status;
}
