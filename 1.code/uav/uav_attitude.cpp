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

#include "uav_attitude.h"
#include "cal_math.h"
#include "dev_mpu6050.h"
#include "dev_us100.h"
#include "uav_config.h"
#include "uav_rc.h"
#include "uav_pid.h"
#include "cal_math.h"
#include "imu.h"

_PID  z_dis[2]; 
_PID  z_spe[2];
#define   MAX_Z_DIS      10.0       //dM 
#define   MAX_Z_SPE      2        // 1ds/S
//�������в����ɵ�
#define SPE_UL_KP            1
#define SPE_UL_KI           0.1
//������
#define DIS_UL_KP            1
#define DIS_UL_KI           0.1
//��ѹ��
#define SPE_MS_KP            1
#define SPE_MS_KI           0.1

#define ACC_MS_KP            1
#define ACC_MS_KI           0.1

#define DIS_MS_KP            1
#define DIS_MS_KI           0.1

_ATTITUDE Attitude; 
void _ATTITUDE::Init(void)
{
  z_dis[ul].Init(z_dis[ul].PID.Kp,z_dis[ul].PID.Ki,z_dis[ul].PID.Kd);
  z_dis[ms].Init(z_dis[ms].PID.Kp,z_dis[ms].PID.Ki,z_dis[ms].PID.Kd);
	z_spe[ul].Init(z_spe[ul].PID.Kp,z_spe[ul].PID.Ki,z_spe[ul].PID.Kd);
  z_spe[ms].Init(z_spe[ms].PID.Kp,z_spe[ms].PID.Ki,z_spe[ms].PID.Kd);
	
}

