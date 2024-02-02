#include "dht20.h"
	
void dht20_run(float *pdht20)
{
	uint8_t pt_DAT[] = {0xAC,0x33,0x00};
	uint8_t pr_DAT[6] = {0x00};
	static char init = 1;
	
	if(init)
	{
		HAL_Delay(100);//上电100ms等待
		init -= 1;
	}
	HAL_I2C_Master_Receive(&hi2c1,0x71,pr_DAT,1,100);//读状态字
		
	if( (pr_DAT[0] & 0x10) && (pr_DAT[0] &0x08) )
	{
		HAL_Delay(10);
		HAL_I2C_Master_Transmit(&hi2c1,0x71,pt_DAT,3,100);//触发测量
	}
	else
	{
		//初始化1B\1C\1E寄存器
	}
	HAL_Delay(80);//等待转换

	HAL_I2C_Master_Receive(&hi2c1,0x71,pr_DAT,7,100);//接受数据
	while( pr_DAT[0]&0x80 )
	{
		;//正忙
	}
	
	pdht20[0] = (((pr_DAT[1]<<12) + (pr_DAT[2]<<4) + (pr_DAT[3]>>4))*100.0) / 0xfffff;
	pdht20[1] = ((((pr_DAT[3]<<28)>>12) + (pr_DAT[4]<<8) + (pr_DAT[5]))*200.0) / 0xfffff - 50;
}


