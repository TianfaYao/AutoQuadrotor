#include "dev_rcc.h"

/*
  ******************************************************************************
  * @file   
  * @author  ytf
  * @version 
  * @date   
  * @brief  �ɿ�ʱ�ӳ�ʼ��
  *    
  *****************************************************************************
*/
 _RCC   Rcc;
void _RCC::Configuration(void)
{

	//LED
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	//����2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //����USART2ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);	
	//����3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //����USART3ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	//����6
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);   //����USART6ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	//I2C
	RCC_AHB1PeriphClockCmd(DEV_RCC_I2C , ENABLE );
	//MOTO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  //FLASH
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	//debus
	 RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA| RCC_AHB1Periph_DMA2,ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	
}
