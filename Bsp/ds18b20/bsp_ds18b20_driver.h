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

/*                                    接口声明                                    */

/*
 *
 * @brief  DS18B20 实例化
 * @param  psd18b20_handle: DS18B20句柄
 * @param  pfow_finit: 1-Wire总线初始化函数
 * @param  pfow_reset: 1-Wire总线复位函数
 * @param  pfow_writebyte: 1-Wire 写字节函数
 * @param  pfow_readbyte: 1-Wire 读字节函数
 * @param  pfds18b20_init: DS18B20 初始化函数
 * @param  pfds18b20_readtemp: DS18B20 读取温度函数
 * @retval 0 实例化成功
 *         -1 1-Wire接口异常
 *         -2 实例化失败
 */
int8_t BSP_DS18B20_Inst(const bsp_ds18b20_handle_t *psds18b20_handle,
                        void (*pfow_init)(void),
                        bool (*pfow_reset)(uint16_t, uint16_t, uint16_t),
                        void (*pfow_writebyte)(uint8_t),
                        uint8_t (*pfow_readbyte)(void),
                        void (*pfds18b20_init)(bsp_ds18b20_handle_t *),
                        int8_t (*pfds18b20_readtemp)(bsp_ds18b20_handle_t *));

/*
 * DS18B20初始化
 * @brief 初始化DS18B20的1-Wire总线
 * @param  psd18b20_handle: DS18B20句柄
 * @retval void
 */
void BSP_DS18B20_Init(bsp_ds18b20_handle_t *psds18b20_handle);

/*
 * DS18B20读取温度
 * @brief 向DS18B20申请温度读取并转换数值
 * @param  psd18b20_handle: DS18B20句柄
 * @retval 0 读取成功
 *         -1 从机未响应
 */
int8_t BSP_DS18B20_ReadTemperature(bsp_ds18b20_handle_t *psds18b20_handle);