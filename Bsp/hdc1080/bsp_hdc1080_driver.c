#include "bsp_hdc1080_driver.h"

#define HDC1080_I2C_ADDR            0x40

#define HDC1080_REG_TEMP            0x00
#define HDC1080_REG_HUMI            0x01
#define HDC1080_REG_CONFIG          0x02
#define HDC1080_REG_ID              0xFB
#define HDC1080_REG_MANUFACTURER_ID 0xFE
#define HDC1080_REG_DEVICE_ID       0xFF

#define HDC1080_CONFIG_RESET        0x8000
#define HDC1080_CONFIG_HEAT         0x2000
#define HDC1080_CONFIG_MODE         0x1000
#define HDC1080_CONFIG_BAT          0x0800
#define HDC1080_CONFIG_TRES         0x0400
#define HDC1080_CONFIG_HRES         0x0200

hdc1080_status_t BSP_HDC1080_Inst(const bsp_hdc1080_t *pshdc1080_handle,
                                  void (*pfdelay_ms)(uint32_t),
                                  int8_t (*pfiic_init)(void),
                                  int8_t (*pfiic_deinit)(void),
                                  int8_t (*pfiic_write)(uint8_t, uint8_t *, uint8_t),
                                  int8_t (*pfiic_read)(uint8_t, uint8_t *, uint8_t),
                                  hdc1080_status_t (*pfhdc1080_init)(bsp_hdc1080_t *),
                                  hdc1080_status_t (*pfhdc1080_update)(bsp_hdc1080_t *))
{
    if (NULL == pshdc1080_handle) {
        return HDC1080_ERROR_HANDLE;
    }

    bsp_hdc1080_t *pstmp                   = pshdc1080_handle;
    static iic_driver_interface_t iic_d_if = {NULL, NULL, NULL, NULL, NULL};

    iic_d_if.init     = pfiic_init;
    iic_d_if.deinit   = pfiic_deinit;
    iic_d_if.write    = pfiic_write;
    iic_d_if.read     = pfiic_read;
    iic_d_if.delay_ms = pfdelay_ms;
    if (NULL == iic_d_if.init ||
        NULL == iic_d_if.deinit ||
        NULL == iic_d_if.write ||
        NULL == iic_d_if.read ||
        NULL == iic_d_if.delay_ms) {
        return HDC1080_ERROR_IIC_INST;
    }

    pstmp->psiic_driver_if = &iic_d_if;
    pstmp->pfinit          = pfhdc1080_init;
    pstmp->pfupdate        = pfhdc1080_update;
    if (
        NULL == pstmp->psiic_driver_if ||
        NULL == pstmp->pfinit ||
        NULL == pstmp->pfupdate) {
        return HDC1080_ERROR_INSTANCE;
    }

    return HDC1080_SUCCESS;
}

hdc1080_status_t BSP_HDC1080_Init(bsp_hdc1080_t *pshdc1080_handle)
{
    pshdc1080_handle->temperature = 0.0f;
    pshdc1080_handle->humidity    = 0.0f;

    if (0 != pshdc1080_handle->psiic_driver_if->init()) {
        return HDC1080_ERROR_IIC_INIT;
    }

    return HDC1080_SUCCESS;
}

hdc1080_status_t BSP_HDC1080_Update(bsp_hdc1080_t *pshdc1080_handle)
{
    uint8_t cmd;
    uint8_t data[4];

    if (NULL == pshdc1080_handle ||
        NULL == pshdc1080_handle->psiic_driver_if) {
        // return HDC1080_ERROR_NULL_PTR;
    }

    const iic_driver_interface_t *piic = pshdc1080_handle->psiic_driver_if;

    // 触发测量温湿度
    cmd = HDC1080_REG_TEMP;
    if (0 != piic->write(HDC1080_I2C_ADDR << 1, &cmd, 1)) {
        // return HDC1080_ERROR_IIC_WRITE;
    }

    // HDC1080 最大转换时间为 6.5ms，建议延迟 15ms 以确保完成
    piic->delay_ms(15);

    // 读取 4 字节：前 2 字节为温度，后 2 字节为湿度
    if (0 != piic->read(HDC1080_I2C_ADDR << 1, data, 4)) {
        // return HDC1080_ERROR_IIC_READ;
    }

    // 解码温度
    uint16_t raw_temp             = ((uint16_t)data[0] << 8) | data[1];
    pshdc1080_handle->temperature = ((float)raw_temp / 65536.0f) * 165.0f - 40.0f;

    // 解码湿度
    uint16_t raw_humi          = ((uint16_t)data[2] << 8) | data[3];
    pshdc1080_handle->humidity = ((float)raw_humi / 65536.0f) * 100.0f;

    return HDC1080_SUCCESS;
}
