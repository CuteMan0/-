#include "dht20.h"
	
void dht20_run(float *pdht20)
{
	uint8_t pt_DAT[] = {0xAC,0x33,0x00};
	uint8_t pr_DAT[6] = {0x00};
	static char init = 1;
	
	if(init)
	{
		HAL_Delay(100);//�ϵ�100ms�ȴ�
		init -= 1;
	}
	HAL_I2C_Master_Receive(&hi2c1,0x71,pr_DAT,1,100);//��״̬��
		
	if( (pr_DAT[0] & 0x10) && (pr_DAT[0] &0x08) )
	{
		HAL_Delay(10);
		HAL_I2C_Master_Transmit(&hi2c1,0x71,pt_DAT,3,100);//��������
	}
	else
	{
		//��ʼ��1B\1C\1E�Ĵ���
	}
	HAL_Delay(80);//�ȴ�ת��

	HAL_I2C_Master_Receive(&hi2c1,0x71,pr_DAT,7,100);//��������
	while( pr_DAT[0]&0x80 )
	{
		;//��æ
	}
	
	pdht20[0] = (((pr_DAT[1]<<12) + (pr_DAT[2]<<4) + (pr_DAT[3]>>4))*100.0) / 0xfffff;
	pdht20[1] = ((((pr_DAT[3]<<28)>>12) + (pr_DAT[4]<<8) + (pr_DAT[5]))*200.0) / 0xfffff - 50;
}


