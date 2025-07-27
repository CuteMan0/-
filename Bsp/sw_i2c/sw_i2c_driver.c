#include "sw_i2c_driver.h"

#if I2C_SPEED_HZ == 100
#define I2C_DELAY_US 5
#elif I2C_SPEED_HZ == 400
#define I2C_DELAY_US 1
#else
#error "Unsupported I2C speed. Please use 100 or 400."
#endif

#define I2C_DELAY(h) (h)->io_if.delay_us(I2C_DELAY_US)
#define SCL(h, x) (h)->io_if.set_scl((x))
#define SDA(h, x) (h)->io_if.set_sda((x))
#define READ_SDA(h) (h)->io_if.get_sda()

static void I2C_Start(sw_i2c_handle_t *hi2c)
{
    SDA(hi2c, 1);
    SCL(hi2c, 1);
    I2C_DELAY(hi2c);

    SDA(hi2c, 0);
    I2C_DELAY(hi2c);
    SCL(hi2c, 0);
    I2C_DELAY(hi2c);
}

static void I2C_Stop(sw_i2c_handle_t *hi2c)
{
    SDA(hi2c, 0);
    I2C_DELAY(hi2c);
    SCL(hi2c, 1);
    I2C_DELAY(hi2c);
    SDA(hi2c, 1);
    I2C_DELAY(hi2c);
}

static void I2C_SendAck(sw_i2c_handle_t *hi2c, I2C_ACKTypedef ack)
{
    SDA(hi2c, ack);
    I2C_DELAY(hi2c);
    SCL(hi2c, 1);
    I2C_DELAY(hi2c);
    SCL(hi2c, 0);
    I2C_DELAY(hi2c);
    SDA(hi2c, 1);
    I2C_DELAY(hi2c);
}

static I2C_ACKTypedef I2C_ReceiveAck(sw_i2c_handle_t *hi2c)
{
    SDA(hi2c, 1); // 释放 SDA
    I2C_DELAY(hi2c);
    SCL(hi2c, 1);
    I2C_DELAY(hi2c);

    I2C_ACKTypedef ack = READ_SDA(hi2c);
    SCL(hi2c, 0);
    I2C_DELAY(hi2c);
    return ack;
}

static void I2C_WriteByte(sw_i2c_handle_t *hi2c, uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        SDA(hi2c, (data & 0x80));
        data <<= 1;
        I2C_DELAY(hi2c);
        SCL(hi2c, 1);
        I2C_DELAY(hi2c);
        SCL(hi2c, 0);
        I2C_DELAY(hi2c);
    }
    SDA(hi2c, 1); // 释放 SDA 等待 ACK
    I2C_DELAY(hi2c);
}

static uint8_t I2C_ReadByte(sw_i2c_handle_t *hi2c)
{
    uint8_t data = 0;
    SDA(hi2c, 1);
    for (uint8_t i = 0; i < 8; i++)
    {
        data <<= 1;
        SCL(hi2c, 1);
        I2C_DELAY(hi2c);
        if (READ_SDA(hi2c))
            data |= 0x01;
        SCL(hi2c, 0);
        I2C_DELAY(hi2c);
    }
    return data;
}

void SW_I2C_Init(sw_i2c_handle_t *hi2c)
{
    // 由上层初始化 I2C GPIO
    if (hi2c)
    {
        SCL(hi2c, 1);
        SDA(hi2c, 1);
    }
}

I2C_StatusTypeDef SW_I2C_Transmit(sw_i2c_handle_t *hi2c, uint8_t addr, uint8_t reg, const uint8_t *data, uint8_t len)
{
    if (!hi2c || !data || len == 0)
        return I2C_ERROR;

    I2C_Start(hi2c);
    I2C_WriteByte(hi2c, addr & 0xFE);
    if (I2C_ReceiveAck(hi2c))
    {
        I2C_Stop(hi2c);
        return I2C_NO_ACK_ON_ADDRESS;
    }

    I2C_WriteByte(hi2c, reg);
    if (I2C_ReceiveAck(hi2c))
    {
        I2C_Stop(hi2c);
        return I2C_NO_ACK_ON_REG;
    }

    for (uint8_t i = 0; i < len; i++)
    {
        I2C_WriteByte(hi2c, data[i]);
        if (I2C_ReceiveAck(hi2c))
        {
            I2C_Stop(hi2c);
            return I2C_NO_ACK_ON_DATA;
        }
    }

    I2C_Stop(hi2c);
    return I2C_OK;
}

I2C_StatusTypeDef SW_I2C_Receive(sw_i2c_handle_t *hi2c, uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len)
{
    if (!hi2c || !data || len == 0)
        return I2C_ERROR;

    I2C_Start(hi2c);
    I2C_WriteByte(hi2c, addr & 0xFE);
    if (I2C_ReceiveAck(hi2c))
    {
        I2C_Stop(hi2c);
        return I2C_NO_ACK_ON_ADDRESS;
    }

    I2C_WriteByte(hi2c, reg);
    if (I2C_ReceiveAck(hi2c))
    {
        I2C_Stop(hi2c);
        return I2C_NO_ACK_ON_REG;
    }

    I2C_Start(hi2c);
    I2C_WriteByte(hi2c, addr | 0x01);
    if (I2C_ReceiveAck(hi2c))
    {
        I2C_Stop(hi2c);
        return I2C_NO_ACK_ON_READ_ADDRESS;
    }

    for (uint8_t i = 0; i < len; i++)
    {
        data[i] = I2C_ReadByte(hi2c);
        I2C_SendAck(hi2c, (i == len - 1) ? NACK : ACK);
    }

    I2C_Stop(hi2c);
    return I2C_OK;
}
