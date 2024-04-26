#ifndef __RINGBUFF_SL_H
#define __RINGBUFF_SL_H

#include <stdio.h>
#include <stdlib.h>

#define RINGBUFF_LEN (4)

typedef uint8_t RBDat_t;

typedef struct RingBuff_Struct
{
    uint16_t Head;
    uint16_t Tail;
    uint16_t Lenght;
    uint16_t Capicity;
    RBDat_t *Array;
} RB_t;

typedef enum RingBuff_Status{
    FLASE,
    TRUE
} RB_Status;

void RingBuff_Init(RB_t *l_rb);
// RB_Status Write_RingBuff_Multi(RB_t *l_rb, RBDat_t *data, uint8_t size);
// RB_Status Read_RingBuff_Multi(RB_t *l_rb, RBDat_t *rdata, uint8_t size);
RB_Status Write_RingBuff(RB_t *l_rb, RBDat_t data);
RB_Status Read_RingBuff(RB_t *l_rb, RBDat_t *rdata);


#endif