/**
  ******************************************************************************
    主频不高时可用寄存器写法替换
    SCK(0)      <=>      SCK_Port->BSRR = SCK_Pin << 16u
    SCK(1)      <=>      SCK_Port->BSRR = SCK_Pin
    MOSI(0)     <=>      MOSI_Port->BSRR = MOSI_Pin << 16u
    MOSI(1)     <=>      MOSI_Port->BSRR = MOSI_Pin

    MISO_GET    <=>      MISO_Port->IDR & MISO_Pin
  ******************************************************************************
  */

#include "spi_sw.h"

/**
 * @brief  SPI发送一个字节
 * @param  SPI_Byte 待发送的字节
 * @retval 无
 */
void SPI_WriteByte(uint8_t SPI_Byte)
{
    // mode 0 / 3
    for (uint8_t i = 0; i < 8; i++) {
        SCK_Port->BSRR = SCK_Pin << 16u;
        if (SPI_Byte & (0x80 >> i))
            MOSI_Port->BSRR = MOSI_Pin;
        else
            MOSI_Port->BSRR = MOSI_Pin << 16u;
        SCK_Port->BSRR = SCK_Pin;
    }
}

/**
 * @brief  SPI发送两个字节
 * @param  SPI_DoubleByte 待发送的双字节
 * @retval 无
 */
void SPI_Write16bit(uint16_t SPI_DoubleByte)
{
    // mode 0 / 3
    for (uint8_t i = 0; i < 16; i++) {
        SCK_Port->BSRR = SCK_Pin << 16u;
        if (SPI_DoubleByte & (0x8000 >> i))
            MOSI_Port->BSRR = MOSI_Pin;
        else
            MOSI_Port->BSRR = MOSI_Pin << 16u;
        SCK_Port->BSRR = SCK_Pin;
    }
}

/**
 * @brief  SPI接收一个字节
 * @param  无
 * @retval SPI_Byte 已接收的字节
 */
uint8_t SPI_ReadByte(void) // 未验证
{
    // mode 0 / 3
    uint8_t SPI_Byte = 0x00;
    for (uint8_t i = 0; i < 8; i++) {
        SCK_Port->BSRR = SCK_Pin;

        if (MISO_Port->IDR & MISO_Pin)
            SPI_Byte |= (0x80 >> i);

        SCK_Port->BSRR = SCK_Pin << 16u;
    }
    return SPI_Byte;
}

/**
 * @brief  SPI发送数据
 * @param  pDATA    待发送数据的数组名
 * @param  Length   待发送数据的字节长度
 * @retval 无
 */
void SPI_Transmit(uint8_t *pDATA, uint8_t Length)
{
    for (uint8_t i = 0; i < Length; i++) {
        SPI_WriteByte(pDATA[i]);
    }
}

/**
 * @brief  SPI发送数据（16位）
 * @param  pDATA    待发送数据的数组名
 * @param  Length   待发送数据的字节长度
 * @retval 无
 */
void SPI_Transmit_16bit(uint16_t *pDATA, uint8_t Length)
{
    for (uint8_t i = 0; i < Length; i++) {
        SPI_Write16bit(pDATA[i]);
    }
}

/**
 * @brief  SPI接收数据
 * @param  pDATA    预接收数据的数组名
 * @param  Length   预接收数据的字节长度
 * @retval 无
 */
void SPI_Receive(uint8_t *pDATA, uint8_t Length) // 未验证
{
    for (uint8_t i = 0; i < Length; i++) {
        pDATA[i] = SPI_ReadByte();
    }
}

/**
 * @brief  SPI交换数据
 * @param  pTxDATA    待发送数据的数组名
 * @param  pRxDATA    预接收数据的数组名
 * @param  Length     交换数据的字节长度
 * @retval 无
 */
void SPI_TransmitReceive(uint8_t *pTxDATA, uint8_t *pRxDATA, uint8_t Length)
{
    for (uint8_t i = 0; i < Length; i++) {
        SPI_WriteByte(pTxDATA[i]);
        pRxDATA[i] = SPI_ReadByte();
    }
}

/**
 * @brief  SPI初始化
 * @param  无
 * @retval 无
 */
void SW_SPI_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin              = SCK_Pin | MOSI_Pin;
    GPIO_InitStruct.Mode             = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull             = GPIO_NOPULL;
    GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SCK_Port, &GPIO_InitStruct);

    HAL_GPIO_WritePin(SCK_Port, SCK_Pin | MOSI_Pin, 1);
}