void _ATTITUDE::GetDIS_SPE(float dt)
{
 		//���ٶȵõ��ļ��ٶȶ�ʱ�кܴ�Ĳο���ֵˢ��Ƶ�ʽϿ�           //Ԥ��
		//�������õ���λ���кܴ������                                 //�۲�
    //��ѹ�ƹ۲�߶� ׼ȷ����ˢ��Ƶ�ʽ���                          //�۲�
	
  float SPE_Z_ul_dFactor =dt / (dt + 1 / (2 * M_PI * 20));
	float SPE_Z_ms_dFactor =dt / (dt + 1 / (2 * M_PI * 2));
	//��ѹ��
  static 	float body_ms_DIS_z=0;
	static 	float lastbody_ms_DIS_z=0;
	static 	float lastbody_ms_DIS_z_fc=0;
	        float speErr_ms;
	static  float speErr_ms_I=0;
  static  float lastbody_ms_SPE_z=0;
		      float disErr_ms;
	static  float disErr_ms_I=0;
//������ٶ�
          float acccal_ms;//������ٶ�
	        float accErr_ms;
	static  float accErr_ms_I=0;
  static  float lastbody_ms_ACC_z=0;
  static 	float lastbody_ms_SPE_fc=0;

	
	//������
	static float body_ul_DIS_z=0;
	static float lastbody_ul_DIS_z=0;
	
	static float lastbody_ul_SPE_z=0;
	       float speErr_ul;
	static float speErr_ul_I=0;
	       float disErr_ul;
	static float disErr_ul_I=0;
	

	//������
  body_ul_DIS_z=Us100.disFilt/10.0;                                      //M
	body_ul_SPE_z=(body_ul_DIS_z-lastbody_ul_DIS_z)/(dt*10.0);             // M/s
  body_ul_SPE_z=body_ul_SPE_z*SPE_Z_ul_dFactor+(1-SPE_Z_ul_dFactor)*lastbody_ul_SPE_z;
	lastbody_ul_SPE_z=body_ul_SPE_z;
	lastbody_ul_DIS_z=body_ul_DIS_z;
	//��
	speErr_ul=body_ul_SPE_z-body_ul_SPE_z_fc;
	speErr_ul_I+=SPE_UL_KI*speErr_ul;
	body_ul_SPE_z_fc=body_ul_SPE_z_fc+SPE_UL_KP*speErr_ul+speErr_ul_I;  //�����������
	body_ul_SPE_z_fc=body_ul_SPE_z_fc+imu.Eacc.z*dt;                    //�Ӽ�ֱ�ӻ���
	//�����ɵ� ʱ�������ǰ�ƶ��Ͼ����ټƿ쳬���ܶ౶
	disErr_ul=body_ul_DIS_z-body_ul_DIS_z_fc;
  disErr_ul_I+=DIS_UL_KI*disErr_ul_I;
	body_ul_DIS_z_fc=body_ul_DIS_z_fc+DIS_UL_KP*disErr_ul+disErr_ul_I;
	body_ul_DIS_z_fc=body_ul_DIS_z_fc+body_ul_SPE_z_fc*dt+imu.Eacc.z*dt*dt*0.5;
	
	
	//��ѹ�� ���������кܴ�ĵ���                 �����ں��໥����
	body_ms_DIS_z=Ms5611.BaroAlt/10.0;         //��ѹ����һ��Ҫ��ͨ
	body_ms_SPE_z=(body_ms_DIS_z-lastbody_ms_DIS_z)/(dt*10.0);   //dm/s
	body_ms_SPE_z=body_ms_SPE_z*SPE_Z_ms_dFactor+(1-SPE_Z_ms_dFactor)*lastbody_ms_SPE_z;
  lastbody_ms_DIS_z=body_ms_DIS_z;
	lastbody_ms_SPE_z=body_ms_SPE_z;
	
	//�ٶ�
	speErr_ms=body_ms_SPE_z-body_ms_SPE_z_fc;
	speErr_ms_I+=SPE_MS_KI*speErr_ms_I;
	body_ms_SPE_z_fc=body_ms_SPE_z_fc+SPE_MS_KP*speErr_ms+speErr_ms_I;//�����������
	body_ms_SPE_z_fc=body_ms_SPE_z_fc+imu.Eacc.z*dt;  
	//body_ms_SPE_z_fc=body_ms_SPE_z_fc+(imu.Eacc.z+accErr_ms)*dt;  
//���ٶ�
	acccal_ms=(body_ms_SPE_z_fc-lastbody_ms_SPE_fc);   // m/s
	accErr_ms=acccal_ms-imu.Eacc.z;                       //����Ӧ����0����������ѹ�Ƶ���������ᷢ���ñ�
//	accErr_ms_I=accErr_ms_I+ ACC_MS_KI*accErr_ms*dt;      //�������ٻ��ֵõ�����ٶȷ����������ٶ���ȥ
	lastbody_ms_SPE_fc =body_ms_SPE_z_fc;                 
	//�߶�
	disErr_ms=body_ms_DIS_z-body_ms_DIS_z_fc;
	disErr_ms_I+=DIS_MS_KI*disErr_ms;
	body_ms_DIS_z_fc=body_ms_DIS_z_fc+DIS_MS_KP*disErr_ms+disErr_ms_I;
	body_ms_DIS_z_fc=body_ms_DIS_z_fc+body_ms_SPE_z_fc*dt+imu.Eacc.z*dt*dt*0.5;
	
if(fabs(body_ms_DIS_z_fc-lastbody_ms_DIS_z_fc)<2)//2dm
{
 ms_flag++;
	if(ms_flag>=200) //1s
	{
		ms_flag=200;  //��ȫ
//	GPIO_ResetBits(GPIOE,GPIO_Pin_2);
	GPIO_ResetBits(GPIOE,GPIO_Pin_3); 
	}
}
else
{
	
 ms_flag=0;
}
	
}
//λ�û�
void _ATTITUDE::DISControl(float dt)
{
  static u16 count=0;
	static u8  ms_flag=0;
	if(Rc.g_CH[ksl]==dis_ul_z) //����ģʽ
	{
		//λ��pid
	 z_dis[ul].PID.expect=MAX_Z_DIS;
	 z_dis[ul].GetErr(body_ul_DIS_z_fc);
//	 VAL_LIMIT(z_dis[ul].PID.err,-MAX_Z_DIS/4,MAX_Z_SPE/4);
//	 z_dis.Err_d(dt);
//	 VAL_LIMIT(z_dis.PID.ec,-MAX_Z_SPE,MAX_Z_SPE);
	 z_dis[ul].Err_i(1);
	 VAL_LIMIT(z_dis[ul].PID.err_i,-MAX_Z_DIS/4.0,MAX_Z_DIS/4.0);
	 z_dis[ul].Cal();
	}  else{z_dis[ul].PID.output=0;}
	
	
	if(Rc.g_CH[ksl]==dis_ms_z) //��ѹ����
	{
//λ��pid
	 if(fabs(Rc.g_CH[thr])<30)	 
	 {
		 count++;
		 if(count>=200)
		 {
			 if(ms_flag==0)
			 z_dis[ms].PID.expect=body_ul_DIS_z_fc;
			 ms_flag=1;
			
		 }
	 }
	 else
	 {
	 ms_flag=0; //Ϊ�´ζ�����׼��
	 z_dis[ms].PID.expect+=MAX_Z_SPE*(Rc.g_Weight[thr]-0.5)*dt;
	 }
	 z_dis[ms].GetErr(body_ul_DIS_z_fc);
	 z_dis[ms].Err_i(1);
	 VAL_LIMIT(z_dis[ms].PID.err_i,-MAX_Z_DIS/4.0,MAX_Z_DIS/4.0);
	 z_dis[ms].Cal();
	} else{
	z_dis[ms].PID.output=0;
	
	}
}

