#ifndef __BSP_NTC_DRIVER_H
#define __BSP_NTC_DRIVER_H

#include <stdint.h>

typedef enum {
    NTC_SUCCESS             = 0,
    NTC_ERROR_ADC_INTERFACE = -1,
    NTC_ERROR_PARAMETER     = -2
} ntc_status_t;

typedef enum {
    NTC_POSITION_UPPER = 0, // 分压电阻在上，NTC接地（Rs→NTC→GND）
    NTC_POSITION_LOWER = 1  // 分压电阻在下，NTC接VCC（VCC→NTC→Rs→GND）
} ntc_position_t;

typedef struct {
    uint16_t (*read_adc)(void); // 用户提供的 ADC 读取函数（0 ~ ADC_MAX）
} ntc_adc_interface_t;

typedef struct bsp_ntc_t {
    ntc_adc_interface_t *p_adc_if;

    float r_fixed;      // 分压电阻值（Ω）
    float beta;         // B 值（K）
    float r0;           // T0 时的阻值（Ω）
    float t0;           // T0（摄氏）
    ntc_position_t pos; // 分压电阻位置（上下）

    float temperature; // 读取的温度（℃）

    ntc_status_t (*pfinit)(struct bsp_ntc_t *self);
    ntc_status_t (*pfupdate)(struct bsp_ntc_t *self);
} bsp_ntc_t;

/*
 * @brief NTC 实例化函数
 */
ntc_status_t BSP_NTC_Inst(const bsp_ntc_t *pntc,
                          uint16_t (*pfread_adc)(void),
                          float r_fixed,
                          float beta,
                          float r0,
                          float t0,
                          ntc_position_t position,
                          ntc_status_t (*pfntc_init)(bsp_ntc_t *),
                          ntc_status_t (*pfntc_update)(bsp_ntc_t *));

/*
 * @brief 初始化 NTC 驱动
 */
ntc_status_t BSP_NTC_Init(bsp_ntc_t *pntc);

/*
 * @brief 更新 NTC 温度
 */
ntc_status_t BSP_NTC_Update(bsp_ntc_t *pntc);

#endif /* __BSP_NTC_DRIVER_H */
