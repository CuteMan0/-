#ifndef __SSD1306_HW_IIC_H
#define __SSD1306_HW_IIC_H

#include "stm32f10x.h"

void HW_I2C_Write(uint8_t *buf,uint8_t len);
void IIC_HW_Init(void);

#endif
