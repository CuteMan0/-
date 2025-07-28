#include "bsp_beep_driver.h"

uint8_t tick = 0;

void BSP_Beep_Init(bsp_beep_t *psbeep_handle, uint8_t duty, uint8_t freq)
{
    psbeep_handle->psio_if->set_pin(!LOGIC);
    psbeep_handle->duty = duty;
    psbeep_handle->freq = freq;
}

void BSP_Beep_Enable(bsp_beep_t *psbeep_handle)
{
    const uint16_t period_ticks = 1000 / psbeep_handle->freq;
    const uint8_t active_ticks  = (psbeep_handle->duty * period_ticks) / 100;

    bool pin_state = (tick < active_ticks) ? LOGIC : !LOGIC;
    psbeep_handle->psio_if->set_pin(pin_state);

    if (tick < period_ticks) {
        psbeep_handle->psio_if->delay_ms(1);
        tick++;
    } else {
        tick = 0;
    }
}

/*
 * @brief  Beep ½ûÓÃ
 * @param  psbeep_handle: Beep¾ä±ú
 * @retval None
 */
void BSP_Beep_Disable(bsp_beep_t *psbeep_handle)
{
    psbeep_handle->psio_if->set_pin(!LOGIC);
    tick = 0;
}

int8_t BSP_Beep_Instance(const bsp_beep_t *psbeep_handle,
                         void (*pfset_pin)(bool level),
                         void (*pfdelay_ms)(uint16_t ms),
                         void (*pfbeep_init)(bsp_beep_t *psbeep_handle, uint8_t duty, uint8_t freq),
                         void (*pfbeep_enable)(bsp_beep_t *psbeep_handle),
                         void (*pfbeep_disable)(bsp_beep_t *psbeep_handle))
{
    if (NULL == psbeep_handle) {
        return -1;
    }
    bsp_beep_t *pstmp                 = psbeep_handle;
    static bsp_beep_io_driver_t io_if = {NULL, NULL};

    io_if.set_pin  = pfset_pin;
    io_if.delay_ms = pfdelay_ms;

    if (NULL == io_if.set_pin ||
        NULL == io_if.delay_ms) {
        return -2;
    }

    pstmp->psio_if   = &io_if;
    pstmp->pfinit    = pfbeep_init;
    pstmp->pfenable  = pfbeep_enable;
    pstmp->pfdisable = pfbeep_disable;

    if (NULL == pstmp->psio_if ||
        NULL == pstmp->pfinit ||
        NULL == pstmp->pfenable ||
        NULL == pstmp->pfdisable) {
        return -3;
    }

    return 0;
}