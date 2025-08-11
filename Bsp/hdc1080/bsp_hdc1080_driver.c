#include "bsp_hdc1080_driver.h"

/* I2C 地址 */
#define HDC1080_I2C_ADDR 0x40

/* 寄存器地址 */
#define HDC1080_REG_TEMP            0x00
#define HDC1080_REG_HUMI            0x01
#define HDC1080_REG_CONFIG          0x02
#define HDC1080_REG_SERIAL_ID_FIRST 0xFB
#define HDC1080_REG_MANUFACTURER_ID 0xFE
#define HDC1080_REG_DEVICE_ID       0xFF

/* 配置寄存器位定义 */
#define HDC1080_CONF_RESET               (1 << 15)
#define HDC1080_CONF_HEATER_EN           (1 << 13)
#define HDC1080_CONF_MODE_BOTH_TEMP_HUMI (1 << 12) // 1=温湿同时测
#define HDC1080_CONF_MODE_TEMP_OR_HUMI   (0 << 12) // 0=温湿单一测

#define HDC1080_CONF_BATTERY_STATUS      (1 << 11)

#define HDC1080_CONF_TEMP_RES_14BIT      (0 << 10)
#define HDC1080_CONF_TEMP_RES_11BIT      (1 << 10)

#define HDC1080_CONF_HUMI_RES_14BIT      (0 << 8)
#define HDC1080_CONF_HUMI_RES_11BIT      (1 << 8)
#define HDC1080_CONF_HUMI_RES_8BIT       (2 << 8)

hdc1080_status_t BSP_HDC1080_Init(bsp_hdc1080_t *phdc1080, hdc1080_measure_mode_t mode)
{
    uint8_t buf[3];
    uint8_t id_buf[2];
    uint16_t config;
    const iic_driver_interface_t *piic;

    if (NULL == phdc1080 ||
        NULL == phdc1080->psiic_driver_if) {
        return HDC1080_ERROR_HANDLE;
    }

    piic = phdc1080->psiic_driver_if;

    if (0 != piic->init()) {
        return HDC1080_ERROR_IIC_INIT;
    }

    phdc1080->temperature = 0.0f;
    phdc1080->humidity    = 0.0f;

    /* Manufacturer ID */
    if (0 != piic->read_reg(HDC1080_I2C_ADDR << 1, HDC1080_REG_MANUFACTURER_ID, id_buf, 2)) {
        return HDC1080_ERROR_IIC_INIT;
    }

    uint16_t man_id = ((uint16_t)id_buf[0] << 8) | id_buf[1];
    if (man_id != 0x5449) {
        return HDC1080_ERROR_INITIALIZATION;
    }

    /* Device ID */
    if (0 != piic->read_reg(HDC1080_I2C_ADDR << 1, HDC1080_REG_DEVICE_ID, id_buf, 2)) {
        return HDC1080_ERROR_IIC_INIT;
    }

    uint16_t dev_id = ((uint16_t)id_buf[0] << 8) | id_buf[1];
    if (dev_id != 0x1050) {
        return HDC1080_ERROR_INITIALIZATION;
    }

    /* 配置采集模式与精度 */
    config                 = 0;
    phdc1080->measure_mode = mode;
    switch (phdc1080->measure_mode) {
        case HDC1080_MEASURE_TEMP:
        case HDC1080_MEASURE_HUMI:
            config |= HDC1080_CONF_MODE_TEMP_OR_HUMI;
            break;
        case HDC1080_MEASURE_TEMP_HUMI:
            config |= HDC1080_CONF_MODE_BOTH_TEMP_HUMI;
            break;
        default:
            return HDC1080_ERROR_INITIALIZATION;
    }

    config |= HDC1080_CONF_TEMP_RES_11BIT;
    config |= HDC1080_CONF_HUMI_RES_8BIT;

    buf[0] = HDC1080_REG_CONFIG;
    buf[1] = (uint8_t)(config >> 8);
    buf[2] = (uint8_t)(config & 0xFF);
    if (0 != piic->write(HDC1080_I2C_ADDR << 1, buf, 3)) {
        return HDC1080_ERROR_IIC_INIT;
    }

    piic->delay_ms(15);

    return HDC1080_SUCCESS;
}

