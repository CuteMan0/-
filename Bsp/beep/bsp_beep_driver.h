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
 * @brief  Beep ʵ����
 * @param  psbeep_handle: Beep���
 * @param  pfset_pin: Beep IO ���ú���
 * @param  pfdelay_ms: Beep ��ʱ����
 * @param  pfbeep_init: Beep ��ʼ������
 * @param  pfbeep_enable: Beep ʹ�ܺ���
 * @param  pfbeep_disable: Beep ���ú���
 * @retval 0 ʵ�����ɹ�
 *         -1 IO�ӿ��쳣
 *         -2 ʵ����ʧ��
 */
int8_t BSP_Beep_Instance(const bsp_beep_t *psbeep_handle,
                         void (*pfset_pin)(bool level),
                         void (*pfdelay_ms)(uint16_t ms),
                         void (*pfbeep_init)(bsp_beep_t *psbeep_handle),
                         void (*pfbeep_enable)(bsp_beep_t *psbeep_handle),
                         void (*pfbeep_disable)(bsp_beep_t *psbeep_handle));

/*
 * @brief  Beep ��ʼ��
 * @param  psbeep_handle: Beep���
 * @param  duty: ռ�ձ�
 * @retval None
 */
void BSP_Beep_Init(bsp_beep_t *psbeep_handle, uint8_t duty, uint8_t freq);
/*
 * @brief  Beep ʹ��
 * @param  psbeep_handle: Beep���
 * @retval None
 */
void BSP_Beep_Enable(bsp_beep_t *psbeep_handle);
/*
 * @brief  Beep ����
 * @param  psbeep_handle: Beep���
 * @retval None
 */
void BSP_Beep_Disable(bsp_beep_t *psbeep_handle);