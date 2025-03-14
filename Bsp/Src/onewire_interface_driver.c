#include "onewire_interface_driver.h"
#include "gpio.h"

#define OW_IO_PORT   GPIOA
#define OW_IO_PIN    GPIO_PIN_0

#define OW_IO_HIGH   HAL_GPIO_WritePin(OW_IO_PORT, OW_IO_PIN, GPIO_PIN_SET)
#define OW_IO_LOW    HAL_GPIO_WritePin(OW_IO_PORT, OW_IO_PIN, GPIO_PIN_RESET)
#define OW_IO_READ() HAL_GPIO_ReadPin(OW_IO_PORT, OW_IO_PIN)

/*
 * 1-Wire ������ʱ����
 * @brief  ���� busy-wait ��ʽʵ����ʱ
 * @param  us: Ҫ��ʱ��΢����
 * @retval void
 */
#define CPU_FREQUENCY_MHZ 32 // STM32ʱ����Ƶ
void Delay_us(volatile uint32_t delay)
{

    int last, curr, val;
    int temp;

    while (delay != 0) {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0) {
            do {
                val = SysTick->VAL;
            } while ((val < last) && (val >= curr));
        } else {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do {
                val = SysTick->VAL;
            } while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }
}

void Onewire_Init(void)
{
    GPIO_InitTypeDef OneWireInitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    OW_IO_HIGH;

    OneWireInitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    OneWireInitStruct.Pin   = OW_IO_PIN;
    OneWireInitStruct.Pull  = GPIO_PULLUP;
    OneWireInitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OW_IO_PORT, &OneWireInitStruct);
}

bool Onewire_Reset(uint16_t rst_time, uint16_t rsp_time, uint16_t wait_time)
{
    // 0. ȷ����������һ���ȶ��ĸߵ�ƽ
    OW_IO_HIGH;
    Delay_us(1);

    // 1. ���͸�λ���� -_����
    OW_IO_LOW;
    Delay_us(rst_time);
    OW_IO_HIGH;
    Delay_us(rsp_time);

    // 2. ��ȡӦ���ź� ����_-
    uint8_t response = (0 == OW_IO_READ()) ? 1 : 0;
    Delay_us(wait_time);

    return response;
}

void write_bit(bool bit)
{
    if (bit) {
        // -_����
        // 1.����дʱ�� -_
        OW_IO_HIGH;
        Delay_us(1);
        OW_IO_LOW;
        // Delay_us(1);
        // 2.���ݡ�1���ĸ�ʽ _����
        Delay_us(1); // 1~15us
        OW_IO_HIGH;
        Delay_us(60);
    } else {
        // -__-
        // 1.����дʱ�� -_
        OW_IO_HIGH;
        Delay_us(1);
        OW_IO_LOW;
        // Delay_us(1);
        // 2. ���ݡ�0���ĸ�ʽ __-
        Delay_us(60); //>60us
        OW_IO_HIGH;
        Delay_us(1);
    }
}

void Onewire_WriteByte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++) {
        // LSB
        write_bit(data & 0x01);
        data >>= 1;
    }
}

uint8_t Onewire_ReadByte(void)
{
    uint8_t data = 0;
    for (uint8_t i = 0; i < 8; i++) {
        // LSB
        data >>= 1;
        // 1. ���Ͷ�ʱ�� >1us
        OW_IO_LOW;
        Delay_us(1);
        // 2.�ͷ����ߣ�����15usǰ����
        OW_IO_HIGH;
        Delay_us(9);
        if (OW_IO_READ()) {
            data |= 0x80;
        }
        // 3.һ�����������������ٱ���60us
        Delay_us(50);
    }
    return data;
}