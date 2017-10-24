#include "dev_ak8975.h"
#include "uav_parameter.h"
#include "uav_filter.h"
/*
//������
                y
								|
                |
x<______________

*/
_AK8975 Ak8975;
//��������
void _AK8975::SampleTriger(void)
{
  SingleWrite(AK8975_ADDRESS,AK8975_CNTL,0x01);
}
//����
void _AK8975::DataSample(void)
{
 
	Vector3i Tmag;
	SingleRead(AK8975_ADDRESS,AK8975_HXL,&buffer[0]); 
	SingleRead(AK8975_ADDRESS,AK8975_HXH,&buffer[1]);
	Tmag.y= ((((int16_t)buffer[1]) << 8) | buffer[0]) ;  //����X

	SingleRead(AK8975_ADDRESS,AK8975_HYL,&buffer[2]);
	SingleRead(AK8975_ADDRESS,AK8975_HYH,&buffer[3]);
	Tmag.x = ((((int16_t)buffer[3]) << 8) | buffer[2]) ;  //����Y

	SingleRead(AK8975_ADDRESS,AK8975_HZL,&buffer[4]);
	SingleRead(AK8975_ADDRESS,AK8975_HZH,&buffer[5]);
	Tmag.z = -((((int16_t)buffer[5]) << 8) | buffer[4]) ;  //����Z	
	
	magTem.x=(float)Tmag.x;  
	magTem.y=(float)Tmag.y;
	magTem.z=(float)Tmag.z;
	//������
  mag=magTem-magOffset;
	mag=magTem;

	
 // printf("mag %f  %f  %f \n",magTem.x,magTem.y,magTem.z);
 //���ݵ�ͨ
 //���̵�ͨ�˲�ϵ��
 
 	float Ak8975factor=0.02 / (0.02 + 1 / (2 * M_PI * 0.5));  //����ʱ����20����5H��ͨ
 	mag=mag*Ak8975factor+lastmag*(1-Ak8975factor);
	lastmag=mag;
//	
//	mag.x=filter.MeanValue(magTem.x);
//	mag.y=filter.MeanValue(magTem.y);
//	mag.z=filter.MeanValue(magTem.z);
	MagCalOffset();
	update=1;
	SampleTriger();//�ٴβ�������
}

void _AK8975:: MagCalOffset(void)
{

  Vector3f  magMAX(-1000,-1000,-1000), minMAX(1000,1000,1000);//���⽫��ֵ�����ֵ
	static u16  cnt_m=0;
	
	if(g_CallSetMagOffset==1)
	{
		static Vector3f temMag(0,0,0);
		//��Χ�ų�����
	 if(fabs(mag.x)<400&&fabs(mag.x)<400&&fabs(mag.x)<400)
	 {
		 //Ѱ�����
		 if((magTem.x-magMAX.x)>0.1)  magMAX.x=magTem.x;
		 if((magTem.y-magMAX.y)>0.1)  magMAX.x=magTem.y;
		 if((magTem.z-magMAX.z)>0.1)  magMAX.x=magTem.z;
		 
		 //Ѱ����С
		 if((magTem.x-minMAX.x)<0.1)  minMAX.x=magTem.x;
		 if((magTem.y-minMAX.y)<0.1)  minMAX.x=magTem.y;
		 if((magTem.z-minMAX.z)<0.1)  minMAX.x=magTem.z;
	 }
	 cnt_m++;
	 if(cnt_m==2000)//������Լ���������ת�����жϽ����������Ƿ�ÿ����ѡװ360����������ǰ
	 {
		//��Բ����
		temMag=(magMAX+minMAX)*0.5f;
		 //������Ʈ����
		Pamameter.SaveMagOfferset(temMag);
		//Delay_ms(2);
		 //������Ʈ����
		Pamameter.ReadMagOfferset(); 
		 //����������ݱ�֤�´ν�����������
	  magMAX(-1000,-1000,-1000), minMAX(1000,1000,1000),temMag(0,0,0);
	  cnt_m=0;
		g_CallSetMagOffset=0; 
	 }
	
	
	}
	
	
}




