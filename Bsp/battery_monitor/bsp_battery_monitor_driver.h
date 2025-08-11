#ifndef __BSP_BATTERY_MONITOR_DRIVER_H
#define __BSP_BATTERY_MONITOR_DRIVER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @brief ��ؼ�����ṹ��
 */
typedef struct bsp_battery_monitor_t {
    /**
     * @brief ADC��ȡ����ָ�룬�û���ʵ�֣�����ADCԭʼֵ
     */
    uint32_t (*adc_read)(void);

    /**
     * @brief ��ǰ��ص����ٷֱȣ���Χ0~100
     */
    float state_of_charge;

    /**
     * @brief ��ʼ���ص����������û�ʵ��
     * @param self ��ǰ�ṹ��ָ��
     */
    void (*pfinit)(struct bsp_battery_monitor_t *self);

    /**
     * @brief ���»ص����������û�ʵ��
     * @param self ��ǰ�ṹ��ָ��
     */
    void (*pfupdate)(struct bsp_battery_monitor_t *self);
} bsp_battery_monitor_t;

/**
 * @brief ʵ������ؼ��ģ��
 *
 * @param pbat_mon       ��ؼ����ָ��
 * @param pfadc_read     ADC��ȡ����ָ�룬����uint16_tԭʼADCֵ
 * @param pfbat_mon_init ��ʼ���ص�����ָ��
 * @param pfbat_mon_update ���»ص�����ָ��
 *
 * @retval  0 ʵ�����ɹ�
 * @retval -1 �����쳣����ָ�룩
 * @retval -2 ���������쳣����ָ�룩
 */
int8_t BSP_Bat_Mon_Instance(const bsp_battery_monitor_t *pbat_mon,
                            uint32_t (*pfadc_read)(void),
                            void (*pfbat_mon_init)(bsp_battery_monitor_t *),
                            void (*pfbat_mon_update)(bsp_battery_monitor_t *));

/**
 * @brief ��ؼ���ʼ������
 *
 * @param pbat_mon   ��ؼ����ָ��
 * @param bat_level  ��ʼ����ֵ��������0����ʵֵ��ADC������ã�
 */
void BSP_Bat_Mon_Init(bsp_battery_monitor_t *pbat_mon);

/**
 * @brief ��ؼ����º���
 *
 * @param pbat_mon ��ؼ����ָ��
 *
 * @note �˺���Ӧ�������Ե��ã����ڸ��µ�ص�����ִ�лص�
 */
void BSP_Bat_Mon_Update(bsp_battery_monitor_t *pbat_mon);

#endif /* __BSP_BATTERY_MONITOR_DRIVER_H */
