#include "bsp_ds18b20_driver.h"

void BSP_DS18B20_Init(bsp_ds18b20_handle_t *psds18b20_handle)
{
    psds18b20_handle->temperature = 0.0f;
    psds18b20_handle->psow_dirve_if->init();
}

int8_t BSP_DS18B20_ReadTemperature(bsp_ds18b20_handle_t *psds18b20_handle)
{

    if (0 == psds18b20_handle->psow_dirve_if->reset(480, 60, 480)) {
        return -1; // Reset failed
    }
    psds18b20_handle->psow_dirve_if->writebyte(0xCC); // Skip ROM command
    psds18b20_handle->psow_dirve_if->writebyte(0x44); // Start conversion command

    if (0 == psds18b20_handle->psow_dirve_if->reset(480, 60, 480)) {
        return -1; // Reset failed
    }
    psds18b20_handle->psow_dirve_if->writebyte(0xCC); // Skip ROM command
    psds18b20_handle->psow_dirve_if->writebyte(0xBE); // Read scratchpad command

    uint8_t tmpl = psds18b20_handle->psow_dirve_if->readbyte();
    uint8_t tmph = psds18b20_handle->psow_dirve_if->readbyte();
    if (tmph & 0x08) {
        tmph = ~tmph;
        tmpl = ~tmpl + 1;
    }

    uint16_t tmp                  = (tmph << 8) | tmpl;
    psds18b20_handle->temperature = (float)tmp * 0.0625;

    return 0;
}

int8_t BSP_DS18B20_Inst(const bsp_ds18b20_handle_t *psds18b20_handle,
                        void (*pfow_init)(void),
                        bool (*pfow_reset)(uint16_t, uint16_t, uint16_t),
                        void (*pfow_writebyte)(uint8_t),
                        uint8_t (*pfow_readbyte)(void),
                        void (*pfds18b20_init)(bsp_ds18b20_handle_t *),
                        int8_t (*pfds18b20_readtemp)(bsp_ds18b20_handle_t *))
{
    bsp_ds18b20_handle_t *pstmp          = psds18b20_handle;
    static ow_driver_interface_t ow_d_if = {NULL, NULL, NULL, NULL};

    ow_d_if.init      = pfow_init;
    ow_d_if.reset     = pfow_reset;
    ow_d_if.writebyte = pfow_writebyte;
    ow_d_if.readbyte  = pfow_readbyte;
    if (NULL == ow_d_if.init ||
        NULL == ow_d_if.reset ||
        NULL == ow_d_if.writebyte ||
        NULL == ow_d_if.readbyte) {
        return -1;
    }

    pstmp->psow_dirve_if = &ow_d_if;
    pstmp->pfinit        = pfds18b20_init;
    pstmp->pfreadtemp    = pfds18b20_readtemp;

    if (NULL == pstmp->psow_dirve_if ||
        NULL == pstmp->pfinit ||
        NULL == pstmp->pfreadtemp) {
        return -2;
    }

    return 0;
}