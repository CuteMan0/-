#include "bsp_battery_monitor_driver.h"

#include <math.h>

// ��ص�ѹ��ѹ��ز���������ʵ�ʵ�·����
#define ADC_MAX_VALUE             4095.0f          // 12-bit ADC
#define VREF                      3.3f             // ADC�ο���ѹ����λ��V
#define R1                        100.0f           // ��ѹ�ϵ��裬��λ��k��
#define R2                        100.0f           // ��ѹ�µ��裬��λ��k��
#define VOLT_DIV_FACTOR           ((R1 + R2) / R2) // ʵ�ʵ�ѹ = ADC��õ�ѹ * VOLT_DIV_FACTOR

#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

// ��ص�ѹ��Χ��ʾ����﮵��3.0~4.2V��Ӧ0~100%��
#define BATTERY_VOLT_MIN 3.0f // 0%
#define BATTERY_VOLT_MAX 4.2f // 100%

typedef struct
{
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

// ---------- �˲� & �ͻ� ���� ----------
#define FILTER_ALPHA   0.15f // ��ͨ�˲�ϵ�� (0~1)��ԽСԽ��
#define SOC_HYSTERESIS 0.5f  // �ٷֱȱ仯С�����ֵ�򲻸���

// ---------- SOC����----------
static float bsp_get_soc(uint32_t adc_raw)
{
    static uint32_t adc_filtered = 0;    // �˲���� ADC ֵ
    static float last_percent    = 0.0f; // ��һ����ʾ�İٷֱ�

    // ��ͨ�˲���ADC��
    if (adc_filtered == 0) {
        adc_filtered = adc_raw;
    }
    adc_filtered = adc_filtered * (1.0f - FILTER_ALPHA) + adc_raw * FILTER_ALPHA;

    // �����ѹ
    float voltage = (adc_filtered / ADC_MAX_VALUE) * VREF * VOLT_DIV_FACTOR;

    // ���Ʒ�Χ
    voltage = constrain(voltage, 3.30f, 4.20f);

    // �ٷֱȣ����Բ�ֵ��
    float new_percent = 0.0f;
    for (uint8_t i = 0; i < (sizeof(vtable) / sizeof(vtable[0])) - 1; i++) {
        if (voltage >= vtable[i + 1].voltage) {
            float v_diff = vtable[i].voltage - vtable[i + 1].voltage;
            float p_diff = vtable[i].percentage - vtable[i + 1].percentage;
            new_percent  = vtable[i + 1].percentage +
                          (voltage - vtable[i + 1].voltage) * (p_diff / v_diff);
            break;
        }
    }

    // �ͻش���
    if (fabsf(new_percent - last_percent) >= SOC_HYSTERESIS) {
        last_percent = new_percent;
    }

    return last_percent;
}

void BSP_Bat_Mon_Init(bsp_battery_monitor_t *pbat_mon)
{
    pbat_mon->state_of_charge = 0xff; // ��ʼ��Ϊ��Чֵ
}

void BSP_Bat_Mon_Update(bsp_battery_monitor_t *pbat_mon)
{
    pbat_mon->state_of_charge = bsp_get_soc(pbat_mon->adc_read());
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

    // ǿתȥ�� const
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