#include "dev_debus.h"
 /*
  ******************************************************************************
  * @file   
  * @author  ytf
  * @version 
  * @date   
  * @brief  
  *    
  *****************************************************************************
*/
 
 _DEBUS Debus;
 
void _DEBUS::RemotCall(void)
{
	//�������״̬��麯��
remot[ch2] = (Dma.buffer[0]| (Dma.buffer[1] << 8)) & 0x07ff;//�ұ�����
remot[ch3] = ((Dma.buffer[1] >> 3) | (Dma.buffer[2] << 5)) & 0x07ff;//�ұ�����
remot[ch0] = ((Dma.buffer[2] >> 6) | (Dma.buffer[3] << 2) | (Dma.buffer[4] << 10)) & 0x07ff;//�������
remot[ch1] = ((Dma.buffer[4] >> 1) | (Dma.buffer[5] << 7)) & 0x07ff;//�������
remot[ch5] = ((Dma.buffer[5] >> 4)& 0x000C) >> 2;//radio_switch_left
remot[ch4] = ((Dma.buffer[5] >> 4)& 0x0003);//radio_switch_right
}
