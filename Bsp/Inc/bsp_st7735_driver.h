#include "stdint.h"
#include "stdbool.h"

void ST7735_Init(void);
void ST7735_SetRotation(uint8_t rotation);
void st7735_write_memory(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t *color_buffer);