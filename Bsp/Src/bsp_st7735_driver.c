#include "bsp_st7735_driver.h"

#include "spi.h"
#include "gpio.h"
#include "dma.h"

#define DELAY_MS(x)        HAL_Delay(x)

#define LCD_CS_EN          LL_GPIO_ResetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin);
#define LCD_CS_DIS         LL_GPIO_SetOutputPin(LCD_CS_GPIO_Port, LCD_CS_Pin);
#define LCD_DC_CMD         LL_GPIO_ResetOutputPin(LCD_DC_GPIO_Port, LCD_DC_Pin);
#define LCD_DC_DAT         LL_GPIO_SetOutputPin(LCD_DC_GPIO_Port, LCD_DC_Pin);
#define LCD_RST_START      LL_GPIO_ResetOutputPin(LCD_RST_GPIO_Port, LCD_RST_Pin);
#define LCD_RST_END        LL_GPIO_SetOutputPin(LCD_RST_GPIO_Port, LCD_RST_Pin);

#define SPI_16BIT_MODE_EN  switch_16bit_mode(1)
#define SPI_16BIT_MODE_DIS switch_16bit_mode(0)

#define LCD_PIXEL_X_OFFSET 2
#define LCD_PIXEL_Y_OFFSET 1

typedef struct
{
    uint8_t command;  // 指令字节
    uint8_t numArgs;  // 参数个数
    uint8_t args[16]; // 参数数组
} ST7735_Command;

ST7735_Command init_list[] = {
    // 0x11: Sleep Out
    {0x11, 0, {}},

    // ST7735S Frame Rate
    {0xB1, 3, {0x05, 0x3C, 0x3C}},
    {0xB2, 3, {0x05, 0x3C, 0x3C}},
    {0xB3, 6, {0x05, 0x3C, 0x3C, 0x05, 0x3C, 0x3C}},

    // Dot inversion
    {0xB4, 1, {0x03}},

    // ST7735S Power Sequence
    {0xC0, 3, {0x28, 0x08, 0x04}},
    {0xC1, 1, {0xC0}},
    {0xC2, 2, {0x0D, 0x00}},
    {0xC3, 2, {0x8D, 0x2A}},
    {0xC4, 2, {0x8D, 0xEE}},

    // VCOM
    {0xC5, 1, {0x1A}},

    // MX, MY, RGB mode
    {0x36, 1, {0xC0}},

    // ST7735S Gamma Sequence
    {0xE0, 16, {0x04, 0x22, 0x07, 0x0A, 0x2E, 0x30, 0x25, 0x2A, 0x28, 0x26, 0x2E, 0x3A, 0x00, 0x01, 0x03, 0x13}},
    {0xE1, 16, {0x04, 0x16, 0x06, 0x0D, 0x2D, 0x26, 0x23, 0x27, 0x27, 0x25, 0x2D, 0x3B, 0x00, 0x01, 0x04, 0x13}},

    // 65k color mode
    {0x3A, 1, {0x05}},

    // Display On
    {0x29, 0, {}},
};

static void send_cmd(uint8_t cmd);
static void send_dat(uint16_t data);
static void switch_16bit_mode(bool mode);
static void st7735_reset(void);

void ST7735_Init(void)
{
    st7735_reset();

    for (int i = 0; i < sizeof(init_list) / sizeof(init_list[0]); i++) {
        send_cmd(init_list[i].command);
        for (int j = 0; j < init_list[i].numArgs; j++) {
            send_dat(init_list[i].args[j]);
        }
    }
}

void ST7735_SetRotation(uint8_t rotation)
{
    uint8_t madctl = 0;

    switch (rotation) {
        case 0:
            madctl = 0x40 | 0x80 | 0x00;
            break;
        case 1:
            madctl = 0x80 | 0x20 | 0x00;
            break;
        case 2:
            madctl = 0x00;
            break;
        case 3:
            madctl = 0x40 | 0x20 | 0x00;
            break;
    }

    send_cmd(0x36);
    send_dat(madctl);
}

void st7735_write_memory(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t *color_buffer)
{
    uint32_t size = (x_end - x_start + 1) * (y_end - y_start + 1);

    send_cmd(0x2a);
    send_dat(x_start >> 8);
    send_dat(x_start + LCD_PIXEL_X_OFFSET);
    send_dat(x_end >> 8);
    send_dat(x_end + LCD_PIXEL_X_OFFSET);

    send_cmd(0x2b);
    send_dat(y_start >> 8);
    send_dat(y_start + LCD_PIXEL_Y_OFFSET);
    send_dat(y_end >> 8);
    send_dat(y_end + LCD_PIXEL_Y_OFFSET);
    send_cmd(0x2c);

    switch_16bit_mode(1);

    LCD_CS_EN;
    LCD_DC_DAT;
    HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)color_buffer, size); // 启动 DMA 传输
    while (hspi1.State == HAL_SPI_STATE_BUSY_TX) {
    };
    LCD_CS_DIS; // 禁用片选

    switch_16bit_mode(0);
}

static void send_cmd(uint8_t cmd)
{
    LCD_CS_EN;
    LCD_DC_CMD;
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&cmd, 1, HAL_MAX_DELAY);
    LCD_CS_DIS;
}

static void send_dat(uint16_t data)
{
    LCD_CS_EN;
    LCD_DC_DAT;
    HAL_SPI_Transmit(&hspi1, (uint8_t *)&data, 1, HAL_MAX_DELAY);
    LCD_CS_DIS;
}

static void switch_16bit_mode(bool mode)
{
    if (mode) {
        hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
    } else {
        hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    }
    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        for (;;);
    }
}

static void st7735_reset(void)
{
    LCD_RST_START;
    DELAY_MS(100);
    LCD_RST_END;
    DELAY_MS(150);
}
