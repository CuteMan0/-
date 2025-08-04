#ifndef __ONEWIRE_DRIVER_H
#define __ONEWIRE_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief 1-Wire ƽ̨����ӿ�
     */
    typedef struct
    {
        void (*set_pin)(bool level);   // �����������ţ�0:���ͣ�1:�ͷţ�
        bool (*get_pin)(void);         // ��ȡ�������ŵ�ƽ
        void (*delay_us)(uint16_t us); // ΢����ʱ����
    } ow_io_interface_t;

    /**
     * @brief 1-Wire ���
     */
    typedef struct
    {
        ow_io_interface_t io_if;
    } ow_handle_t;

    /**
     * @brief ��ʼ�� 1-Wire ���ߣ�Ĭ�����ߣ�
     * @param ow: ���
     */
    void OW_Init(ow_handle_t *ow);

    /**
     * @brief ���͸�λ���������ź�
     * @param ow: ���
     * @return true: ���豸Ӧ��false: ����Ӧ
     */
    bool OW_Reset(ow_handle_t *ow, uint16_t rstl_time, uint16_t pdih_time, uint16_t rsth_time);

    /**
     * @brief д��һ���ֽڣ�LSB First��
     * @param ow: ���
     * @param byte: ��д����
     */
    void OW_WriteByte(ow_handle_t *ow, uint8_t byte);

    /**
     * @brief ��ȡһ���ֽڣ�LSB First��
     * @param ow: ���
     * @return ��ȡ�����ֽ�
     */
    uint8_t OW_ReadByte(ow_handle_t *ow);

#ifdef __cplusplus
}
#endif

#endif // __OW_DRIVER_H
