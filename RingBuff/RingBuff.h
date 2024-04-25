#include "stm32h7xx_hal.h"

#define RINGBUFF_LEN (10) // 定义最大接收字节数

typedef struct
{
    uint16_t Head;
    uint16_t Tail;
    uint16_t Lenght;
    uint8_t Ring_data[RINGBUFF_LEN];
} RingBuff_t;

typedef enum {
    RINGBUFF_ERR,
    RINGBUFF_OK
} RingBuff_Status;

void RingBuff_Init(RingBuff_t *l_ringBuff);
RingBuff_Status Write_RingBuff_Multi(RingBuff_t *l_ringBuff, uint8_t *data, uint8_t size);
RingBuff_Status Read_RingBuff_Multi(RingBuff_t *l_ringBuff, uint8_t *rData, uint8_t size);
RingBuff_Status Write_RingBuff(RingBuff_t *l_ringBuff, uint8_t data);
RingBuff_Status Read_RingBuff(RingBuff_t *l_ringBuff, uint8_t *rData);