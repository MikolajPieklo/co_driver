/**
 ********************************************************************************
 * @file    i2c.h
 * @author  Mikolaj Pieklo
 * @date    12.10.2023
 * @brief   I2C driver.
 ********************************************************************************
 */

#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include <stdbool.h>
#include <stdint.h>

#include <stm32f103xb.h>

/************************************
 * MACROS AND DEFINES
 ************************************/
#define I2C_DRV_STATUSES                                                                           \
   I2C_DRV_STATUS(SUCCESS)                                                                         \
   I2C_DRV_STATUS(FAILURE)                                                                         \
   I2C_DRV_STATUS(FAILURE_TIMEOUT)                                                                 \
   I2C_DRV_STATUS(WRITE_FAILURE)                                                                   \
   I2C_DRV_STATUS(READ_FAILURE)                                                                    \
   I2C_DRV_STATUS(FAILURE_IS_NOT_INITIALIZED)                                                      \
   I2C_DRV_STATUS(FAILURE_OPERATION_NOT_SUPPORTED)

/************************************
 * TYPEDEFS
 ************************************/
typedef enum I2c_Drv_Status_Tag
{
#define I2C_DRV_STATUS(x) I2C_DRV_STATUS_##x,
   I2C_DRV_STATUSES
#undef I2C_DRV_STATUS
} I2c_Drv_Status_T;

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
I2c_Drv_Status_T I2C_Init(I2C_TypeDef *dev);

bool I2C_Master_Reg8_Transmit_Byte(I2C_TypeDef *dev, uint8_t address, uint8_t reg, uint8_t data);

bool I2C_Master_Reg16_Transmit_Byte(I2C_TypeDef *dev, uint8_t address, uint16_t reg, uint8_t data);

bool I2C_Master_Reg8_Transmit_Bytes(I2C_TypeDef *dev, uint8_t address, uint8_t reg, uint8_t *data,
                                    uint8_t len);

bool I2C_Master_Reg16_Transmit_Bytes(I2C_TypeDef *dev, uint8_t address, uint16_t reg, uint8_t *data,
                                     uint8_t len);

bool I2C_Master_Reg8_Recessive_Bytes(I2C_TypeDef *dev, uint8_t address, uint8_t reg, uint8_t *data,
                                     uint8_t len);

bool I2C_Master_Reg16_Recessive_Bytes(I2C_TypeDef *dev, uint8_t address, uint16_t reg,
                                      uint8_t *data, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif