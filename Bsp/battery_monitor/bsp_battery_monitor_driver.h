#ifndef __BSP_BATTERY_MONITOR_DRIVER_H
#define __BSP_BATTERY_MONITOR_DRIVER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @brief 电池监测句柄结构体
 */
typedef struct bsp_battery_monitor_t {
    /**
     * @brief ADC读取函数指针，用户需实现，返回ADC原始值
     */
    uint32_t (*adc_read)(void);

    /**
     * @brief 当前电池电量百分比，范围0~100
     */
    float state_of_charge;

    /**
     * @brief 初始化回调函数，由用户实现
     * @param self 当前结构体指针
     */
    void (*pfinit)(struct bsp_battery_monitor_t *self);

    /**
     * @brief 更新回调函数，由用户实现
     * @param self 当前结构体指针
     */
    void (*pfupdate)(struct bsp_battery_monitor_t *self);
} bsp_battery_monitor_t;

/**
 * @brief 实例化电池监测模块
 *
 * @param pbat_mon       电池监测句柄指针
 * @param pfadc_read     ADC读取函数指针，返回uint16_t原始ADC值
 * @param pfbat_mon_init 初始化回调函数指针
 * @param pfbat_mon_update 更新回调函数指针
 *
 * @retval  0 实例化成功
 * @retval -1 参数异常（空指针）
 * @retval -2 函数链接异常（空指针）
 */
int8_t BSP_Bat_Mon_Instance(const bsp_battery_monitor_t *pbat_mon,
                            uint32_t (*pfadc_read)(void),
                            void (*pfbat_mon_init)(bsp_battery_monitor_t *),
                            void (*pfbat_mon_update)(bsp_battery_monitor_t *));

/**
 * @brief 电池监测初始化函数
 *
 * @param pbat_mon   电池监测句柄指针
 * @param bat_level  初始电量值（建议填0，真实值由ADC测量获得）
 */
void BSP_Bat_Mon_Init(bsp_battery_monitor_t *pbat_mon);

/**
 * @brief 电池监测更新函数
 *
 * @param pbat_mon 电池监测句柄指针
 *
 * @note 此函数应被周期性调用，用于更新电池电量和执行回调
 */
void BSP_Bat_Mon_Update(bsp_battery_monitor_t *pbat_mon);

#endif /* __BSP_BATTERY_MONITOR_DRIVER_H */
