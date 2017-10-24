/*
  ******************************************************************************
  * @file   
  * @author  ytf
  * @version 
  * @date   
  * @brief  ��ʼ������
  *    
  *****************************************************************************
*/

#include "uav_init.h"
#include  "drv_w25qxx.h"
#include "uav_parameter.h"
#include "dev_moto.h"
#include "uav_control.h"
#include "uav_rc.h"
#include "uav_attitude.h"
_UAV Uav;
void _UAV::Init(void)
{
	//ʱ�ӳ�ʼ��
	Rcc.Configuration();
	//�˿ڳ�ʼ��
  Port.Configuration();
	//���ڳ�ʼ��
	Usart.Configuration();
	//�жϳ�ʼ��
	Nvic.Configuration();
 //DMA��ʼ��
	Dma.Configuration();
	//LED��ʼ��
  Led.Configuration();
	//Flash��ʼ��
	flash25q32.Init_();
	//������ʼ��
  Pamameter.Configuration();
	//���Ʋ�����ʼ��
	Control.Init();
	//λ�ÿ��Ƴ�ʼ��
	Attitude.Init();
	//�����ʼ��
	Delay_ms(100);
	Moto.MotoInit();
	Delay_ms(100);
	//��������ʼ��
	Mpu6050.Configuration(20);
	//��ѹ�Ƴ�ʼ��
	Ms5611.Configuration();
	
	
//	while(Rc.g_CH[ksr]!=1&&Rc.g_CH[ksr]!=2&&Rc.g_CH[ksr]!=3);
//	while(Rc.g_CH[ksl]!=1&&Rc.g_CH[ksl]!=2&&Rc.g_CH[ksl]!=3);
	
};