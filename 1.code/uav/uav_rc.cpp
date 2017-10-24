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

#include "uav_rc.h"
#include "dev_debus.h"
#include "math.h"
#include "imu.h"
_RC Rc;
void _RC::DataMange(float dt )
{
 //ҡ�������ݸ�������
 g_CH[yaw]=(Debus.remot[ch0]-1024)* 0.7575f; //+-500
 g_CH[thr]=(Debus.remot[ch1]-1024)* 0.7575f;   
 g_CH[rol]=(Debus.remot[ch2]-1024)* 0.7575f;
 g_CH[pit]=(Debus.remot[ch3]-1024)* 0.7575f;
 g_CH[ksr]=(Debus.remot[ch4]);
 g_CH[ksl]=(Debus.remot[ch5]);
	
//���Ŷ���
 Thr();
//HoveringFlight();
//����
 Deblock(dt);
	//ģʽ
 Mode();
}

void _RC::Deblock(float dt)
{
 static u16 unlockcnt=0;
		if(Debus.remot[ch1]==364)  	//�ڰ˽���
		{
			if(Debus.remot[ch0]==1684)
				{
						if(Debus.remot[ch2]==364)
							{
							 if(Debus.remot[ch3]==364)
								 {
									 //����
								 qudrotorState=unlock;

							   }
						  }
			  }
		}
//����
		if( qudrotorState==lock)
		{
			 if(Debus.remot[ch1]==364)
				{
					if(abs(Debus.remot[ch0]-1024)<=3)
					unlockcnt+=1000 *dt;
				}		
				if(unlockcnt>=1000)//1S
				{
					qudrotorState = lock;
					unlockcnt=0;
				}
		}
}


//���ſ���
void _RC::Thr(void)
{
	
  g_ThrVAL=( g_CH[thr]+500)*0.8; //0-800
  g_Weight[thr]=g_ThrVAL/800.0;
	
	g_ThrVAL=1.15*g_ThrVAL;
	
 VAL_LIMIT(g_ThrVAL,0,800);
	
	if(Debus.remot[ch1]>400)
	{
	  g_Thr_Low=0;
	}else
	{
	 g_Thr_Low=1;
	}
//������Ӷ��߿���
}
//ģʽ����
void _RC::Mode(void)
{
 if(qudrotorState==unlock)
 {
 		if(g_CH[ksl]==1 )
		{
			g_MODE= 0; //���
		}
		else if(  g_CH[ksl]==3 )
		{
			g_MODE = 1; //��ѹ
		}
		else
		{

		}
	}
	else
	{
	g_MODE = 0;
		
	}
}
//��̬�������� �ѿ���ͣ
void _RC::HoveringFlight(void)
{
	 float x_kd=0.125 ;
	 float y_kd=0.125;
	 float z_kd;
	
	//���ٶ�ֱ�ӷ�������������Ʒɻ��Ĵ��ε�
	if(abs(g_CH[rol])<30)
	{
	//rol�Ĵ�С�ı�ɻ���y�����
		if(imu.Eacc.y>0)			
			g_CH[rol]=30+y_kd*imu.Eacc.y;
		
	 if(imu.Eacc.y<=0)
			g_CH[rol]=-30+y_kd*imu.Eacc.y;
		
	}

	if(abs(g_CH[pit])<30)
	{
		//pit�Ĵ�С�ı�ɻ���x�����	
	 if(imu.Eacc.x>0)	
	 g_CH[pit]=30+x_kd*imu.Eacc.x;
	 
	 if(imu.Eacc.x<=0)	
	 g_CH[pit]=30+x_kd*imu.Eacc.x;
	
	}
	
}

