#ifndef __BSP_HDC1080_DRIVER_H
#define __BSP_HDC1080_DRIVER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* 状态码 */
typedef enum {
    HDC1080_SUCCESS              = 0,
    HDC1080_ERROR_HANDLE         = -1,
    HDC1080_ERROR_IIC_INST       = -2,
    HDC1080_ERROR_INSTANCE       = -3,
    HDC1080_ERROR_IIC_INIT       = -4,
    HDC1080_ERROR_INITIALIZATION = -5
} hdc1080_status_t;

/* I2C 驱动接口 */
typedef struct
{
    int8_t (*init)(void);
    int8_t (*deinit)(void);
    int8_t (*write)(uint8_t addr, uint8_t *dat, uint8_t len);
    int8_t (*read_dat)(uint8_t addr, uint8_t *dat, uint8_t len);
    int8_t (*read_reg)(uint8_t addr, uint8_t reg, uint8_t *dat, uint8_t len); // restart
    void (*delay_ms)(uint32_t ms);
} iic_driver_interface_t;

/* 测量模式 */
typedef enum {
    HDC1080_MEASURE_TEMP = 0, // 只测温度
    HDC1080_MEASURE_HUMI,     // 只测湿度
    HDC1080_MEASURE_TEMP_HUMI // 同时测温湿度
} hdc1080_measure_mode_t;

/* HDC1080 句柄 */
typedef struct bsp_hdc1080_t {
    iic_driver_interface_t *psiic_driver_if;

    float humidity;
    float temperature;

    hdc1080_measure_mode_t measure_mode;

    hdc1080_status_t (*pfinit)(struct bsp_hdc1080_t *self, hdc1080_measure_mode_t mode);
    hdc1080_status_t (*pfupdate)(struct bsp_hdc1080_t *self);
} bsp_hdc1080_t;

/* API 声明 */
hdc1080_status_t BSP_HDC1080_Inst(const bsp_hdc1080_t *phdc1080,
                                  void (*pfdelay_ms)(uint32_t),
                                  int8_t (*pfiic_init)(void),
                                  int8_t (*pfiic_deinit)(void),
                                  int8_t (*pfiic_write)(uint8_t, uint8_t *, uint8_t),
                                  int8_t (*pfiic_read_dat)(uint8_t, uint8_t *, uint8_t),
                                  int8_t (*pfiic_read_reg)(uint8_t, uint8_t, uint8_t *, uint8_t),
                                  hdc1080_status_t (*pfhdc1080_init)(bsp_hdc1080_t *, hdc1080_measure_mode_t),
                                  hdc1080_status_t (*pfhdc1080_update)(bsp_hdc1080_t *));

hdc1080_status_t BSP_HDC1080_Init(bsp_hdc1080_t *phdc1080, hdc1080_measure_mode_t mode);
hdc1080_status_t BSP_HDC1080_Update(bsp_hdc1080_t *phdc1080);

#endif /* __BSP_HDC1080_DRIVER_H */
