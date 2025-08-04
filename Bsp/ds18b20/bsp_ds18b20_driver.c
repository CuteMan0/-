#include "bsp_ds18b20_driver.h"

#include "onewire_interface_driver.h"

void BSP_DS18B20_Init(bsp_ds18b20_t *pds18b20)
{
    pds18b20->temperature = 0.0f;
    pds18b20->psonewire_dirve_if->pfinit();
}

int8_t BSP_DS18B20_ReadTemperature(bsp_ds18b20_t *pds18b20)
{

    if (0 == pds18b20->psonewire_dirve_if->pfreset(480, 60, 480))
    {
        return -1; // Reset failed
    }
    pds18b20->psonewire_dirve_if->pfwritebyte(0xCC); // Skip ROM command
    pds18b20->psonewire_dirve_if->pfwritebyte(0x44); // Start conversion command

    if (0 == pds18b20->psonewire_dirve_if->pfreset(480, 60, 480))
    {
        return -1; // Reset failed
    }

    pds18b20->psonewire_dirve_if->delay_ms(750);

    pds18b20->psonewire_dirve_if->pfwritebyte(0xCC); // Skip ROM command
    pds18b20->psonewire_dirve_if->pfwritebyte(0xBE); // Read scratchpad command

    uint8_t tmpl = pds18b20->psonewire_dirve_if->pfreadbyte();
    uint8_t tmph = pds18b20->psonewire_dirve_if->pfreadbyte();
    if (tmph & 0x08)
    {
        tmph = ~tmph;
        tmpl = ~tmpl + 1;
    }

    uint16_t tmp = (tmph << 8) | tmpl;
    pds18b20->temperature = (float)tmp * 0.0625f;

    return 0;
}

int8_t BSP_DS18B20_Inst(const bsp_ds18b20_t *pds18b20,
                        void (*pfow_init)(void),
                        bool (*pfow_reset)(uint16_t, uint16_t, uint16_t),
                        void (*pfow_writebyte)(uint8_t),
                        uint8_t (*pfow_readbyte)(void),
                        void (*pfow_delayms)(uint16_t),
                        void (*pfds18b20_init)(bsp_ds18b20_t *),
                        int8_t (*pfds18b20_readtemp)(bsp_ds18b20_t *))
{
    if (NULL == pds18b20)
    {
        return;
    }
    bsp_ds18b20_t *pstmp = pds18b20;
    static ow_driver_interface_t ow_d_if = {NULL, NULL, NULL, NULL, NULL};

    ow_d_if.pfinit = pfow_init;
    ow_d_if.pfreset = pfow_reset;
    ow_d_if.pfwritebyte = pfow_writebyte;
    ow_d_if.pfreadbyte = pfow_readbyte;
    ow_d_if.pfdelay_ms = pfow_delayms;
    if (NULL == ow_d_if.pfinit ||
        NULL == ow_d_if.pfreset ||
        NULL == ow_d_if.pfwritebyte ||
        NULL == ow_d_if.pfreadbyte ||
        NULL == ow_d_if.pfdelay_ms)
    {
        return -1;
    }

    pstmp->psonewire_dirve_if = &ow_d_if;
    pstmp->pfinit = pfds18b20_init;
    pstmp->pfreadtemp = pfds18b20_readtemp;

    if (NULL == pstmp->psonewire_dirve_if ||
        NULL == pstmp->pfinit ||
        NULL == pstmp->pfreadtemp)
    {
        return -2;
    }

    return 0;
}