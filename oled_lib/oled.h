#ifndef __OLED_H
#define __OLED_H

#include "i2c_sw.h"

#define ID_LENGTH   4
#define DATA_LENGTH 4

#define SALVE_ADDRESS 0x78 //0x78 0x7A

void OLED_Init(void);
void OLED_Clear(void);
void OLED_Fill(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBMP(const uint8_t *bmp);

#endif
