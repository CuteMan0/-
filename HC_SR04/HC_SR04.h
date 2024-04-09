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

enum Time {
    FPS40 = 25, // 4m
    FPS20 = 50, // 7m
    FPS16 = 60
};

// PIN Define
#define ECHO_OK 		 HAL_GPIO_ReadPin(l_uc->Echo_Port, l_uc->Echo_Pin)
#define WAIT_FOR_NEXT(x) HAL_Delay(x)
// U1 Set
#define ECHO_PIN_1  GPIO_PIN_6
#define ECHO_PORT_1 GPIOA
#define TRIG_PIN_1  GPIO_PIN_11
#define TRIG_PORT_1 GPIOA

// U2 Set
#define ECHO_PIN_2  GPIO_PIN_7
#define ECHO_PORT_2 GPIOA
#define TRIG_PIN_2  GPIO_PIN_12
#define TRIG_PORT_2 GPIOA

// U3 Set
#define ECHO_PIN_3  GPIO_PIN_0
#define ECHO_PORT_3 GPIOB
#define TRIG_PIN_3  GPIO_PIN_5
#define TRIG_PORT_3 GPIOB

// U4 Set
#define ECHO_PIN_4  GPIO_PIN_1
#define ECHO_PORT_4 GPIOB
#define TRIG_PIN_4  GPIO_PIN_12
#define TRIG_PORT_4 GPIOB

// U5 Set
#define ECHO_PIN_5  GPIO_PIN_6
#define ECHO_PORT_5 GPIOB
#define TRIG_PIN_5  GPIO_PIN_13
#define TRIG_PORT_5 GPIOB

// U6 Set
#define ECHO_PIN_6  GPIO_PIN_7
#define ECHO_PORT_6 GPIOB
#define TRIG_PIN_6  GPIO_PIN_14
#define TRIG_PORT_6 GPIOB

// U7 Set
#define ECHO_PIN_7  GPIO_PIN_8
#define ECHO_PORT_7 GPIOB
#define TRIG_PIN_7  GPIO_PIN_15
#define TRIG_PORT_7 GPIOB

// U8 Set
#define ECHO_PIN_8  GPIO_PIN_9
#define ECHO_PORT_8 GPIOB
#define TRIG_PIN_8  GPIO_PIN_4
#define TRIG_PORT_8 GPIOA

// Function Declaration
void Ultrasonic_Upload(HC_SR04 *l_uc);

void Multi_Measure(HC_SR04 *l_uc);
void Measure_lenth(HC_SR04 *l_uc);

void HC_SR_Init(HC_SR04 *l_uc);
void Base_Init(HC_SR04 *l_uc);

void Delay_us(uint32_t us);

#endif