#ifndef __BSP_LTC4054_DRIVER_H
#define __BSP_LTC4054_DRIVER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef enum
{
    LTC4054_SUCCESS = 0,
    LTC4054_ERROR_HANDLE = -1,
    LTC4054_ERROR_IO_INST = -2,
    LTC4054_ERROR_INSTANCE = -3
} ltc4054_status_t;

typedef enum
{
    STATE_UNKNOWN,
    STATE_CHARGING,
    STATE_CHARGE_DONE,
    STATE_UVLO_OR_FLOAT
} ltc4054_charge_state_t;

typedef struct
{
    void (*set_out_high)(void);    // OUT = ����ߵ�ƽ
    void (*set_out_z)(void);       // OUT = ��������
    bool (*read_in)(void);         // ��ȡ CHRG ���ţ��� IN��
    void (*delay_ms)(uint16_t ms); // ��ʱ
} bsp_ltc4054_io_driver_t;

typedef struct bsp_ltc4054_t
{
    bsp_ltc4054_io_driver_t *psio_if;

    ltc4054_charge_state_t state;

    void (*pfinit)(struct bsp_ltc4054_t *self);
    void (*pfget_state)(struct bsp_ltc4054_t *self);
} bsp_ltc4054_t;

/**
 * @brief ��ʼ�� LTC4054 ʵ��
 * @param pltc4054 LTC4054 ���
 * @param pfio_setout_high ���� OUT ����Ϊ�ߵ�ƽ
 * @param pfio_setout_z ���� OUT ����Ϊ��������
 * @param pfio_readin ��ȡ IN ����
 * @param pfio_delay_ms ��ʱ����
 * @param pfltc4054_init LTC4054 ��ʼ������
 * @param pfltc4054_get_state LTC4054 ��ȡ״̬����
 * @retval ʵ����״̬
 */
ltc4054_status_t BSP_LTC4054_Inst(const bsp_ltc4054_t *pltc4054,
                                  void (*pfio_setout_high)(void),
                                  void (*pfio_setout_z)(void),
                                  bool (*pfio_readin)(void),
                                  void (*pfdelay_ms)(uint16_t),
                                  void (*pfltc4054_init)(bsp_ltc4054_t *),
                                  void (*pfltc4054_get_state)(bsp_ltc4054_t *));

/**
 * @brief ��ʼ�� LTC4054
 * @param pltc4054 LTC4054 ���
 */
void BSP_LTC4054_Init(bsp_ltc4054_t *pltc4054);
/**
 * @brief ��ȡ LTC4054 ״̬
 * @param pltc4054 LTC4054 ���
 */
void BSP_LTC4054_Get_State(bsp_ltc4054_t *pltc4054);

#endif /* __BSP_LTC4054_DRIVER_H */