#include "dev_port.h"

/*
  ******************************************************************************
  * @file    
  * @author  Ҧ�췢
  * @version 
  * @date    
  * @brief   �ɿض˿ڳ�ʼ��
  *    
  *****************************************************************************
*/
 _PORT  port;
void _PORT::Configuration(void)
{
//����2
  GPIO_InitTypeDef  GPIO_InitStructure;	
	//LED
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1\
	                               |GPIO_Pin_2| GPIO_Pin_3 ; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
//����PD5��ΪUSART2��Tx
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_5; 
  GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP ;
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
//����PD6��ΪUSART2��Rx
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_6 ; 
  GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType  = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL ;
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
//PD8 GPS  
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_8; 
  GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP ;
	//GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
//PD9 GPS
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_9; 
  GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType  = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL ;
	//GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
//����PC6 ��Ϊ���������ݽ���RX
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_6; 
  GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP ;
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
//����PC7 ��Ϊ���������ݽ���TX
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_7 ; 
  GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType  = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL ;
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
	 
//I2C
  GPIO_InitStructure.GPIO_Pin =  I2C_Pin_SCL | I2C_Pin_SDA; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(DEV_GPIO_I2C, &GPIO_InitStructure);		

//MOTO
	GPIO_InitStructure.GPIO_Pin    = GPIO_Pin_9 | GPIO_Pin_11 \
	                                 | GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode   = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP ;
  GPIO_Init(GPIOE, &GPIO_InitStructure); 

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1); 
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
//debus
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource10 ,GPIO_AF_USART1);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
	//
}
