#include "bsp_ltc4054_driver.h"

void BSP_LTC4054_Init(bsp_ltc4054_t *pltc4054)
{
    pltc4054->state = STATE_UNKNOWN;
}

void BSP_LTC4054_Get_State(bsp_ltc4054_t *pltc4054)
{
    // Step 1: OUT 输出高电平（拉高CHRG）
    pltc4054->psio_if->set_out_high();
    pltc4054->psio_if->delay_ms(1);

    if (!pltc4054->psio_if->read_in()) {
        // 正在充电
        pltc4054->state = STATE_CHARGING;
        return;
    }

    // Step 2: 进一步判断是否为 UVLO
    pltc4054->psio_if->set_out_z();
    pltc4054->psio_if->delay_ms(1);

    if (pltc4054->psio_if->read_in()) {
        pltc4054->state = STATE_UVLO_OR_FLOAT;
    } else {
        pltc4054->state = STATE_CHARGE_DONE;
    }
}

ltc4054_status_t BSP_LTC4054_Inst(const bsp_ltc4054_t *pltc4054,
                                  void (*pfio_setout_high)(void),
                                  void (*pfio_setout_z)(void),
                                  bool (*pfio_readin)(void),
                                  void (*pfdelay_ms)(uint16_t),
                                  void (*pfltc4054_init)(bsp_ltc4054_t *),
                                  void (*pfltc4054_get_state)(bsp_ltc4054_t *))
{
    if (NULL == pltc4054) {
        return LTC4054_ERROR_HANDLE;
    }

    bsp_ltc4054_t *pstmp                 = pltc4054;
    static bsp_ltc4054_io_driver_t io_if = {NULL, NULL, NULL, NULL};

    io_if.set_out_high = pfio_setout_high;
    io_if.set_out_z    = pfio_setout_z;
    io_if.read_in      = pfio_readin;
    io_if.delay_ms     = pfdelay_ms;

    if (NULL == io_if.set_out_high ||
        NULL == io_if.set_out_z ||
        NULL == io_if.read_in ||
        NULL == io_if.delay_ms) {
        return LTC4054_ERROR_IO_INST;
    }

    pstmp->psio_if     = &io_if;
    pstmp->pfinit      = pfltc4054_init;
    pstmp->pfget_state = pfltc4054_get_state;
    if (
        NULL == pstmp->psio_if ||
        NULL == pstmp->pfinit ||
        NULL == pstmp->pfget_state) {
        return LTC4054_ERROR_INSTANCE;
    }

    return LTC4054_SUCCESS;
}