#ifndef __BSP_HDCXX_DRIVER_H
#define __BSP_HDCXX_DRIVER_H

#include "stdbool.h"
#include "stdint.h"
#include <stddef.h>

typedef enum {
    HDC1080_SUCCESS              = 0,
    HDC1080_ERROR_HANDLE         = -1,
    HDC1080_ERROR_IIC_INST       = -2,
    HDC1080_ERROR_INSTANCE       = -3,
    HDC1080_ERROR_IIC_INIT       = -4,
    HDC1080_ERROR_INITIALIZATION = -5
} hdc1080_status_t;

typedef struct
{
    int8_t (*init)(void);
    int8_t (*deinit)(void);
    int8_t (*write)(uint8_t, uint8_t *, uint8_t);
    int8_t (*read)(uint8_t, uint8_t *, uint8_t);
    void (*delay_ms)(uint32_t);
} iic_driver_interface_t;

typedef struct bsp_hdc1080_t {
    iic_driver_interface_t *psiic_driver_if;

    float humidity;
    float temperature;

    hdc1080_status_t (*pfinit)(struct bsp_hdc1080_t *);
    hdc1080_status_t (*pfupdate)(struct bsp_hdc1080_t *);
} bsp_hdc1080_t;

/*                                    �ӿ�����                                    */

/*
 *
 * @brief  HDC1080ʵ����
 * @param  pshdc1080_handle: HDC1080���
 * @param  pfiic_init: iic ��ʼ������
 * @param  pfiic_deinit: iic ���ʼ������
 * @param  pfiic_write: iic д����
 * @param  pfiic_read: iic ������
 * @param  pfhdc1080_init: HDC1080 ��ʼ������
 * @param  pfhdc1080_update: HDC1080 ������ʪ�����ݺ���
 * @retval 0 ʵ�����ɹ�
 *         -1 HDC1080����쳣
 *         -2 iic�ӿ��쳣
 *         -3 ʵ����ʧ��
 */
hdc1080_status_t BSP_HDC1080_Inst(const bsp_hdc1080_t *pshdc1080_handle,
                                  void (*pfdelay_ms)(uint32_t),
                                  int8_t (*pfiic_init)(void),
                                  int8_t (*pfiic_deinit)(void),
                                  int8_t (*pfiic_write)(uint8_t, uint8_t *, uint8_t),
                                  int8_t (*pfiic_read)(uint8_t, uint8_t *, uint8_t),
                                  hdc1080_status_t (*pfhdc1080_init)(bsp_hdc1080_t *),
                                  hdc1080_status_t (*pfhdc1080_update)(bsp_hdc1080_t *));

/*
 * HDC1080��ʼ��
 * @brief ��ʼ��HDC1080��iic�ӿڣ�����λ��ʪ������
 * @param  pshdc1080_handle: HDC1080���
 * @retval void
 */
hdc1080_status_t BSP_HDC1080_Init(bsp_hdc1080_t *pshdc1080_handle);

/*
 * HDC1080��ȡ�¶�
 * @brief ��HDC1080������ʪ�ȶȶ�ȡ��ת����ֵ
 * @param  pshdc1080_handle: HDC1080���
 * @retval 0 ��ȡ�ɹ�
 */
hdc1080_status_t BSP_HDC1080_Update(bsp_hdc1080_t *pshdc1080_handle);

#endif /* __BSP_HDCXX_DRIVER_H__ */