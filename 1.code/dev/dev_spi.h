#ifndef DEV_SPI_H
#define DEV_SPI_H
#include "stm32f4xx.h"
	void SPI1_Init(void);			 //��ʼ��SPI��
	void SPI1_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
	u8 SPI1_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�


class _SPI
{
	public:
	
	private:

};
extern _SPI Spi;

#endif