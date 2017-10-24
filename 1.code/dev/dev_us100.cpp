#include "dev_us100.h"
/*
  ******************************************************************************
  * @file   
  * @author ytf
  * @version 
  * @date   
  * @brief  
  *    
  *****************************************************************************
*/
_US100 Us100;

/*
us100 ����1m�����ȶ�
*/

void _US100::SampleTriger(float dt)
{
	  u8 	temp = 0x55;
		USART_SendData(USART6,temp);
	  sendFLag=1;
}

void _US100::disCall(void)
{
	rxTem=USART_ReceiveData(USART6);
	float dis_Factor =0.05 / (0.05 + 1 / (2 * 3.14159 * 2));
	
	static u8 disTem;
	if(sendFLag==1)                                              //��һ�����ݵ���
	{
	 disTem=rxTem;
	 rxTem=0;
	 sendFLag=2;                                                //�ѽ��ܵ�һ������
	}
	if(sendFLag==2)
	{
	 dis=(float)((disTem<<8)+ rxTem)/10.0; //��λ������
	 rxTem=0;
	 if(dis>200) dis=lastdis;                                    //������Χ��������һ��
	
	 disFilt= dis* dis_Factor +(1-dis_Factor)*lastdis;           //��ͨ
	 lastdis =disFilt;		
	}
}