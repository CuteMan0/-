#ifndef __BSP_BEEP_DRIVER_H
#define __BSP_BEEP_DRIVER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct
{
    void (*set_pin)(bool level);
    void (*delay_ms)(uint16_t ms);
} bsp_beep_io_driver_t;

typedef struct bsp_beep_t {
    bsp_beep_io_driver_t *psio_if;
    uint8_t duty; // 0~100
    uint8_t freq; // 0~5Hz

    void (*pfinit)(struct bsp_beep_t *self, uint8_t duty, uint8_t freq);
    void (*pfenable)(struct bsp_beep_t *self);
    void (*pfdisable)(struct bsp_beep_t *self);
} bsp_beep_t;

#define LOGIC 1 // 1 : high active, 0: low active

/*
 *
 * @brief  Beep ʵ����
 * @param  psbeep_handle: Beep���
 * @param  pfset_pin: Beep IO ���ú���
 * @param  pfdelay_ms: Beep ��ʱ����
 * @param  pfbeep_init: Beep ��ʼ������
 * @param  pfbeep_enable: Beep ʹ�ܺ���
 * @param  pfbeep_disable: Beep ���ú���
 * @retval 0 ʵ�����ɹ�
 *         -1 ����쳣
 *         -2 IO�ӿ��쳣
 *         -3 ʵ����ʧ��
 */
int8_t BSP_Beep_Instance(const bsp_beep_t *pbeep,
                         void (*pfset_pin)(bool),
                         void (*pfdelay_ms)(uint16_t),
                         void (*pfbeep_init)(bsp_beep_t *, uint8_t, uint8_t),
                         void (*pfbeep_enable)(bsp_beep_t *),
                         void (*pfbeep_disable)(bsp_beep_t *));

/*
 * @brief  Beep ��ʼ��
 * @param  psbeep_handle: Beep���
 * @param  duty: ռ�ձ�
 * @retval None
 */
void BSP_Beep_Init(bsp_beep_t *pbeep, uint8_t duty, uint8_t freq);
/*
 * @brief  Beep ʹ��
 * @param  psbeep_handle: Beep���
 * @retval None
 */
void BSP_Beep_Enable(bsp_beep_t *pbeep);
/*
 * @brief  Beep ����
 * @param  psbeep_handle: Beep���
 * @retval None
 */
void BSP_Beep_Disable(bsp_beep_t *pbeep);

#endif /* __BSP_BEEP_DRIVER_H */