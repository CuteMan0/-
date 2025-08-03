#ifndef __BSP_NTC_DRIVER_H
#define __BSP_NTC_DRIVER_H

#include <stdint.h>

typedef enum {
    NTC_SUCCESS             = 0,
    NTC_ERROR_ADC_INTERFACE = -1,
    NTC_ERROR_PARAMETER     = -2
} ntc_status_t;

typedef enum {
    NTC_POSITION_UPPER = 0, // ��ѹ�������ϣ�NTC�ӵأ�Rs��NTC��GND��
    NTC_POSITION_LOWER = 1  // ��ѹ�������£�NTC��VCC��VCC��NTC��Rs��GND��
} ntc_position_t;

typedef struct {
    uint16_t (*read_adc)(void); // �û��ṩ�� ADC ��ȡ������0 ~ ADC_MAX��
} ntc_adc_interface_t;

typedef struct bsp_ntc_t {
    ntc_adc_interface_t *p_adc_if;

    float r_fixed;      // ��ѹ����ֵ������
    float beta;         // B ֵ��K��
    float r0;           // T0 ʱ����ֵ������
    float t0;           // T0�����ϣ�
    ntc_position_t pos; // ��ѹ����λ�ã����£�

    float temperature; // ��ȡ���¶ȣ��棩

    ntc_status_t (*pfinit)(struct bsp_ntc_t *self);
    ntc_status_t (*pfupdate)(struct bsp_ntc_t *self);
} bsp_ntc_t;

/*
 * @brief NTC ʵ��������
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
 * @brief ��ʼ�� NTC ����
 */
ntc_status_t BSP_NTC_Init(bsp_ntc_t *pntc);

/*
 * @brief ���� NTC �¶�
 */
ntc_status_t BSP_NTC_Update(bsp_ntc_t *pntc);

#endif /* __BSP_NTC_DRIVER_H */
