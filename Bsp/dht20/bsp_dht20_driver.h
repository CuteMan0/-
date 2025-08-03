#ifndef __BSP_DHT20_DRIVER_H
#define __BSP_DHT20_DRIVER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define DEV_ADDRESS 0x38

typedef enum {
    DHT20_SUCCESS = 0,
    DHT20_ERROR_IIC_INST,
    DHT20_ERROR_INSTANCE,
    DHT20_ERROR_IIC_INIT,
    DHT20_ERROR_INITIALIZATION
} dht20_status_t;

typedef struct {
    int8_t (*init)(void);
    int8_t (*write)(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len);
    int8_t (*read)(uint8_t addr, uint8_t *data, uint8_t len);
    void (*delay_ms)(uint16_t ms);
} iic_driver_interface_t;

typedef struct bsp_dht20_t {
    iic_driver_interface_t *psiic_driver_if;

    dht20_status_t status;
    float humidity;
    float temperature;
    uint8_t crc_val;

    dht20_status_t (*pfinit)(struct bsp_dht20_t *self);
    dht20_status_t (*pfupdate)(struct bsp_dht20_t *self);
} bsp_dht20_handle_t;

/*                                    �ӿ�����                                    */

/*
 *
 * @brief  DHT20ʵ����
 * @param  pdht20: DHT20���
 * @param  pfdelay_ms: ms����ʱ����
 * @param  pfiic_init: iic ��ʼ������
 * @param  pfiic_write: iic д����
 * @param  pfiic_read: iic ������
 * @param  pfdht20_init: DHT20 ��ʼ������
 * @param  pfdht20_update: DHT20 ������ʪ�����ݺ���
 * @retval 0 ʵ�����ɹ�
 *         -1 delay�ӿ��쳣
 *         -2 iic�ӿ��쳣
 *         -3 ʵ����ʧ��
 */
dht20_status_t BSP_DHT20_Inst(const bsp_dht20_handle_t *pdht20,
                              void (*pfdelay_ms)(uint16_t),
                              int8_t (*pfiic_init)(void),
                              int8_t (*pfiic_write)(uint8_t, uint8_t, uint8_t *, uint8_t),
                              int8_t (*pfiic_read)(uint8_t, uint8_t *, uint8_t),
                              dht20_status_t (*pfdht20_init)(bsp_dht20_handle_t *),
                              dht20_status_t (*pfdht20_update)(bsp_dht20_handle_t *));

/*
 * DHT20��ʼ��
 * @brief ��ʼ��DHT20��iic�ӿڣ�����λ��ʪ������
 * @param  pdht20: DHT20���
 * @retval void
 */
dht20_status_t BSP_DHT20_Init(bsp_dht20_handle_t *pdht20);

/*
 * DHT20��ȡ�¶�
 * @brief ��DHT20������ʪ�ȶȶ�ȡ��ת����ֵ
 * @param  pdht20: DHT20���
 * @retval 0 ��ȡ�ɹ�
 */
dht20_status_t BSP_DHT20_Update(bsp_dht20_handle_t *pdht20);

#endif /* __BSP_DHT20_DRIVER_H */