#ifndef __HR_SR04_H
#define __HR_SR04_H

#include "tim.h"
#include "usart.h"

typedef struct
{
    // uint8_t Uc_Id;
    GPIO_TypeDef *Echo_Port;
    GPIO_TypeDef *Trig_Port;
    uint32_t Echo_Pin;
    uint32_t Trig_Pin;
    uint16_t data;
} HC_SR04;

enum Ultrasonic_Id {
    U1 = 1,
    U2,
    U3,
    U4,
    U5,
    U6,
    U7,
    U8
};

// PIN Define
#define Echo_Ok HAL_GPIO_ReadPin(l_uc->Echo_Port, l_uc->Echo_Pin)
// U1 Set
#define Echo_Pin_1  GPIO_PIN_6
#define Echo_Port_1 GPIOA
#define Trig_Pin_1  GPIO_PIN_11
#define Trig_Port_1 GPIOA

// U2 Set
#define Echo_Pin_2  GPIO_PIN_7
#define Echo_Port_2 GPIOA
#define Trig_Pin_2  GPIO_PIN_12
#define Trig_Port_2 GPIOA

// U3 Set
#define Echo_Pin_3  GPIO_PIN_0
#define Echo_Port_3 GPIOB
#define Trig_Pin_3  GPIO_PIN_5
#define Trig_Port_3 GPIOB

// U4 Set
#define Echo_Pin_4  GPIO_PIN_1
#define Echo_Port_4 GPIOB
#define Trig_Pin_4  GPIO_PIN_12
#define Trig_Port_4 GPIOB

// U5 Set
#define Echo_Pin_5  GPIO_PIN_6
#define Echo_Port_5 GPIOB
#define Trig_Pin_5  GPIO_PIN_13
#define Trig_Port_5 GPIOB

// U6 Set
#define Echo_Pin_6  GPIO_PIN_7
#define Echo_Port_6 GPIOB
#define Trig_Pin_6  GPIO_PIN_14
#define Trig_Port_6 GPIOB

// U7 Set
#define Echo_Pin_7  GPIO_PIN_8
#define Echo_Port_7 GPIOB
#define Trig_Pin_7  GPIO_PIN_15
#define Trig_Port_7 GPIOB

// U8 Set
#define Echo_Pin_8  GPIO_PIN_9
#define Echo_Port_8 GPIOB
#define Trig_Pin_8  GPIO_PIN_4
#define Trig_Port_8 GPIOA

// Function Declaration
void Ultrasonic_Upload(HC_SR04 *l_uc);

void Multi_Measure(HC_SR04 *l_uc);
void Measure_lenth(HC_SR04 *l_uc);

void HC_SR_Init(HC_SR04 *l_uc);
void Base_Init(HC_SR04 *l_uc);

void delay_us(uint32_t us);

#endif