#ifndef __SSD1306_SW_I2C_H
#define __SSD1306_SW_I2C_H

#include "stm32f1xx_hal.h"

#define I2C_SPEED_HZ 100 // 支持设置为 100kbps 或 400kbps

typedef enum
{
    ACK = 0,
    NACK
} I2C_ACKTypedef;

typedef enum
{
    I2C_OK = 0,
    I2C_ERROR,
    I2C_NO_ACK_ON_ADDRESS,
    I2C_NO_ACK_ON_REG,
    I2C_NO_ACK_ON_DATA
} I2C_StatusTypeDef;

#define SCL_PORT GPIOA
#define SCL_PIN GPIO_PIN_1
#define SDA_PORT GPIOA
#define SDA_PIN GPIO_PIN_8

#define SCL(x) HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, (GPIO_PinState)(x))
#define SDA(x) HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, (GPIO_PinState)(x))
#define READ_SDA HAL_GPIO_ReadPin(SDA_PORT, SDA_PIN)

I2C_StatusTypeDef I2C_Transmit(uint8_t Salve_Address, uint8_t Reg_Address, uint8_t *pDATA, uint8_t Length);
I2C_StatusTypeDef I2C_Receive(uint8_t Salve_Address, uint8_t Reg_Address, uint8_t *pDATA, uint8_t Length);

void I2C_SW_Init(void);

#endif
