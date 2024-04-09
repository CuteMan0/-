#include "i2c_sw.h"

/**
 * @brief  I2C开始
 * @param  无
 * @retval 无
 */
void I2C_Start(void)
{
    SDA(1);
    SCL(1);
    SDA(0);
    SCL(0);
}

/**
 * @brief  I2C停止
 * @param  无
 * @retval 无
 */
void I2C_Stop(void)
{
    SDA(0);
    SCL(1);
    SDA(1);
}

/**
 * @brief  I2C发送ACK
 * @param  ACK_Bit 发送的ACK位，参考
 * @retval 无
 */
void I2C_SendAck(I2C_ACKTypedef ACK_Bit)
{
    if (ACK_Bit)
        SDA(1);
    else
        SDA(0);
    SCL(1);
    SCL(0);
}

/**
 * @brief  I2C接收从机ACK
 * @param  无
 * @retval ACK_Bit 从机ACK信号
 */
I2C_ACKTypedef I2C_ReceiveAck(void)
{
    I2C_ACKTypedef ACK_Bit;
    SDA(1); // 主机释放，从机接管
    SCL(1);
    ACK_Bit = READ_SDA;
    SCL(0);
    return ACK_Bit;
}

/**
 * @brief  I2C发送一个字节
 * @param  I2C_Byte 待发送的字节
 * @retval 无
 */
void I2C_WriteByte(uint8_t I2C_Byte)
{
    uint8_t i;
    uint8_t data = I2C_Byte;
    SCL(0);
    for (i = 0; i < 8; i++) {
        if (data & (0x80 >> i))
            SDA(1);
        else
            SDA(0);
        SCL(1);
        SCL(0);
    }
}

/**
 * @brief  I2C接收一个字节
 * @param  无
 * @retval I2C_Byte 已接收的字节
 */
uint8_t I2C_ReadByte(void)
{
    uint8_t i, I2C_Byte = 0x00;
    SDA(1); // 主机释放，从机接管
    for (i = 0; i < 8; i++) {
        SCL(1);
        if (READ_SDA)
            I2C_Byte |= (0x80 >> i);
        SCL(0);
    }
    return I2C_Byte;
}

/**
 * @brief  I2C发送数据
 * @param  Salve_Address 左移一位的从机地址
 * @param  pDATA		 待发送数据的数组名
 * @param  Length		 待发送数据的字节长度
 * @retval I2C接收数据状态
 */
I2C_StatusTypeDef I2C_Transmit(uint8_t Salve_Address, uint8_t *pDATA, uint8_t Length)
{
    uint8_t Address_Send = 1;

    I2C_Start();

    if (Address_Send) {
        I2C_WriteByte(Salve_Address & 0xFE);
        while (I2C_ReceiveAck()) {
            return I2C_ERROR; // 从机未响应
        }
        Address_Send -= 1;
    }

    for (uint8_t i = 0; i < Length; i++) {
        I2C_WriteByte(pDATA[i]);
        while (I2C_ReceiveAck()) {
            return I2C_ERROR; // 从机未响应
        }
    }

    I2C_Stop();
    return I2C_OK;
}

/**
 * @brief  I2C接收数据
 * @param  Salve_Address 左移一位的从机地址
 * @param  pDATA		 预接收数据的数组名
 * @param  Length		 预接收数据的字节长度
 * @retval I2C接收数据状态
 */
I2C_StatusTypeDef I2C_Receive(uint8_t Salve_Address, uint8_t *pDATA, uint8_t Length)
{
    uint8_t Address_Send = 1;

    I2C_Start();

    if (Address_Send) {
        I2C_WriteByte(Salve_Address | 0x01);
        while (I2C_ReceiveAck()) {
            return I2C_ERROR; // 从机未响应
        }
        Address_Send -= 1;
    }

    for (uint8_t i = 0; i < Length; i++) {
        pDATA[i] = I2C_ReadByte();
        if (i == Length - 1)
            I2C_SendAck(NACK);
        I2C_SendAck(ACK);
    }

    I2C_Stop();

    return I2C_OK;
}

/**
 * @brief  I2C引脚初始化
 * @param  无
 * @retval 无
 */
void I2C_SW_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin              = SCL_PIN | SDA_PIN;
    GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Mode             = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(SCL_PORT, &GPIO_InitStruct);
    // HAL_GPIO_Init(SDA_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN | SDA_PIN, 1);
}
