#ifndef __ONEWIRE_DRIVER_H
#define __ONEWIRE_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief 1-Wire 平台抽象接口
     */
    typedef struct
    {
        void (*set_pin)(bool level);   // 设置总线引脚（0:拉低，1:释放）
        bool (*get_pin)(void);         // 读取总线引脚电平
        void (*delay_us)(uint16_t us); // 微秒延时函数
    } ow_io_interface_t;

    /**
     * @brief 1-Wire 句柄
     */
    typedef struct
    {
        ow_io_interface_t io_if;
    } ow_handle_t;

    /**
     * @brief 初始化 1-Wire 总线（默认拉高）
     * @param ow: 句柄
     */
    void OW_Init(ow_handle_t *ow);

    /**
     * @brief 发送复位并检测存在信号
     * @param ow: 句柄
     * @return true: 有设备应答，false: 无响应
     */
    bool OW_Reset(ow_handle_t *ow, uint16_t rstl_time, uint16_t pdih_time, uint16_t rsth_time);

    /**
     * @brief 写入一个字节（LSB First）
     * @param ow: 句柄
     * @param byte: 待写数据
     */
    void OW_WriteByte(ow_handle_t *ow, uint8_t byte);

    /**
     * @brief 读取一个字节（LSB First）
     * @param ow: 句柄
     * @return 读取到的字节
     */
    uint8_t OW_ReadByte(ow_handle_t *ow);

#ifdef __cplusplus
}
#endif

#endif // __OW_DRIVER_H
