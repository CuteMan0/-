#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct
{
    void (*init)(void);
    bool (*reset)(uint16_t, uint16_t, uint16_t);
    void (*writebyte)(uint8_t);
    uint8_t (*readbyte)(void);
} ow_driver_interface_t;

typedef struct bsp_ds18b20_t {
    ow_driver_interface_t *psow_dirve_if;

    float temperature;

    void (*pfinit)(struct bsp_ds18b20_t *);
    int8_t (*pfreadtemp)(struct bsp_ds18b20_t *);

} bsp_ds18b20_handle_t;

/*                                    �ӿ�����                                    */

/*
 *
 * @brief  DS18B20 ʵ����
 * @param  psd18b20_handle: DS18B20���
 * @param  pfow_finit: 1-Wire���߳�ʼ������
 * @param  pfow_reset: 1-Wire���߸�λ����
 * @param  pfow_writebyte: 1-Wire д�ֽں���
 * @param  pfow_readbyte: 1-Wire ���ֽں���
 * @param  pfds18b20_init: DS18B20 ��ʼ������
 * @param  pfds18b20_readtemp: DS18B20 ��ȡ�¶Ⱥ���
 * @retval 0 ʵ�����ɹ�
 *         -1 1-Wire�ӿ��쳣
 *         -2 ʵ����ʧ��
 */
int8_t BSP_DS18B20_Inst(const bsp_ds18b20_handle_t *psds18b20_handle,
                        void (*pfow_init)(void),
                        bool (*pfow_reset)(uint16_t, uint16_t, uint16_t),
                        void (*pfow_writebyte)(uint8_t),
                        uint8_t (*pfow_readbyte)(void),
                        void (*pfds18b20_init)(bsp_ds18b20_handle_t *),
                        int8_t (*pfds18b20_readtemp)(bsp_ds18b20_handle_t *));

/*
 * DS18B20��ʼ��
 * @brief ��ʼ��DS18B20��1-Wire����
 * @param  psd18b20_handle: DS18B20���
 * @retval void
 */
void BSP_DS18B20_Init(bsp_ds18b20_handle_t *psds18b20_handle);

/*
 * DS18B20��ȡ�¶�
 * @brief ��DS18B20�����¶ȶ�ȡ��ת����ֵ
 * @param  psd18b20_handle: DS18B20���
 * @retval 0 ��ȡ�ɹ�
 *         -1 �ӻ�δ��Ӧ
 */
int8_t BSP_DS18B20_ReadTemperature(bsp_ds18b20_handle_t *psds18b20_handle);