#ifndef __SSD1306_SW_I2C_H
#define __SSD1306_SW_I2C_H

#include "stm32f1xx_hal.h"

typedef enum {
    ACK = 0,
    NACK
}I2C_ACKTypedef;

typedef enum {
    I2C_OK      = 0x00U,
    I2C_ERROR   = 0x01U,
    I2C_BUSY    = 0x02U,
    I2C_TIMEOUT = 0x03U
} I2C_StatusTypeDef;

#define SCL_PORT GPIOA
#define SCL_PIN  GPIO_PIN_1
#define SDA_PORT GPIOA
#define SDA_PIN  GPIO_PIN_8

#define SCL(x)   HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, (GPIO_PinState)(x))
#define SDA(x)   HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, (GPIO_PinState)(x))
#define READ_SDA HAL_GPIO_ReadPin(SDA_PORT, SDA_PIN)

void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendAck(I2C_ACKTypedef ACK_Bit);
I2C_ACKTypedef I2C_ReceiveAck(void);

void I2C_WriteByte(uint8_t I2C_Byte);
uint8_t I2C_ReadByte(void);

I2C_StatusTypeDef I2C_Transmit(uint8_t Salve_Address, uint8_t *pDATA, uint8_t Length);
I2C_StatusTypeDef I2C_Receive(uint8_t Salve_Address, uint8_t *pDATA, uint8_t Length);

void I2C_SW_Init(void);

#endif
