#include "dev_led.h"
 _LED Led;
//led��ʼ��
void _LED::Configuration(void)
{
	//0 1 2 3                                            //23�졢13�� 12 �� 02 ��  3��
	 GPIO_SetBits(GPIOE,GPIO_Pin_2);
	 GPIO_SetBits(GPIOE,GPIO_Pin_3);
	 Delay_ms(20);
}