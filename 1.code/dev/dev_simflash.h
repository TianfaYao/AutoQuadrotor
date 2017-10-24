#ifndef DEV_SIMFLASH_H
#define DEV_SIMFLASH_H


#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
#define STM32_FLASH_SIZE 128	 		      //��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_BASE 0x08000000 	  //STM32 FLASH����ʼ��ַ
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024            //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif

#define FLASH_SAVE_ADDR  0X08010000 	//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)


#include "stm32f4xx.h"

enum 
{
  gro,
	acc,
};
#define  SENSORNUM           2  										//��������Ŀ
#define  FASHCACHE_SIZE     sizeof(Vector3i)*SENSORNUM     //�����Ǻͼ��ٶȼƵ�


class _FLSHSIMROM
{
	public:
		void flashInit(void);
	  void  sensorOffsetDataWrite(void);
	  void  sensorOffsetDataRead(void);

		void Test_Write(u32 WriteAddr,u16 WriteData);		
    void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
    void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����	
	private:
		u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];                              //�����2K�ֽ�
		u16 STMFLASH_ReadHalfWord(u32 faddr);		                          //��������  
  	void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);  

};

extern _FLSHSIMROM  flashsimrom;

#endif