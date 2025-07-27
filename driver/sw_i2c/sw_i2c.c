#include "i2c_sw.h"

#if I2C_SPEED_HZ == 100
#define I2C_DELAY_CYCLES 5 // 粗略设定为 5 微秒延时
#elif I2C_SPEED_HZ == 400
#define I2C_DELAY_CYCLES 1 // 粗略设定为 1 微秒延时
#else
#error "Unsupported I2C speed. Please use 100 or 400."
#endif

// #include "delay.h"
#define I2C_DELAY() delay_us(I2C_DELAY_CYCLES)

/**
 * @brief  I2C开始
 * @param  无
 * @retval 无
 */
void I2C_Start(void)
{
    SDA(1);
    SCL(1);
    I2C_DELAY();

    SDA(0);
    I2C_DELAY();

    SCL(0);
    I2C_DELAY();
}

/**
 * @brief  I2C停止
 * @param  无
 * @retval 无
 */
void I2C_Stop(void)
{
    SDA(0);
    I2C_DELAY();

    SCL(1);
    I2C_DELAY();

    SDA(1);
    I2C_DELAY();
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
    I2C_DELAY();

    SCL(1);
    I2C_DELAY();

    SCL(0);
    I2C_DELAY();

    SDA(1); // 释放 SDA
    I2C_DELAY();
}

/**
 * @brief  I2C接收从机ACK
 * @param  无
 * @retval ACK_Bit 从机ACK信号
 */
I2C_ACKTypedef I2C_ReceiveAck(void)
{
    I2C_ACKTypedef ACK_Bit;

    SDA(1); // 主机释放 SDA
    I2C_DELAY();

    SCL(1);
    I2C_DELAY();

    ACK_Bit = READ_SDA;

    SCL(0);
    I2C_DELAY();

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
    SCL(0);
    I2C_DELAY();

    for (i = 0; i < 8; i++)
    {
        if (I2C_Byte & (0x80 >> i))
            SDA(1);
        else
            SDA(0);
        I2C_DELAY();

        SCL(1);
        I2C_DELAY();

        SCL(0);
        I2C_DELAY();
    }

    SDA(1); // 释放 SDA 等待 ACK
    I2C_DELAY();
}

/**
 * @brief  I2C接收一个字节
 * @param  无
 * @retval I2C_Byte 已接收的字节
 */
uint8_t I2C_ReadByte(void)
{
    uint8_t i, I2C_Byte = 0x00;

    SDA(1); // 主机释放 SDA
    I2C_DELAY();

    for (i = 0; i < 8; i++)
    {
        SCL(1);
        I2C_DELAY();

        if (READ_SDA)
            I2C_Byte |= (0x80 >> i);

        SCL(0);
        I2C_DELAY();
    }

    return I2C_Byte;
}

/**
 * @brief  模拟I2C发送数据（带寄存器地址）
 * @param  Slave_Address 左移1位的从机地址（例如0x40<<1）
 * @param  Reg_Address   寄存器地址
 * @param  pDATA         待发送数据缓冲区
 * @param  Length        待发送字节数
 * @retval I2C状态码
 */
I2C_StatusTypeDef I2C_Transmit(uint8_t Slave_Address, uint8_t Reg_Address, const uint8_t *pDATA, uint8_t Length)
{
    if (pDATA == NULL || Length == 0)
        return I2C_ERROR;

    I2C_Start();

    // 发送从机地址（写）
    I2C_WriteByte(Slave_Address & 0xFE);
    if (I2C_ReceiveAck())
    {
        I2C_Stop();
        return I2C_NO_ACK_ON_ADDRESS;
    }

    // 发送寄存器地址
    I2C_WriteByte(Reg_Address);
    if (I2C_ReceiveAck())
    {
        I2C_Stop();
        return I2C_NO_ACK_ON_REG;
    }

    // 发送数据
    for (uint8_t i = 0; i < Length; i++)
    {
        I2C_WriteByte(pDATA[i]);
        if (I2C_ReceiveAck())
        {
            I2C_Stop();
            return I2C_NO_ACK_ON_DATA;
        }
    }

    I2C_Stop();
    return I2C_OK;
}

/**
 * @brief  模拟I2C发送数据（带寄存器地址）
 * @param  Slave_Address 左移1位的从机地址（例如0x40<<1）
 * @param  Reg_Address   寄存器地址
 * @param  pDATA         待接受数据缓冲区
 * @param  Length        待接受字节数
 * @retval I2C状态码
 */
I2C_StatusTypeDef I2C_Receive(uint8_t Slave_Address, uint8_t Reg_Address, uint8_t *pDATA, uint8_t Length)
{
    if (pDATA == NULL || Length == 0)
        return I2C_ERROR;

    // 发送从机地址（写）
    I2C_Start();
    I2C_WriteByte(Slave_Address & 0xFE); // 写地址
    if (I2C_ReceiveAck())
    {
        I2C_Stop();
        return I2C_NO_ACK_ON_ADDRESS;
    }

    // 发送寄存器地址
    I2C_WriteByte(Reg_Address);
    if (I2C_ReceiveAck())
    {
        I2C_Stop();
        return I2C_NO_ACK_ON_REG;
    }

    // 重起始 + 从机地址（读）
    I2C_Start();
    I2C_WriteByte(Slave_Address | 0x01); // 读地址
    if (I2C_ReceiveAck())
    {
        I2C_Stop();
        return I2C_NO_ACK_ON_READ_ADDRESS;
    }

    // 读取数据
    for (uint8_t i = 0; i < Length; i++)
    {
        pDATA[i] = I2C_ReadByte();
        if (i == Length - 1)
            I2C_SendAck(NACK); // 最后一个字节，发NACK
        else
            I2C_SendAck(ACK); // 其他字节，发ACK
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
    __HAL_RCC_GPIOA_CLK_ENABLE();
    // __HAL_RCC_GPIOB_CLK_ENABLE();
    // __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SCL_PIN | SDA_PIN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(SCL_PORT, &GPIO_InitStruct);
    // HAL_GPIO_Init(SDA_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN | SDA_PIN, 1);
}
