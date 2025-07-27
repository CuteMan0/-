#include "bsp_dht20_driver.h"

uint8_t r_status  = 0;
uint8_t pt_cmd[]  = {0x33, 0x00};
uint8_t pr_dat[7] = {0x00};

dht20_status_t BSP_DHT20_Init(bsp_dht20_handle_t *psdht20_handle)
{
    psdht20_handle->temperature = 0.0f;
    psdht20_handle->humidity    = 0.0f;

    if (0 != psdht20_handle->psiic_driver_if->init()) {
        return DHT20_ERROR_IIC_INIT;
    }

    static bool init = 1;
    if (init) {
        init -= 1;
        psdht20_handle->psiic_driver_if->delay_ms(100);                                 // 上电等待
        psdht20_handle->psiic_driver_if->read((DEV_ADDRESS << 1) | 0x01, &r_status, 1); // 读状态字

        if (0x18 != (r_status & 0x18)) {
            // 初始化1B\1C\1E寄存器

            // DHT20_ERROR_INITIALIZATION
        }
        psdht20_handle->psiic_driver_if->delay_ms(10);
    }
    return DHT20_SUCCESS;
}

dht20_status_t BSP_DHT20_Update(bsp_dht20_handle_t *psdht20_handle)
{
    psdht20_handle->psiic_driver_if->write(DEV_ADDRESS << 1, 0xAC, pt_cmd, 2); // 触发测量
    psdht20_handle->psiic_driver_if->delay_ms(80);

    for (uint8_t i = 0; i < 5; i++) {
        psdht20_handle->psiic_driver_if->read((DEV_ADDRESS << 1) | 0x01, &r_status, 1); // 读状态字
        if (r_status & 0x80) {                                                          // 最高位为1，继续等待
            psdht20_handle->psiic_driver_if->delay_ms(80);
        } else {
            break; // 最高位为0，直接退出
        }
    }

    psdht20_handle->psiic_driver_if->read((DEV_ADDRESS << 1) | 0x01, pr_dat, 7); // 读数据

    psdht20_handle->humidity    = (((pr_dat[1] << 12) + (pr_dat[2] << 4) + (pr_dat[3] >> 4)) * 100.0) / 0xfffff;
    psdht20_handle->temperature = ((((pr_dat[3] << 28) >> 12) + (pr_dat[4] << 8) + (pr_dat[5])) * 200.0) / 0xfffff - 50;

    // CRC calculation    pr_dat[6];
    return DHT20_SUCCESS;
}

dht20_status_t BSP_DHT20_Inst(const bsp_dht20_handle_t *psdht20_handle,
                              void (*pfdelay_ms)(uint16_t),
                              int8_t (*pfiic_init)(void),
                              int8_t (*pfiic_write)(uint8_t, uint8_t, uint8_t *, uint8_t),
                              int8_t (*pfiic_read)(uint8_t, uint8_t *, uint8_t),
                              dht20_status_t (*pfdht20_init)(bsp_dht20_handle_t *),
                              dht20_status_t (*pfdht20_update)(bsp_dht20_handle_t *))
{
    bsp_dht20_handle_t *pstmp = psdht20_handle;

    static iic_driver_interface_t iic_d_if = {NULL, NULL, NULL, NULL};
    iic_d_if.init                          = pfiic_init;
    iic_d_if.write                         = pfiic_write;
    iic_d_if.read                          = pfiic_read;
    iic_d_if.delay_ms                      = pfdelay_ms;
    if (NULL == iic_d_if.init ||
        NULL == iic_d_if.read ||
        NULL == iic_d_if.write ||
        NULL == iic_d_if.delay_ms) {
        return DHT20_ERROR_IIC_INST;
    }

    pstmp->psiic_driver_if = &iic_d_if;
    pstmp->pfinit          = pfdht20_init;
    pstmp->pfupdate        = pfdht20_update;
    if (NULL == pstmp->psiic_driver_if ||
        NULL == pstmp->pfinit ||
        NULL == pstmp->pfupdate) {
        return DHT20_ERROR_INSTANCE;
    }

    return DHT20_SUCCESS;
}