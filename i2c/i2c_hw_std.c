#include "i2c_hw.h"

// SCL PB6
// SDA PB7
#define SSD1306_ADDRESS 0x78

void I2C_HW_Write(uint8_t *buf,uint8_t len)
{
	if(len<=0)
		return ;

	/* wait for the busy falg to be reset */
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) );

	/* start transfer */
	I2C_GenerateSTART(I2C1,ENABLE);
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	

	I2C_Send7bitAddress(I2C1,SSD1306_ADDRESS,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	
	for(uint8_t i=0;i<len;i++)
	{
		if(i < (len - 1) )
		{
			I2C_SendData(I2C1, buf[i]);
			while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING)==ERROR);
		}
		if(i ==(len - 1) )
		{
			I2C_SendData(I2C1, buf[i]);
			while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
		}
	}
	
	I2C_GenerateSTOP(I2C1,ENABLE);
}







void I2C_HW_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);		//开启I2C2的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);					
	
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_ClockSpeed=400000;
	I2C_InitStruct.I2C_Mode=I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1,&I2C_InitStruct);
	
	I2C_Cmd(I2C1,ENABLE);
	
	
}

