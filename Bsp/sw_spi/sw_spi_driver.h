#ifndef __SW_SPI_DRIVER_H
#define __SW_SPI_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

// ģ�� SPI ���Ų����ӿ�
typedef struct
{
    void (*set_sclk)(bool level);  // ����ʱ������
    void (*set_mosi)(bool level);  // ����MOSI����
    bool (*get_miso)(void);        // ��ȡMISO���ŵ�ƽ
    void (*set_cs)(bool level);    // ����Ƭѡ���ţ���ѡ��
    void (*delay_us)(uint16_t us); // ΢�뼶��ʱ����
} sw_spi_io_interface_t;

// ģ�� SPI ���ýṹ��
typedef struct
{
    sw_spi_io_interface_t io_if;
    uint8_t cpol;      // ʱ�ӿ��е�ƽ��0:�ͣ�1:�ߣ�
    uint8_t cpha;      // �������أ�0:��һ����1:�ڶ�����
    uint8_t lsb_first; // �Ƿ�LSB�ȴ���0:MSB first��1:LSB first��
} sw_spi_config_t;

// ģ�� SPI ���
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
