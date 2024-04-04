#ifndef __SSD1306_HW_IIC_H
#define __SSD1306_HW_IIC_H

#include "stm32f10x.h"

void I2C_HW_Write(uint8_t *buf,uint8_t len);
void I2C_HW_Init(void);

#endif
