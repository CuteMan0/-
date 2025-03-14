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

/*                                    �ӿ�����                                    */

/*
 *
 * @brief  TDS ʵ����
 * @param  pstds_handle: TDS���
 * @param  pfadc_init: ADC��ʼ������
 * @param  pfadc_start: ADC��������
 * @param  pfadc_read: ADC��ȡ����
 * @param  pftds_init: TDS��ʼ������
 * @param  pftds_read: TDS��ȡ����
 * @retval 0 ʵ�����ɹ�
 *         -1 ADC�ӿ��쳣
 *         -2 ʵ����ʧ��
 */
int8_t BSP_TDS_Inst(const bsp_tds_t *pstds_handle,
                    int8_t (*pfadc_init)(void),
                    int8_t (*pfadc_start)(void),
                    uint32_t (*pfadc_read)(void),
                    int8_t (*pftds_init)(bsp_tds_t *),
                    int8_t (*pftds_read)(bsp_tds_t *, float));

/*
 * TDS��ȡ
 * @brief ��ȡ��⵽��TDS��ֵ
 * @param  pstds_handle: TDS���
 * @param  temperature: ����ˮ����¶�ֵ��Ĭ��ֵ25.0�棩
 * @retval 0 ��ȡ�ɹ�
 *         -1 ��ȡʧ��
 */
int8_t BSP_TDS_Read(bsp_tds_t *pstds_handle, float temperature);

/*
 * TDS��ʼ��
 * @brief ��ʼ��ADC�ӿ�
 * @param  pstds_handle: TDS���
 * @retval 0 ��ʼ���ɹ�
 *         -1 ��ʼ��ʧ��
 */
int8_t BSP_TDS_Init(bsp_tds_t *pstds_handle);