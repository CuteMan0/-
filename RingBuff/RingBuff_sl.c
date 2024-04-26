#include "RingBuff_sl.h"

/**
 * @brief 环形缓冲区初始化
 *
 * @param l_rb 缓冲区句柄
 */
void RingBuff_Init(RB_t *l_rb)
{
    l_rb->Lenght = l_rb->Capicity = l_rb->Head = l_rb->Tail = 0;
    l_rb->Array                                             = NULL;
}

/**
 * @brief 写入环形缓冲区
 *
 * @param l_rb 环形缓冲区句柄
 * @param data 用于缓存写入的数据
 * @return RB_Status
 */
RB_Status Write_RingBuff(RB_t *l_rb, RBDat_t data)
{
    // 缓冲区满
    if (l_rb->Lenght >= l_rb->Capicity) {
        uint32_t newcap = l_rb->Capicity == 0 ? RINGBUFF_LEN : l_rb->Capicity * 2;
        RBDat_t *temp   = (RBDat_t *)realloc(l_rb->Array, newcap * sizeof(RBDat_t));
        if (temp == NULL) {
            // 堆区放不下
            return FLASE;
        }
        l_rb->Array    = temp;
        l_rb->Capicity = newcap;
    }
    l_rb->Array[l_rb->Tail] = data;
    // 缓冲区临满
    if ((l_rb->Tail + 1) % l_rb->Capicity == l_rb->Head) {
        // 预先指向未分配的内存
        l_rb->Tail++;
    } else {
        l_rb->Tail = (l_rb->Tail + 1) % l_rb->Capicity;
    }
    l_rb->Lenght++;
    return TRUE;
}

/**
 * @brief 读出环形缓冲区
 *
 * @param l_rb 缓冲区句柄
 * @param rdata 用于存储读出的数据
 * @return RB_Status
 */
RB_Status Read_RingBuff(RB_t *l_rb, RBDat_t *rdata)
{
    // 缓冲区空
    if (l_rb->Lenght == 0) {
        // 如果Tail预先指向了，但无扩容需求
        if (l_rb->Head == 0) {
            // 此时Head读完所有数据会重新指向表头
            l_rb->Tail = l_rb->Head;
        }
        return FLASE;
    }
    *rdata     = l_rb->Array[l_rb->Head];
    l_rb->Head = (l_rb->Head + 1) % l_rb->Capicity;
    l_rb->Lenght--;
    return TRUE;
}