#include "HC_SR04.h"

/**
 * @brief 将测量数据上传主机
 *
 * @param l_uc 传入的是第一个超声波的地址
 */
void Ultrasonic_Upload(HC_SR04 *l_uc)
{
    uint16_t buffer[8] = {0x00};
    for (uint8_t i = 0; i < 8; i++) {
        buffer[i] = l_uc->data;
        l_uc++;
    }

    HAL_UART_Transmit(&huart1, buffer, 16, 100);
}

/**
 * @brief 批量测距,建议连续测量间隔在60ms以上
 *
 * @param l_uc 传入的是第一个超声波的地址
 */
void Multi_Measure(HC_SR04 *l_uc)
{
    for (uint8_t i = 0; i < 8; i++) {
        Measure_lenth(l_uc++);
    }
    HAL_Delay(60);
}

/**
 * @brief 测距
 *
 * @param l_uc 传入实体超声波的指针
 */
void Measure_lenth(HC_SR04 *l_uc)
{
    uint16_t startTime = 0, endTime = 0;

    // start measure
    HAL_GPIO_WritePin(l_uc->Trig_Port, l_uc->Trig_Pin, 1);
    delay_us(10);
    HAL_GPIO_WritePin(l_uc->Trig_Port, l_uc->Trig_Pin, 0);

    // start count
    HAL_TIM_Base_Start(&htim1);

    while (!Echo_Ok)
        ;
    startTime = __HAL_TIM_GET_COUNTER(&htim1);
    // end count
    while (Echo_Ok)
        ;
    endTime = __HAL_TIM_GET_COUNTER(&htim1);

    // end count
    HAL_TIM_Base_Stop(&htim1);

    // time of back
    uint16_t backTime = endTime - startTime;
    // mm of distance
    float distance = backTime * 0.17;

    // 测量值存入data
    l_uc->data = (uint16_t)distance;
    // return l_uc->data;
}

/**
 * @brief HC_SR-04 Trig and Echo pin to gpio
 * @param l_uc 传入的是第一个超声波的地址
 */
void HC_SR_Init(HC_SR04 *l_uc)
{
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // 对第一个超声波进行初始化
    l_uc->Echo_Pin  = Echo_Pin_1;
    l_uc->Echo_Port = Echo_Port_1;
    l_uc->Trig_Pin  = Trig_Pin_1;
    l_uc->Trig_Port = Trig_Port_1;
    Base_Init(l_uc);

    // 更新地址，初始化第二个超声波
    l_uc++;
    l_uc->Echo_Pin  = Echo_Pin_2;
    l_uc->Echo_Port = Echo_Port_2;
    l_uc->Trig_Pin  = Trig_Pin_2;
    l_uc->Trig_Port = Trig_Port_2;
    Base_Init(l_uc);

    // 3
    l_uc++;
    l_uc->Echo_Pin  = Echo_Pin_3;
    l_uc->Echo_Port = Echo_Port_3;
    l_uc->Trig_Pin  = Trig_Pin_3;
    l_uc->Trig_Port = Trig_Port_3;
    Base_Init(l_uc);

    // 4
    l_uc++;
    l_uc->Echo_Pin  = Echo_Pin_4;
    l_uc->Echo_Port = Echo_Port_4;
    l_uc->Trig_Pin  = Trig_Pin_4;
    l_uc->Trig_Port = Trig_Port_4;
    Base_Init(l_uc);

    // 5
    l_uc++;
    l_uc->Echo_Pin  = Echo_Pin_5;
    l_uc->Echo_Port = Echo_Port_5;
    l_uc->Trig_Pin  = Trig_Pin_5;
    l_uc->Trig_Port = Trig_Port_5;
    Base_Init(l_uc);

    // 6
    l_uc++;
    l_uc->Echo_Pin  = Echo_Pin_6;
    l_uc->Echo_Port = Echo_Port_6;
    l_uc->Trig_Pin  = Trig_Pin_6;
    l_uc->Trig_Port = Trig_Port_6;
    Base_Init(l_uc);

    // 7
    l_uc++;
    l_uc->Echo_Pin  = Echo_Pin_7;
    l_uc->Echo_Port = Echo_Port_7;
    l_uc->Trig_Pin  = Trig_Pin_7;
    l_uc->Trig_Port = Trig_Port_7;
    Base_Init(l_uc);

    // 8
    l_uc++;
    l_uc->Echo_Pin  = Echo_Pin_8;
    l_uc->Echo_Port = Echo_Port_8;
    l_uc->Trig_Pin  = Trig_Pin_8;
    l_uc->Trig_Port = Trig_Port_8;
    Base_Init(l_uc);
}

/**
 * @brief 对超声波引脚进行初始化
 * @param l_uc 传入实体超声波的指针
 */
void Base_Init(HC_SR04 *l_uc)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*Configure trig pin Output Level */
    HAL_GPIO_WritePin(l_uc->Trig_Port, l_uc->Trig_Pin, 0);

    /*Configure GPIO pin : trig */
    GPIO_InitStruct.Pin   = l_uc->Trig_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(l_uc->Trig_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : echo */
    GPIO_InitStruct.Pin  = l_uc->Echo_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(l_uc->Echo_Port, &GPIO_InitStruct);
}

/**
 * @brief about us delay at 72MHz
 *
 * @param us
 */
void delay_us(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--) {
        ;
    }
}