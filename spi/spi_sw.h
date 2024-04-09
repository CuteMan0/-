#ifndef __SPI_SW_H
#define __SPI_SW_H

#include "stm32f1xx_hal.h"

#define SCK_PORT  GPIOA
#define SCK_PIN   GPIO_PIN_5
#define MOSI_PORT GPIOA
#define MOSI_PIN  GPIO_PIN_7
#define MISO_PORT GPIOA
#define MISO_PIN  GPIO_PIN_1
#define RST_PORT  GPIOB
#define RST_PIN   GPIO_PIN_0
#define DC_PORT   GPIOB
#define DC_PIN    GPIO_PIN_1
#define CS_PORT   GPIOA
#define CS_PIN    GPIO_PIN_0

#define SCK(x)    HAL_GPIO_WritePin(SCK_PORT, SCK_PIN, (GPIO_PinState)(x))
#define MOSI(x)   HAL_GPIO_WritePin(MOSI_PORT, MOSI_PIN, (GPIO_PinState)(x))
#define MISO(x)   HAL_GPIO_WritePin(MISO_PORT, MISO_PIN, (GPIO_PinState)(x))
#define RST(x)    HAL_GPIO_WritePin(RST_PORT, RST_PIN, (GPIO_PinState)(x))
#define DC(x)     HAL_GPIO_WritePin(DC_PORT, DC_PIN, (GPIO_PinState)(x))
#define CS(x)     HAL_GPIO_WritePin(CS_PORT, CS_PIN, (GPIO_PinState)(x))

#define MISO_GET  HAL_GPIO_ReadPin(MISO_PORT, MISO_PIN)

void SPI_WriteByte(uint8_t SPI_Byte);
void SPI_Write16bit(uint16_t SPI_DoubleByte);
uint8_t SPI_ReadByte(void);

void SPI_Transmit(uint8_t *pDATA, uint8_t Length);
void SPI_Transmit_16bit(uint16_t *pDATA, uint8_t Length);
void SPI_Receive(uint8_t *pDATA, uint8_t Length);
void SPI_TransmitReceive(uint8_t *pTxDATA, uint8_t *pRxDATA, uint8_t Length);

void SW_SPI_Init(void);

#endif
