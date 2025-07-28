#ifndef __BSP_HDCXX_DRIVER_H
#define __BSP_HDCXX_DRIVER_H

#include "stdbool.h"
#include "stdint.h"
#include <stddef.h>

typedef enum {
    HDC1080_SUCCESS              = 0,
    HDC1080_ERROR_HANDLE         = -1,
    HDC1080_ERROR_IIC_INST       = -2,
    HDC1080_ERROR_INSTANCE       = -3,
    HDC1080_ERROR_IIC_INIT       = -4,
    HDC1080_ERROR_INITIALIZATION = -5
} hdc1080_status_t;

typedef struct
{
    int8_t (*init)(void);
    int8_t (*deinit)(void);
    int8_t (*write)(uint8_t, uint8_t *, uint8_t);
    int8_t (*read)(uint8_t, uint8_t *, uint8_t);
    void (*delay_ms)(uint32_t);
} iic_driver_interface_t;

typedef struct bsp_hdc1080_t {
    iic_driver_interface_t *psiic_driver_if;

    float humidity;
    float temperature;

    hdc1080_status_t (*pfinit)(struct bsp_hdc1080_t *);
    hdc1080_status_t (*pfupdate)(struct bsp_hdc1080_t *);
} bsp_hdc1080_t;

/*                                    接口声明                                    */

/*
 *
 * @brief  HDC1080实例化
 * @param  pshdc1080_handle: HDC1080句柄
 * @param  pfiic_init: iic 初始化函数
 * @param  pfiic_deinit: iic 解初始化函数
 * @param  pfiic_write: iic 写函数
 * @param  pfiic_read: iic 读函数
 * @param  pfhdc1080_init: HDC1080 初始化函数
 * @param  pfhdc1080_update: HDC1080 更新温湿度数据函数
 * @retval 0 实例化成功
 *         -1 HDC1080句柄异常
 *         -2 iic接口异常
 *         -3 实例化失败
 */
hdc1080_status_t BSP_HDC1080_Inst(const bsp_hdc1080_t *pshdc1080_handle,
                                  void (*pfdelay_ms)(uint32_t),
                                  int8_t (*pfiic_init)(void),
                                  int8_t (*pfiic_deinit)(void),
                                  int8_t (*pfiic_write)(uint8_t, uint8_t *, uint8_t),
                                  int8_t (*pfiic_read)(uint8_t, uint8_t *, uint8_t),
                                  hdc1080_status_t (*pfhdc1080_init)(bsp_hdc1080_t *),
                                  hdc1080_status_t (*pfhdc1080_update)(bsp_hdc1080_t *));

/*
 * HDC1080初始化
 * @brief 初始化HDC1080的iic接口，并复位温湿度数据
 * @param  pshdc1080_handle: HDC1080句柄
 * @retval void
 */
hdc1080_status_t BSP_HDC1080_Init(bsp_hdc1080_t *pshdc1080_handle);

/*
 * HDC1080读取温度
 * @brief 向HDC1080申请温湿度度读取并转换数值
 * @param  pshdc1080_handle: HDC1080句柄
 * @retval 0 读取成功
 */
hdc1080_status_t BSP_HDC1080_Update(bsp_hdc1080_t *pshdc1080_handle);

#endif /* __BSP_HDCXX_DRIVER_H__ */