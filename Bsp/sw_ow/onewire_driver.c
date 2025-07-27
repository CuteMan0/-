#include "onewire_driver.h"

// ˽��д bit����׼дʱ��
static void OW_WriteBit(ow_handle_t *ow, bool bit)
{
    ow->io_if.set_pin(0);
    if (bit)
    {
        ow->io_if.delay_us(6); // д1��������
        ow->io_if.set_pin(1);
        ow->io_if.delay_us(64);
    }
    else
    {
        ow->io_if.delay_us(60); // д0��������
        ow->io_if.set_pin(1);
        ow->io_if.delay_us(10);
    }
}

// ˽�ж� bit����׼��ʱ��
static bool OW_ReadBit(ow_handle_t *ow)
{
    bool bit;

    ow->io_if.set_pin(0);
    ow->io_if.delay_us(3); // ��ʼ����
    ow->io_if.set_pin(1);
    ow->io_if.delay_us(10);    // �ȴ��ӻ�Ӧ��
    bit = ow->io_if.get_pin(); // ��ȡ SDA ״̬
    ow->io_if.delay_us(50);    // �ȴ� slot ����

    return bit;
}

void OW_Init(ow_handle_t *ow)
{
    if (ow && ow->io_if.set_pin)
    {
        ow->io_if.set_pin(1); // Ĭ���ͷ�����
    }
}

bool OW_Reset(ow_handle_t *ow)
{
    ow->io_if.set_pin(0);
    ow->io_if.delay_us(480); // ��λ���� �� 480us
    ow->io_if.set_pin(1);
    ow->io_if.delay_us(70); // �ȴ��ӻ�Ӧ��

    bool presence = !ow->io_if.get_pin(); // �ӻ����ͱ�ʾ����
    ow->io_if.delay_us(410);              // �����ڽ���

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
