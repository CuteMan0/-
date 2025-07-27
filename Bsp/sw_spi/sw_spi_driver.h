#ifndef __SW_SPI_DRIVER_H
#define __SW_SPI_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

// 模拟 SPI 引脚操作接口
typedef struct
{
    void (*set_sclk)(bool level);  // 设置时钟引脚
    void (*set_mosi)(bool level);  // 设置MOSI引脚
    bool (*get_miso)(void);        // 读取MISO引脚电平
    void (*set_cs)(bool level);    // 设置片选引脚（可选）
    void (*delay_us)(uint16_t us); // 微秒级延时函数
} sw_spi_io_interface_t;

// 模拟 SPI 配置结构体
typedef struct
{
    sw_spi_io_interface_t io_if;
    uint8_t cpol;      // 时钟空闲电平（0:低，1:高）
    uint8_t cpha;      // 采样边沿（0:第一个，1:第二个）
    uint8_t lsb_first; // 是否LSB先传（0:MSB first，1:LSB first）
} sw_spi_config_t;

// 模拟 SPI 句柄
typedef struct
{
    sw_spi_config_t config;
} sw_spi_handle_t;

#ifdef __cplusplus
extern "C"
{
#endif

    void sw_spi_init(sw_spi_handle_t *hspi, const sw_spi_config_t *config);
    uint8_t sw_spi_transfer_byte(sw_spi_handle_t *hspi, uint8_t data);
    void sw_spi_transfer_bytes(sw_spi_handle_t *hspi, const uint8_t *tx_buf, uint8_t *rx_buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif // __SW_SPI_DRIVER_H
