#include "ringBuff.h"

/**
 * @brief 环形缓冲区初始化
 *
 * @param l_ringBuff 缓冲区的句柄
 */
void RingBuff_Init(RingBuff_t *l_ringBuff)
{
    l_ringBuff->Head   = 0;
    l_ringBuff->Tail   = 0;
    l_ringBuff->Lenght = 0;
}

/**
 * @brief 环形缓冲区批量写入
 *
 * @param l_ringBuff 缓冲区的句柄
 * @param data 用于缓存收到的数据
 * @param size 数据长度
 * @return RingBuff_Status FLASE:环形缓冲区已满，写入失败;TRUE:写入成功
 */
RingBuff_Status Write_RingBuff_Multi(RingBuff_t *l_ringBuff, uint8_t *data, uint8_t size)
{
    if (l_ringBuff->Lenght + size > RINGBUFF_LEN) {
        return RINGBUFF_ERR;
    }
    for (uint8_t i = 0; i < size; i++) {
        l_ringBuff->Ring_data[l_ringBuff->Tail] = *data;
        l_ringBuff->Tail                        = (l_ringBuff->Tail + 1) % RINGBUFF_LEN;
        l_ringBuff->Lenght++;
        data++;
    }
    return RINGBUFF_OK;
}

/**
 * @brief 环形缓冲区批量读出
 *
 * @param l_ringBuff 缓冲区的句柄
 * @param rdata 用于保存读取的数据
 * @param size 数据长度
 * @return RingBuff_Status FLASE:环形缓冲区已满，写入失败;TRUE:写入成功
 */
RingBuff_Status Read_RingBuff_Multi(RingBuff_t *l_ringBuff, uint8_t *rdata, uint8_t size)
{
    if (l_ringBuff->Lenght - size < 0) {
        return RINGBUFF_ERR;
    }
    for (uint8_t i = 0; i < size; i++) {
        *rdata           = l_ringBuff->Ring_data[l_ringBuff->Head];
        l_ringBuff->Head = (l_ringBuff->Head + 1) % RINGBUFF_LEN;
        l_ringBuff->Lenght--;
        rdata++;
    }
    return RINGBUFF_OK;
}

/**
 * @brief 环形缓冲区写入
 *
 * @param l_ringBuff 缓冲区的句柄
 * @param data 用于缓存收到的数据
 * @return RingBuff_Status FLASE:环形缓冲区已满，写入失败;TRUE:写入成功
 */
RingBuff_Status Write_RingBuff(RingBuff_t *l_ringBuff, uint8_t data)
{
    if (l_ringBuff->Lenght >= RINGBUFF_LEN) // 判断缓冲区是否已满
    {
        return RINGBUFF_ERR;
    }
    l_ringBuff->Ring_data[l_ringBuff->Tail] = data;
    l_ringBuff->Tail                        = (l_ringBuff->Tail + 1) % RINGBUFF_LEN; // 防止越界非法访问
    l_ringBuff->Lenght++;
    return RINGBUFF_OK;
}

/**
 * @brief 环形缓冲区读出
 *
 * @param l_ringBuff 缓冲区的句柄 缓冲区的句柄
 * @param rdata 用于保存读取的数据
 * @return RingBuff_Status FLASE:环形缓冲区已满，写入失败;TRUE:写入成功
 */
RingBuff_Status Read_RingBuff(RingBuff_t *l_ringBuff, uint8_t *rdata)
{
    if (l_ringBuff->Lenght == 0) // 判断非空
    {
        return RINGBUFF_ERR;
    }
    *rdata           = l_ringBuff->Ring_data[l_ringBuff->Head]; // 先进先出FIFO，从缓冲区头出
    l_ringBuff->Head = (l_ringBuff->Head + 1) % RINGBUFF_LEN;   // 防止越界非法访问
    l_ringBuff->Lenght--;
    return RINGBUFF_OK;
}