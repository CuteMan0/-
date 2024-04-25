#include "HC_SR04.h"

/**
 * @brief 响应主机
 *
 */
void Ultrasonic_ACK(void)
{
    uint8_t Ack_Byte = ACK_M;
    HAL_UART_Transmit(&huart1, &Ack_Byte, 1, 100);
}

/**
 * @brief 将测量数据上传主机
 *
 * @param l_uc 超声波的句柄
 */
void Ultrasonic_Upload(HC_SR04 *l_uc)
{
    uint8_t buffer[DEVICES + 2] = {0x00};
    buffer[0]                   = HEAD;
    buffer[DEVICES + 1]         = TAIL;
    for (uint8_t i = 1; i < (DEVICES + 1); i++) {
        l_uc++;
        buffer[i] = (uint8_t)l_uc->data;
    }
    HAL_UART_Transmit(&huart1, buffer, DEVICES + 2, 100);
}

/**
 * @brief 接收主机命令
 *
 * @return DataStatusType 状态
 */
DataStatusType Ultrasonic_Download(void)
{
    uint8_t temp = 0x00;
    HAL_UART_Receive(&huart1, &temp, 1, 100);

    if (temp == START_M)
        return DAT_OK;
    else
        return DAT_ERROR;
}

/**
 * @brief 单/多端测距
 *
 * @param l_uc 超声波的句柄
 * @return DataStatusType 状态
 */
DataStatusType Multi_Measure(HC_SR04 *l_uc)
{
    uint8_t cmd = 0x00;
    HAL_UART_Receive(&huart1, &cmd, 1, 100);
    // if (HAL_UART_Receive(&huart1, &cmd, 1, 100))
    // {
    switch (cmd) {
        case UC1:
            Measure_lenth(l_uc + (cmd & 0x0F));
            return DAT_OK;
        case UC2:
            Measure_lenth(l_uc + (cmd & 0x0F));
            return DAT_OK;
        case UC3:
            Measure_lenth(l_uc + (cmd & 0x0F));
            return DAT_OK;
        case UC4:
            Measure_lenth(l_uc + (cmd & 0x0F));
            return DAT_OK;
        case UC5:
            Measure_lenth(l_uc + (cmd & 0x0F));
            return DAT_OK;
        case UC6:
            Measure_lenth(l_uc + (cmd & 0x0F));
            return DAT_OK;
        case UC_ALL:
            for (uint8_t i = 1; i < (DEVICES + 1); i++) {
                Measure_lenth(l_uc + i);
            }
            return DAT_OK;
        default:
            return DAT_ERROR;
    }
    // }
}

/**
 * @brief 测距
 *
 * @param l_uc 数据包指针
 */
void Measure_lenth(HC_SR04 *l_uc)
{
    uint16_t startTime = 0, endTime = 0;

#ifdef __time_cnt__
    // start measure
    HAL_GPIO_WritePin(l_uc->Trig_Port, l_uc->Trig_Pin, 1);
    Delay_us(10);
    HAL_GPIO_WritePin(l_uc->Trig_Port, l_uc->Trig_Pin, 0);

    // start count
    HAL_TIM_Base_Start(&htim1);

    while (!ECHO_OK)
        ;
    startTime = __HAL_TIM_GET_COUNTER(&htim1);
    // end count
    while (ECHO_OK)
        ;
    endTime = __HAL_TIM_GET_COUNTER(&htim1);

    // end count
    HAL_TIM_Base_Stop(&htim1);

    // time of back
    uint16_t backTime = endTime - startTime;

    // mm of distance
    float distance = backTime * 0.017;
    
#endif // __time_cnt__

#ifdef __inp_capture__

#endif // __inp_capture__

    // 测量值存入data
    l_uc->data = (uint16_t)distance;
    // return l_uc->data;
}

/**
 * @brief HC_SR-04 TRIG and ECHO pin to gpio
 * @param l_uc 超声波的句柄
 */
void HC_SR_Init(HC_SR04 *l_uc)
{
    /* GPIO PORTs Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // 对第一个超声波进行初始化
    l_uc++;
    l_uc->Echo_Pin  = ECHO_PIN_1;
    l_uc->Echo_Port = ECHO_PORT_1;
    l_uc->Trig_Pin  = TRIG_PIN_1;
    l_uc->Trig_Port = TRIG_PORT_1;
    Base_Init(l_uc);

    // 2
    l_uc++;
    l_uc->Echo_Pin  = ECHO_PIN_2;
    l_uc->Echo_Port = ECHO_PORT_2;
    l_uc->Trig_Pin  = TRIG_PIN_2;
    l_uc->Trig_Port = TRIG_PORT_2;
    Base_Init(l_uc);

    // 3
    l_uc++;
    l_uc->Echo_Pin  = ECHO_PIN_3;
    l_uc->Echo_Port = ECHO_PORT_3;
    l_uc->Trig_Pin  = TRIG_PIN_3;
    l_uc->Trig_Port = TRIG_PORT_3;
    Base_Init(l_uc);

    // 4
    l_uc++;
    l_uc->Echo_Pin  = ECHO_PIN_4;
    l_uc->Echo_Port = ECHO_PORT_4;
    l_uc->Trig_Pin  = TRIG_PIN_4;
    l_uc->Trig_Port = TRIG_PORT_4;
    Base_Init(l_uc);

    // 5
    l_uc++;
    l_uc->Echo_Pin  = ECHO_PIN_5;
    l_uc->Echo_Port = ECHO_PORT_5;
    l_uc->Trig_Pin  = TRIG_PIN_5;
    l_uc->Trig_Port = TRIG_PORT_5;
    Base_Init(l_uc);

    // 6
    l_uc++;
    l_uc->Echo_Pin  = ECHO_PIN_6;
    l_uc->Echo_Port = ECHO_PORT_6;
    l_uc->Trig_Pin  = TRIG_PIN_6;
    l_uc->Trig_Port = TRIG_PORT_6;
    Base_Init(l_uc);

    // 7
    l_uc++;
    l_uc->Echo_Pin  = ECHO_PIN_7;
    l_uc->Echo_Port = ECHO_PORT_7;
    l_uc->Trig_Pin  = TRIG_PIN_7;
    l_uc->Trig_Port = TRIG_PORT_7;
    Base_Init(l_uc);

    // 8
    l_uc++;
    l_uc->Echo_Pin  = ECHO_PIN_8;
    l_uc->Echo_Port = ECHO_PORT_8;
    l_uc->Trig_Pin  = TRIG_PIN_8;
    l_uc->Trig_Port = TRIG_PORT_8;
    Base_Init(l_uc);
}

/**
 * @brief 对超声波引脚进行初始化
 * @param l_uc 数据包指针
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
 * @brief 微秒级延时
 *
 * @param us
 */
void Delay_us(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--) {
        ;
    }
}