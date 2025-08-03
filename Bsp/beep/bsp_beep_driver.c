#include "bsp_beep_driver.h"

uint8_t tick = 0;

void BSP_Beep_Init(bsp_beep_t *pbeep, uint8_t duty, uint8_t freq)
{
    pbeep->psio_if->set_pin(!LOGIC);
    pbeep->duty = duty;
    pbeep->freq = freq;
}

void BSP_Beep_Enable(bsp_beep_t *pbeep)
{
    const uint16_t period_ticks = 1000 / pbeep->freq;
    const uint8_t active_ticks  = (pbeep->duty * period_ticks) / 100;

    bool pin_state = (tick < active_ticks) ? LOGIC : !LOGIC;
    pbeep->psio_if->set_pin(pin_state);

    if (tick < period_ticks) {
        pbeep->psio_if->delay_ms(1);
        tick++;
    } else {
        tick = 0;
    }
}

/*
 * @brief  Beep ½ûÓÃ
 * @param  pbeep: Beep¾ä±ú
 * @retval None
 */
void BSP_Beep_Disable(bsp_beep_t *pbeep)
{
    pbeep->psio_if->set_pin(!LOGIC);
    tick = 0;
}

int8_t BSP_Beep_Instance(const bsp_beep_t *pbeep,
                         void (*pfset_pin)(bool),
                         void (*pfdelay_ms)(uint16_t),
                         void (*pfbeep_init)(bsp_beep_t *, uint8_t, uint8_t),
                         void (*pfbeep_enable)(bsp_beep_t *),
                         void (*pfbeep_disable)(bsp_beep_t *))
{
    if (NULL == pbeep) {
        return -1;
    }
    bsp_beep_t *pstmp                 = pbeep;
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