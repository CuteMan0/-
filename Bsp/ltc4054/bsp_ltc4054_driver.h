#ifndef __BSP_LTC4054_DRIVER_H
#define __BSP_LTC4054_DRIVER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef enum
{
    LTC4054_SUCCESS = 0,
    LTC4054_ERROR_HANDLE = -1,
    LTC4054_ERROR_IO_INST = -2,
    LTC4054_ERROR_INSTANCE = -3
} ltc4054_status_t;

typedef enum
{
    STATE_UNKNOWN,
    STATE_CHARGING,
    STATE_CHARGE_DONE,
    STATE_UVLO_OR_FLOAT
} ltc4054_charge_state_t;

typedef struct
{
    void (*set_out_high)(void);    // OUT = 推挽高电平
    void (*set_out_z)(void);       // OUT = 高阻输入
    bool (*read_in)(void);         // 读取 CHRG 引脚（即 IN）
    void (*delay_ms)(uint16_t ms); // 延时
} bsp_ltc4054_io_driver_t;

typedef struct bsp_ltc4054_t
{
    bsp_ltc4054_io_driver_t *psio_if;

    ltc4054_charge_state_t state;

    void (*pfinit)(struct bsp_ltc4054_t *self);
    void (*pfget_state)(struct bsp_ltc4054_t *self);
} bsp_ltc4054_t;

/**
 * @brief 初始化 LTC4054 实例
 * @param pltc4054 LTC4054 句柄
 * @param pfio_setout_high 设置 OUT 引脚为高电平
 * @param pfio_setout_z 设置 OUT 引脚为高阻输入
 * @param pfio_readin 读取 IN 引脚
 * @param pfio_delay_ms 延时函数
 * @param pfltc4054_init LTC4054 初始化函数
 * @param pfltc4054_get_state LTC4054 获取状态函数
 * @retval 实例化状态
 */
ltc4054_status_t BSP_LTC4054_Inst(const bsp_ltc4054_t *pltc4054,
                                  void (*pfio_setout_high)(void),
                                  void (*pfio_setout_z)(void),
                                  bool (*pfio_readin)(void),
                                  void (*pfdelay_ms)(uint16_t),
                                  void (*pfltc4054_init)(bsp_ltc4054_t *),
                                  void (*pfltc4054_get_state)(bsp_ltc4054_t *));

/**
 * @brief 初始化 LTC4054
 * @param pltc4054 LTC4054 句柄
 */
void BSP_LTC4054_Init(bsp_ltc4054_t *pltc4054);
/**
 * @brief 获取 LTC4054 状态
 * @param pltc4054 LTC4054 句柄
 */
void BSP_LTC4054_Get_State(bsp_ltc4054_t *pltc4054);

#endif /* __BSP_LTC4054_DRIVER_H */