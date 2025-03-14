#include "onewire_interface_driver.h"
#include "gpio.h"

#define OW_IO_PORT   GPIOA
#define OW_IO_PIN    GPIO_PIN_0

#define OW_IO_HIGH   HAL_GPIO_WritePin(OW_IO_PORT, OW_IO_PIN, GPIO_PIN_SET)
#define OW_IO_LOW    HAL_GPIO_WritePin(OW_IO_PORT, OW_IO_PIN, GPIO_PIN_RESET)
#define OW_IO_READ() HAL_GPIO_ReadPin(OW_IO_PORT, OW_IO_PIN)

/*
 * 1-Wire 总线延时函数
 * @brief  采用 busy-wait 方式实现延时
 * @param  us: 要延时的微秒数
 * @retval void
 */
#define CPU_FREQUENCY_MHZ 32 // STM32时钟主频
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
    // 0. 确保总线上有一段稳定的高电平
    OW_IO_HIGH;
    Delay_us(1);

    // 1. 发送复位脉冲 -_——
    OW_IO_LOW;
    Delay_us(rst_time);
    OW_IO_HIGH;
    Delay_us(rsp_time);

    // 2. 读取应答信号 ——_-
    uint8_t response = (0 == OW_IO_READ()) ? 1 : 0;
    Delay_us(wait_time);

    return response;
}

void write_bit(bool bit)
{
    if (bit) {
        // -_——
        // 1.发送写时序 -_
        OW_IO_HIGH;
        Delay_us(1);
        OW_IO_LOW;
        // Delay_us(1);
        // 2.数据“1”的格式 _——
        Delay_us(1); // 1~15us
        OW_IO_HIGH;
        Delay_us(60);
    } else {
        // -__-
        // 1.发送写时序 -_
        OW_IO_HIGH;
        Delay_us(1);
        OW_IO_LOW;
        // Delay_us(1);
        // 2. 数据“0”的格式 __-
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
        // 1. 发送读时序 >1us
        OW_IO_LOW;
        Delay_us(1);
        // 2.释放总线，并在15us前采样
        OW_IO_HIGH;
        Delay_us(9);
        if (OW_IO_READ()) {
            data |= 0x80;
        }
        // 3.一整个读操作周期至少保持60us
        Delay_us(50);
    }
    return data;
}