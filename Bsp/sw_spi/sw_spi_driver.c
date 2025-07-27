#include "sw_spi_driver.h"

static void sw_spi_set_clock(sw_spi_handle_t *hspi, bool level)
{
    hspi->config.io_if.set_sclk(level);
}

static void sw_spi_set_mosi(sw_spi_handle_t *hspi, bool level)
{
    hspi->config.io_if.set_mosi(level);
}

static bool sw_spi_get_miso(sw_spi_handle_t *hspi)
{
    return hspi->config.io_if.get_miso();
}

static void sw_spi_set_cs(sw_spi_handle_t *hspi, bool level)
{
    if (hspi->config.io_if.set_cs)
    {
        hspi->config.io_if.set_cs(level);
    }
}

static void sw_spi_delay(sw_spi_handle_t *hspi)
{
    hspi->config.io_if.delay_us(1); // 默认1us延时，可改
}

void sw_spi_init(sw_spi_handle_t *hspi, const sw_spi_config_t *config)
{
    sw_spi_set_clock(hspi, hspi->config.cpol);
    sw_spi_set_cs(hspi, true); // 不选中设备
}

uint8_t sw_spi_transfer_byte(sw_spi_handle_t *hspi, uint8_t data_out)
{
    uint8_t data_in = 0;
    uint8_t bit;

    for (bit = 0; bit < 8; bit++)
    {
        uint8_t bit_index = hspi->config.lsb_first ? bit : (7 - bit);
        bool bit_val = (data_out >> bit_index) & 0x01;

        if (hspi->config.cpha == 0)
        {
            sw_spi_set_mosi(hspi, bit_val);
            sw_spi_delay(hspi);
            sw_spi_set_clock(hspi, !hspi->config.cpol);
            sw_spi_delay(hspi);
            if (sw_spi_get_miso(hspi))
            {
                data_in |= (1 << bit_index);
            }
            sw_spi_set_clock(hspi, hspi->config.cpol);
        }
        else
        {
            sw_spi_set_clock(hspi, !hspi->config.cpol);
            sw_spi_set_mosi(hspi, bit_val);
            sw_spi_delay(hspi);
            sw_spi_set_clock(hspi, hspi->config.cpol);
            sw_spi_delay(hspi);
            if (sw_spi_get_miso(hspi))
            {
                data_in |= (1 << bit_index);
            }
        }
    }

    return data_in;
}

void sw_spi_transfer_bytes(sw_spi_handle_t *hspi, const uint8_t *tx_buf, uint8_t *rx_buf, uint16_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        uint8_t tx = tx_buf ? tx_buf[i] : 0xFF;
        uint8_t rx = sw_spi_transfer_byte(hspi, tx);
        if (rx_buf)
        {
            rx_buf[i] = rx;
        }
    }
}
