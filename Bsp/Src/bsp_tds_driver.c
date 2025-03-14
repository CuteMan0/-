#include "bsp_tds_driver.h"

#include <math.h>

#define VREF                      3.3   // ADC参考电压
#define K                         0.976 // TDS修正系数
#define K1                        66.71
#define K2                        127.93
#define K3                        428.7
#define __CONVERT_TO_TDS(VOL_ADJ) (K1 * pow(VOL_ADJ, 3) - K2 * pow(VOL_ADJ, 2) + K3 * VOL_ADJ) * K

int8_t BSP_TDS_Read(bsp_tds_t *pstds_handle, float temperature)
{
    // 启动ADC
    if (0 != pstds_handle->psadc_if->pfstart()) {
        return -1;
    }

    // 获取ADC转换值
    uint32_t adc_value = pstds_handle->psadc_if->pfread();

    float vol_a0 = (float)adc_value * VREF / 4096.0;

    // 温度补偿
    float vol_adj = vol_a0 * (1 + 0.02 * (temperature - 25));
    // 计算TDS值
    pstds_handle->tds_value = __CONVERT_TO_TDS(vol_adj);

    return 0; // 读取TDS值成功
}

int8_t BSP_TDS_Init(bsp_tds_t *pstds_handle)
{
    pstds_handle->tds_value = 0;
    if (0 != pstds_handle->psadc_if->pfinit()) {
        return -1;
    }

    return 0;
}

int8_t BSP_TDS_Inst(const bsp_tds_t *pstds_handle,
                    int8_t (*pfadc_init)(void),
                    int8_t (*pfadc_start)(void),
                    uint32_t (*pfadc_read)(void),
                    int8_t (*pftds_init)(bsp_tds_t *),
                    int8_t (*pftds_read)(bsp_tds_t *, float))
{
    bsp_tds_t *pstmp              = pstds_handle;
    static adc_interface_t adc_if = {NULL, NULL, NULL};

    adc_if.pfinit  = pfadc_init;
    adc_if.pfstart = pfadc_start;
    adc_if.pfread  = pfadc_read;
    if (NULL == adc_if.pfinit ||
        NULL == adc_if.pfstart ||
        NULL == adc_if.pfread) {
        return -1;
    }

    pstmp->psadc_if = &adc_if;
    pstmp->pfinit   = pftds_init;
    pstmp->pfread   = pftds_read;
    if (NULL == pstmp->psadc_if ||
        NULL == pstmp->pfinit ||
        NULL == pstmp->pfread) {
        return -2;
    }

    return 0;
}