/* 采集更新 */
hdc1080_status_t BSP_HDC1080_Update(bsp_hdc1080_t *phdc1080)
{
    uint8_t reg;
    uint8_t data[4] = {0};
    uint16_t raw_val;
    const iic_driver_interface_t *piic;

    if (NULL == phdc1080 ||
        NULL == phdc1080->psiic_driver_if) {
        return HDC1080_ERROR_HANDLE;
    }

    piic = phdc1080->psiic_driver_if;

    switch (phdc1080->measure_mode) {
        case HDC1080_MEASURE_TEMP_HUMI:
            reg = HDC1080_REG_TEMP;
            piic->write(HDC1080_I2C_ADDR << 1, &reg, 1);
            piic->delay_ms(15);
            piic->read_dat(HDC1080_I2C_ADDR << 1, data, 4);
            raw_val               = ((uint16_t)data[0] << 8) | data[1];
            phdc1080->temperature = ((float)raw_val / 65536.0f) * 165.0f - 40.0f;
            raw_val               = ((uint16_t)data[2] << 8) | data[3];
            phdc1080->humidity    = ((float)raw_val / 65536.0f) * 100.0f;
            break;

        case HDC1080_MEASURE_TEMP:
            reg = HDC1080_REG_TEMP;
            piic->write(HDC1080_I2C_ADDR << 1, &reg, 1);
            piic->delay_ms(15);
            piic->read_dat(HDC1080_I2C_ADDR << 1, data, 2);
            raw_val               = ((uint16_t)data[0] << 8) | data[1];
            phdc1080->temperature = ((float)raw_val / 65536.0f) * 165.0f - 40.0f;
            break;

        case HDC1080_MEASURE_HUMI:
            reg = HDC1080_REG_HUMI;
            piic->write(HDC1080_I2C_ADDR << 1, &reg, 1);
            piic->delay_ms(15);
            piic->read_dat(HDC1080_I2C_ADDR << 1, data, 2);
            raw_val            = ((uint16_t)data[0] << 8) | data[1];
            phdc1080->humidity = ((float)raw_val / 65536.0f) * 100.0f;
            break;

        default:
            return HDC1080_ERROR_HANDLE;
    }

    return HDC1080_SUCCESS;
}

hdc1080_status_t BSP_HDC1080_Inst(const bsp_hdc1080_t *phdc1080,
                                  void (*pfdelay_ms)(uint32_t),
                                  int8_t (*pfiic_init)(void),
                                  int8_t (*pfiic_deinit)(void),
                                  int8_t (*pfiic_write)(uint8_t, uint8_t *, uint8_t),
                                  int8_t (*pfiic_read_dat)(uint8_t, uint8_t *, uint8_t),
                                  int8_t (*pfiic_read_reg)(uint8_t, uint8_t, uint8_t *, uint8_t),
                                  hdc1080_status_t (*pfhdc1080_init)(bsp_hdc1080_t *, hdc1080_measure_mode_t),
                                  hdc1080_status_t (*pfhdc1080_update)(bsp_hdc1080_t *))
{
    if (NULL == phdc1080) {
        return HDC1080_ERROR_HANDLE;
    }

    bsp_hdc1080_t *pstmp                   = (bsp_hdc1080_t *)phdc1080;
    static iic_driver_interface_t iic_d_if = {NULL, NULL, NULL, NULL, NULL, NULL};

    iic_d_if.init     = pfiic_init;
    iic_d_if.deinit   = pfiic_deinit;
    iic_d_if.write    = pfiic_write;
    iic_d_if.read_dat = pfiic_read_dat;
    iic_d_if.read_reg = pfiic_read_reg;
    iic_d_if.delay_ms = pfdelay_ms;

    if (NULL == iic_d_if.init ||
        NULL == iic_d_if.deinit ||
        NULL == iic_d_if.write ||
        NULL == iic_d_if.read_dat ||
        NULL == iic_d_if.read_reg ||
        NULL == iic_d_if.delay_ms) {
        return HDC1080_ERROR_IIC_INST;
    }

    pstmp->psiic_driver_if = &iic_d_if;
    pstmp->pfinit          = pfhdc1080_init;
    pstmp->pfupdate        = pfhdc1080_update;

    if (NULL == pstmp->psiic_driver_if ||
        NULL == pstmp->pfinit ||
        NULL == pstmp->pfupdate) {
        return HDC1080_ERROR_INSTANCE;
    }

    return HDC1080_SUCCESS;
}