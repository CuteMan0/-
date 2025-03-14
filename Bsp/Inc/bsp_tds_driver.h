#include <stdint.h>
#include <stddef.h>

typedef struct
{
    int8_t (*pfinit)(void);
    int8_t (*pfstart)(void);
    uint32_t (*pfread)(void);
} adc_interface_t;

typedef struct bsp_tds_t {
    adc_interface_t *psadc_if;

    int8_t (*pfinit)(struct bsp_tds_t *);
    int8_t (*pfread)(struct bsp_tds_t *, float);

    float tds_value;
} bsp_tds_t;

/*                                    接口声明                                    */

/*
 *
 * @brief  TDS 实例化
 * @param  pstds_handle: TDS句柄
 * @param  pfadc_init: ADC初始化函数
 * @param  pfadc_start: ADC启动函数
 * @param  pfadc_read: ADC读取函数
 * @param  pftds_init: TDS初始化函数
 * @param  pftds_read: TDS读取函数
 * @retval 0 实例化成功
 *         -1 ADC接口异常
 *         -2 实例化失败
 */
int8_t BSP_TDS_Inst(const bsp_tds_t *pstds_handle,
                    int8_t (*pfadc_init)(void),
                    int8_t (*pfadc_start)(void),
                    uint32_t (*pfadc_read)(void),
                    int8_t (*pftds_init)(bsp_tds_t *),
                    int8_t (*pftds_read)(bsp_tds_t *, float));

/*
 * TDS读取
 * @brief 读取检测到的TDS数值
 * @param  pstds_handle: TDS句柄
 * @param  temperature: 被测水体的温度值（默认值25.0℃）
 * @retval 0 读取成功
 *         -1 读取失败
 */
int8_t BSP_TDS_Read(bsp_tds_t *pstds_handle, float temperature);

/*
 * TDS初始化
 * @brief 初始化ADC接口
 * @param  pstds_handle: TDS句柄
 * @retval 0 初始化成功
 *         -1 初始化失败
 */
int8_t BSP_TDS_Init(bsp_tds_t *pstds_handle);