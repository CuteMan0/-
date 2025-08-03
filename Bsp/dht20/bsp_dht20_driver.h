#ifndef __BSP_DHT20_DRIVER_H
#define __BSP_DHT20_DRIVER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define DEV_ADDRESS 0x38

typedef enum {
    DHT20_SUCCESS = 0,
    DHT20_ERROR_IIC_INST,
    DHT20_ERROR_INSTANCE,
    DHT20_ERROR_IIC_INIT,
    DHT20_ERROR_INITIALIZATION
} dht20_status_t;

typedef struct {
    int8_t (*init)(void);
    int8_t (*write)(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len);
    int8_t (*read)(uint8_t addr, uint8_t *data, uint8_t len);
    void (*delay_ms)(uint16_t ms);
} iic_driver_interface_t;

typedef struct bsp_dht20_t {
    iic_driver_interface_t *psiic_driver_if;

    dht20_status_t status;
    float humidity;
    float temperature;
    uint8_t crc_val;

    dht20_status_t (*pfinit)(struct bsp_dht20_t *self);
    dht20_status_t (*pfupdate)(struct bsp_dht20_t *self);
} bsp_dht20_handle_t;

/*                                    接口声明                                    */

/*
 *
 * @brief  DHT20实例化
 * @param  pdht20: DHT20句柄
 * @param  pfdelay_ms: ms级延时函数
 * @param  pfiic_init: iic 初始化函数
 * @param  pfiic_write: iic 写函数
 * @param  pfiic_read: iic 读函数
 * @param  pfdht20_init: DHT20 初始化函数
 * @param  pfdht20_update: DHT20 更新温湿度数据函数
 * @retval 0 实例化成功
 *         -1 delay接口异常
 *         -2 iic接口异常
 *         -3 实例化失败
 */
dht20_status_t BSP_DHT20_Inst(const bsp_dht20_handle_t *pdht20,
                              void (*pfdelay_ms)(uint16_t),
                              int8_t (*pfiic_init)(void),
                              int8_t (*pfiic_write)(uint8_t, uint8_t, uint8_t *, uint8_t),
                              int8_t (*pfiic_read)(uint8_t, uint8_t *, uint8_t),
                              dht20_status_t (*pfdht20_init)(bsp_dht20_handle_t *),
                              dht20_status_t (*pfdht20_update)(bsp_dht20_handle_t *));

/*
 * DHT20初始化
 * @brief 初始化DHT20的iic接口，并复位温湿度数据
 * @param  pdht20: DHT20句柄
 * @retval void
 */
dht20_status_t BSP_DHT20_Init(bsp_dht20_handle_t *pdht20);

/*
 * DHT20读取温度
 * @brief 向DHT20申请温湿度度读取并转换数值
 * @param  pdht20: DHT20句柄
 * @retval 0 读取成功
 */
dht20_status_t BSP_DHT20_Update(bsp_dht20_handle_t *pdht20);

#endif /* __BSP_DHT20_DRIVER_H */