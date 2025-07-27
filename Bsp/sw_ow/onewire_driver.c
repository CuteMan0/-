#include "onewire_driver.h"

// 私有写 bit（标准写时序）
static void OW_WriteBit(ow_handle_t *ow, bool bit)
{
    ow->io_if.set_pin(0);
    if (bit)
    {
        ow->io_if.delay_us(6); // 写1：短拉低
        ow->io_if.set_pin(1);
        ow->io_if.delay_us(64);
    }
    else
    {
        ow->io_if.delay_us(60); // 写0：长拉低
        ow->io_if.set_pin(1);
        ow->io_if.delay_us(10);
    }
}

// 私有读 bit（标准读时序）
static bool OW_ReadBit(ow_handle_t *ow)
{
    bool bit;

    ow->io_if.set_pin(0);
    ow->io_if.delay_us(3); // 起始脉冲
    ow->io_if.set_pin(1);
    ow->io_if.delay_us(10);    // 等待从机应答
    bit = ow->io_if.get_pin(); // 读取 SDA 状态
    ow->io_if.delay_us(50);    // 等待 slot 结束

    return bit;
}

void OW_Init(ow_handle_t *ow)
{
    if (ow && ow->io_if.set_pin)
    {
        ow->io_if.set_pin(1); // 默认释放总线
    }
}

bool OW_Reset(ow_handle_t *ow)
{
    ow->io_if.set_pin(0);
    ow->io_if.delay_us(480); // 复位脉冲 ≥ 480us
    ow->io_if.set_pin(1);
    ow->io_if.delay_us(70); // 等待从机应答

    bool presence = !ow->io_if.get_pin(); // 从机拉低表示存在
    ow->io_if.delay_us(410);              // 总周期结束

    return presence;
}

void OW_WriteByte(ow_handle_t *ow, uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        OW_WriteBit(ow, byte & 0x01);
        byte >>= 1;
    }
}

uint8_t OW_ReadByte(ow_handle_t *ow)
{
    uint8_t byte = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        if (OW_ReadBit(ow))
        {
            byte |= (1 << i); // LSB first
        }
    }

    return byte;
}
