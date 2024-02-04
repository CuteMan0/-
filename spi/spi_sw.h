#ifndef __SPI_SW_H
#define __SPI_SW_H

#include "stm32f1xx_hal.h"

#define SCK_Port  GPIOA
#define SCK_Pin   GPIO_PIN_5
#define MOSI_Port GPIOA
#define MOSI_Pin  GPIO_PIN_7
#define MISO_Port GPIOA
#define MISO_Pin  GPIO_PIN_1
#define RST_Port  GPIOB
#define RST_Pin   GPIO_PIN_0
#define DC_Port   GPIOB
#define DC_Pin    GPIO_PIN_1
#define CS_Port   GPIOA
#define CS_Pin    GPIO_PIN_0

#define SCK(x)    HAL_GPIO_WritePin(SCK_Port, SCK_Pin, (GPIO_PinState)(x))
#define MOSI(x)   HAL_GPIO_WritePin(MOSI_Port, MOSI_Pin, (GPIO_PinState)(x))
#define MISO(x)   HAL_GPIO_WritePin(MISO_Port, MISO_Pin, (GPIO_PinState)(x))
#define RST(x)    HAL_GPIO_WritePin(RST_Port, RST_Pin, (GPIO_PinState)(x))
#define DC(x)     HAL_GPIO_WritePin(DC_Port, DC_Pin, (GPIO_PinState)(x))
#define CS(x)     HAL_GPIO_WritePin(CS_Port, CS_Pin, (GPIO_PinState)(x))

#define MISO_GET  HAL_GPIO_ReadPin(MISO_Port, MISO_Pin)

void SPI_WriteByte(uint8_t SPI_Byte);
uint8_t SPI_ReadByte(void);

void SPI_Transmit(uint8_t *pDATA, uint8_t Length);
void SPI_Receive(uint8_t *pDATA, uint8_t Length);
void SPI_TransmitReceive(uint8_t *pTxDATA, uint8_t *pRxDATA, uint8_t Length);

void SW_SPI_Init(void);

#endif
