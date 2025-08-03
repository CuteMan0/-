#include "bsp_ntc_driver.h"
#include <math.h>
#include <stddef.h>

#define ADC_MAX_VALUE 4095.0f

static ntc_adc_interface_t ntc_adc_if;

ntc_status_t BSP_NTC_Inst(const bsp_ntc_t *pntc,
                          uint16_t (*pfread_adc)(void),
                          float r_fixed,
                          float beta,
                          float r0,
                          float t0,
                          ntc_position_t position,
                          ntc_status_t (*pfntc_init)(bsp_ntc_t *),
                          ntc_status_t (*pfntc_update)(bsp_ntc_t *))
{
    if (pntc == NULL || pfread_adc == NULL)
        return NTC_ERROR_ADC_INTERFACE;

    ntc_adc_if.read_adc = pfread_adc;

    bsp_ntc_t *pn = (bsp_ntc_t *)pntc;
    pn->p_adc_if  = &ntc_adc_if;
    pn->r_fixed   = r_fixed;
    pn->beta      = beta;
    pn->r0        = r0;
    pn->t0        = t0;
    pn->pos       = position;
    pn->pfinit    = pfntc_init;
    pn->pfupdate  = pfntc_update;

    return NTC_SUCCESS;
}

ntc_status_t BSP_NTC_Init(bsp_ntc_t *pntc)
{
    if (pntc == NULL || pntc->p_adc_if == NULL)
        return NTC_ERROR_PARAMETER;

    pntc->temperature = -273.15f;
    return NTC_SUCCESS;
}

ntc_status_t BSP_NTC_Update(bsp_ntc_t *pntc)
{
    if (pntc == NULL || pntc->p_adc_if == NULL || pntc->p_adc_if->read_adc == NULL)
        return NTC_ERROR_PARAMETER;

    uint16_t adc_val = pntc->p_adc_if->read_adc();
    if (adc_val == 0 || adc_val >= ADC_MAX_VALUE)
        return NTC_ERROR_ADC_INTERFACE;

    float v_ratio = (float)adc_val / ADC_MAX_VALUE;
    if (v_ratio <= 0.0f || v_ratio >= 1.0f)
        return NTC_ERROR_ADC_INTERFACE;

    float r_ntc;

    if (pntc->pos == NTC_POSITION_LOWER) {
        // Rs 在下（低边），NTC接VCC
        r_ntc = pntc->r_fixed * ((1.0f / v_ratio) - 1.0f);
    } else {
        // Rs 在上（高边），NTC接GND
        r_ntc = pntc->r_fixed * (v_ratio / (1.0f - v_ratio));
    }

    float t0_K        = pntc->t0 + 273.15f;
    float temp_k      = 1.0f / ((1.0f / t0_K) + (1.0f / pntc->beta) * logf(r_ntc / pntc->r0));
    pntc->temperature = temp_k - 273.15f;

    return NTC_SUCCESS;
}
