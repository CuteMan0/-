#ifndef __BSP_DHTXX_DRIVER_H__
#define __BSP_DHTXX_DRIVER_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define DEV_ADDRESS 0x38

typedef enum {
    DHT20_SUCCESS              = 0,
    DHT20_ERROR_DELAY_INST     = -1,
    DHT20_ERROR_IIC_INST       = -2,
    DHT20_ERROR_INSTANCE       = -3,
    DHT20_ERROR_IIC_INIT       = -4,
    DHT20_ERROR_INITIALIZATION = -5
} dht20_status_t;

typedef struct {
    void (*pf_ms)(uint32_t);
} delay_driver_interface_t;

typedef struct {
    int8_t (*pfinit)(void);
    int8_t (*pfdeinit)(void);
    int8_t (*pfwrite)(uint8_t, uint8_t *, uint16_t);
    int8_t (*pfread)(uint8_t, uint8_t *, uint16_t);
} iic_driver_interface_t;

typedef struct bsp_dht20_t {
    delay_driver_interface_t *psdelay_driver_if;
    iic_driver_interface_t *psiic_driver_if;

    dht20_status_t status;
    float humidity;
    float temperature;
    uint8_t crc_val;

    dht20_status_t (*pfinit)(struct bsp_dht20_t *);
    dht20_status_t (*pfupdate)(struct bsp_dht20_t *);
} bsp_dht20_t;

/*                                    �ӿ�����                                    */

/*
 *
 * @brief  DHT20ʵ����
 * @param  psdht20_handle: DHT20���
 * @param  pfiic_init: iic ��ʼ������
 * @param  pfiic_deinit: iic ���ʼ������
 * @param  pfiic_write: iic д����
 * @param  pfiic_read: iic ������
 * @param  pfdht20_init: DHT20 ��ʼ������
 * @param  pfdht20_update: DHT20 ������ʪ�����ݺ���
 * @retval 0 ʵ�����ɹ�
 *         -1 delay�ӿ��쳣
 *         -2 iic�ӿ��쳣
 *         -3 ʵ����ʧ��
 */
dht20_status_t BSP_DHT20_Inst(const bsp_dht20_t *psdht20_handle,
                              void (*pfdelay_ms)(uint32_t),
                              int8_t (*pfiic_init)(void),
                              int8_t (*pfiic_deinit)(void),
                              int8_t (*pfiic_write)(uint8_t, uint8_t *, uint16_t),
                              int8_t (*pfiic_read)(uint8_t, uint8_t *, uint16_t),
                              dht20_status_t (*pfdht20_init)(bsp_dht20_t *),
                              dht20_status_t (*pfdht20_update)(bsp_dht20_t *));

/*
 * DHT20��ʼ��
 * @brief ��ʼ��DHT20��iic�ӿڣ�����λ��ʪ������
 * @param  psdht20_handle: DHT20���
 * @retval void
 */
dht20_status_t BSP_DHT20_Init(bsp_dht20_t *psdht20_handle);

/*
 * DHT20��ȡ�¶�
 * @brief ��DHT20������ʪ�ȶȶ�ȡ��ת����ֵ
 * @param  psdht20_handle: DHT20���
 * @retval 0 ��ȡ�ɹ�
 */
dht20_status_t BSP_DHT20_Update(bsp_dht20_t *psdht20_handle);

#endif // __BSP_DHTXX_DRIVER_H__