//�ٶȻ�
void _ATTITUDE::SPEControl(float dt)
{ 
	
//��������
if(Rc.g_CH[ksl]==dis_ul_z)
{
	z_spe[ul].PID.expect=z_dis[ul].PID.output/100;
//	z_spe[ul].PID.expect=MAX_Z_SPE*(Rc.g_Weight[thr]-0.5)*2;
//	
//	VAL_LIMIT(z_spe[ul].PID.expect,-MAX_Z_SPE,MAX_Z_SPE);
	z_spe[ul].GetErr(body_ul_SPE_z_fc);
	z_spe[ul].PID.err=z_spe[ul].PID.err*100;
	z_spe[ul].PID.ec=imu.Eacc.z*100;             //���ٶȵĲ���ֱֵ����Ϊ����
	//z_spe.Err_d(dt);
	z_spe[ul].Cal();
	//������
	z_spe[ul].PID.output=0.5*z_spe[ul].PID.output+0.5*z_dis[ul].PID.output;
	VAL_LIMIT(z_spe[ul].PID.output,-500,500);
  Rc.g_ThrVAL=Rc.g_ThrVAL+((1-Rc.g_Weight[thr])*z_spe[ul].PID.output);
}else{z_spe[ul].PID.output=0;}

//��ѹ����
if(Rc.g_CH[ksl]==dis_ms_z) 
{
	//z_spe[ms].PID.expect=z_dis[ms].PID.output/100;
	
	z_spe[ms].PID.expect=MAX_Z_SPE*(Rc.g_Weight[thr]-0.5)*2;
  VAL_LIMIT(z_spe[ms].PID.expect,-MAX_Z_SPE,MAX_Z_SPE);
	z_spe[ms].GetErr(body_ms_SPE_z_fc);
	z_spe[ms].PID.err=z_spe[ms].PID.err*100;
	z_spe[ms].PID.ec=imu.Eacc.z*100;             //���ٶȵĲ���ֱֵ����Ϊ����
	z_spe[ms].disErr_i(dt,MAX_Z_SPE/3.0);
	z_spe[ms].PID.offsetKi= z_spe[ms].PID.Ki*0.4*(1-fabs((Rc.g_Weight[thr]-0.5))*2);//
	z_spe[ms].Cal();
	//������
	VAL_LIMIT(z_spe[ms].PID.output,-500,500);
  Rc.g_ThrVAL=Rc.g_ThrVAL+((1-Rc.g_Weight[thr])*z_spe[ms].PID.output);
}else{z_spe[ms].PID.output=0;}
//���
if(Rc.g_CH[ksl]!=dis_ms_z&&Rc.g_CH[ksl]!=dis_ul_z)
{
//float fd=imu.Eacc.z*z_spe[ms].PID.Kd;
//VAL_LIMIT(fd,-100,100);
//Rc.g_ThrVAL=Rc.g_ThrVAL+((1-Rc.g_Weight[thr])*fd);
Rc.g_ThrVAL=Rc.g_ThrVAL+((1-Rc.g_Weight[thr]));
}
}
	
