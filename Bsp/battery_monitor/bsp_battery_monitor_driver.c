#include "bsp_battery_monitor_driver.h"

// 电池电压分压相关参数，根据实际电路设置
#define ADC_MAX_VALUE             4095.0f          // 12-bit ADC
#define VREF                      3.3f             // ADC参考电压，单位：V
#define R1                        100.0f           // 分压上电阻，单位：kΩ
#define R2                        100.0f           // 分压下电阻，单位：kΩ
#define VOLT_DIV_FACTOR           ((R1 + R2) / R2) // 实际电压 = ADC测得电压 * VOLT_DIV_FACTOR

#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

// 电池电压范围（示例，锂电池3.0~4.2V对应0~100%）
#define BATTERY_VOLT_MIN 3.0f // 0%
#define BATTERY_VOLT_MAX 4.2f // 100%

typedef struct {
    float voltage;
    float percentage;
} VoltageTable;

static const VoltageTable vtable[] = {
    {4.20f, 100.0f},
    {4.15f, 95.0f},
    {4.10f, 90.0f},
    {4.05f, 85.0f},
    {4.00f, 80.0f},
    {3.95f, 70.0f},
    {3.90f, 60.0f},
    {3.85f, 45.0f},
    {3.80f, 30.0f},
    {3.75f, 20.0f},
    {3.70f, 10.0f},
    {3.60f, 5.0f},
    {3.30f, 0.0f}};

static float bsp_get_voltage(uint16_t adc_raw)
{
    float v_adc = (adc_raw / ADC_MAX_VALUE) * VREF;
    return v_adc * VOLT_DIV_FACTOR;
}

static float bsp_voltage_to_percent(float voltage)
{
    voltage = constrain(voltage, 3.30f, 4.20f);
    for (uint8_t i = 0; i < sizeof(vtable) / sizeof(vtable[0]) - 1; i++) {
        if (voltage >= vtable[i + 1].voltage) {
            // 线性插值
            float v_diff = vtable[i].voltage - vtable[i + 1].voltage;
            float p_diff = vtable[i].percentage - vtable[i + 1].percentage;
            return vtable[i + 1].percentage +
                   (voltage - vtable[i + 1].voltage) * (p_diff / v_diff);
        }
    }
    return 0.0f;
}

void BSP_Bat_Mon_Init(bsp_battery_monitor_t *pbat_mon)
{
    pbat_mon->bat_level = 0xff; // 初始化为 0xff
}

void BSP_Bat_Mon_Update(bsp_battery_monitor_t *pbat_mon)
{
    uint16_t adc        = pbat_mon->adc_read();
    float voltage       = bsp_get_voltage(adc);
    pbat_mon->bat_level = bsp_voltage_to_percent(voltage);
}

int8_t BSP_Bat_Mon_Instance(const bsp_battery_monitor_t *pbat_mon,
                            uint32_t (*pfadc_read)(void),
                            void (*pfbat_mon_init)(bsp_battery_monitor_t *),
                            void (*pfbat_mon_update)(bsp_battery_monitor_t *))
{
    if (NULL == pbat_mon ||
        NULL == pfadc_read ||
        NULL == pfbat_mon_init ||
        NULL == pfbat_mon_update)
        return -1;

    // 强转去除 const
    bsp_battery_monitor_t *pstmp = (bsp_battery_monitor_t *)pbat_mon;

    pstmp->adc_read = pfadc_read;
    pstmp->pfinit   = pfbat_mon_init;
    pstmp->pfupdate = pfbat_mon_update;
    if (NULL == pstmp->adc_read ||
        NULL == pstmp->pfinit ||
        NULL == pstmp->pfupdate)
        return -2;

    return 0;
}