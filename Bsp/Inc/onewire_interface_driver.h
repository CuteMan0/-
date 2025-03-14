#include <stdbool.h>
#include <stdint.h>

// 作为独立的1-Wire驱动文件应该需要提供delay_us函数和不同器件的1-Wire读/写一位数据函数的回调接口

/*                                    接口声明                                    */

/*
 * 1-Wire 总线初始化函数
 * @brief  使能并初始化 1-Wire 总线 GPIO 口
 * @param  void
 * @retval void
 */
void Onewire_Init(void);

/*
 * 1-Wire 总线复位函数
 * @brief  发送复位脉冲并读取应答信号
 * @param  rst_time: 复位脉冲的持续时间-us
 * @param  rsp_time: 从机响应信号持续时间-us
 * @param  wait_time: 应答信号建立及从机释放总线的等待时间-us
 * @retval 1: 应答成功
 *         0: 应答失败
 */
bool Onewire_Reset(uint16_t rst_time, uint16_t rsp_time, uint16_t wait_time);

/*
 * 1-Wire 总线上写入一个字节
 * @brief  采用低位先行（LSB）方式写入一个字节
 * @param  data: 写入的字节数据
 * @retval void
 */
void Onewire_WriteByte(uint8_t data);

/*
 * 1-Wire 总线上读取一个字节
 * @brief  采用低位先行（LSB）方式读取一个字节
 * @param  void
 * @retval 读取的字节数据
 */
uint8_t Onewire_ReadByte(void);
