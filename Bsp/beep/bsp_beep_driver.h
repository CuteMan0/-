#include <stdbool.h>
#include <stdint.h>
// #include <stddef.h>

typedef struct
{
    void (*set_pin)(bool level);
    void (*delay_ms)(uint16_t ms);
} bsp_beep_io_driver_t;

typedef struct bsp_beep_t {
    bsp_beep_io_driver_t *psio_if;
    uint8_t duty; // 0~100
    uint8_t freq; // 0~5Hz

    void (*pfinit)(struct bsp_beep_t *psbeep_handle);
    void (*pfenable)(struct bsp_beep_t *psbeep_handle);
    void (*pfdisable)(struct bsp_beep_t *psbeep_handle);
} bsp_beep_t;

#define LOGIC 1 // 1 : high active, 0: low active

/*
 *
 * @brief  Beep 实例化
 * @param  psbeep_handle: Beep句柄
 * @param  pfset_pin: Beep IO 设置函数
 * @param  pfdelay_ms: Beep 延时函数
 * @param  pfbeep_init: Beep 初始化函数
 * @param  pfbeep_enable: Beep 使能函数
 * @param  pfbeep_disable: Beep 禁用函数
 * @retval 0 实例化成功
 *         -1 IO接口异常
 *         -2 实例化失败
 */
int8_t BSP_Beep_Instance(const bsp_beep_t *psbeep_handle,
                         void (*pfset_pin)(bool level),
                         void (*pfdelay_ms)(uint16_t ms),
                         void (*pfbeep_init)(bsp_beep_t *psbeep_handle),
                         void (*pfbeep_enable)(bsp_beep_t *psbeep_handle),
                         void (*pfbeep_disable)(bsp_beep_t *psbeep_handle));

/*
 * @brief  Beep 初始化
 * @param  psbeep_handle: Beep句柄
 * @param  duty: 占空比
 * @retval None
 */
void BSP_Beep_Init(bsp_beep_t *psbeep_handle, uint8_t duty, uint8_t freq);
/*
 * @brief  Beep 使能
 * @param  psbeep_handle: Beep句柄
 * @retval None
 */
void BSP_Beep_Enable(bsp_beep_t *psbeep_handle);
/*
 * @brief  Beep 禁用
 * @param  psbeep_handle: Beep句柄
 * @retval None
 */
void BSP_Beep_Disable(bsp_beep_t *psbeep_handle);