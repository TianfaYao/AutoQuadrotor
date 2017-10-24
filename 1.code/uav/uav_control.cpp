
/*
  ******************************************************************************
  * @file   
  * @author  ytf
  * @version 
  * @date   
  * @brief   ����ִ���ļ������е����������ִ��
  *    
  *****************************************************************************
*/
#include "uav_control.h"
#include "uav_pid.h"
#include "imu.h"
#include "dev_mpu6050.h"
#include "uav_rc.h"
#include "cal_math.h"
#include "dev_moto.h"
#include "dev_mavlink.h"



#define MAX_ANGLE              30
#define MAX_YAW_ANGULAR        150
#define MAX_ROLL_PITCH_ANGULAR 400
#define MAX_ANGLE              30
#define DATA_MIDLE             30//������������������


_PID 			qAngle[3];
_PID      qAngular[3];

_CONTROL Control;
void _CONTROL::Init(void)
{
//�ǶȻ����Ʋ�����ʼ�� ��flash��ȡ
	qAngle[x].Init(qAngle[x].PID.Kp,qAngle[x].PID.Ki,qAngle[x].PID.Kd);  //roll
	qAngle[y].Init(qAngle[y].PID.Kp,qAngle[y].PID.Ki,qAngle[y].PID.Kd);  //pit
	qAngle[z].Init(qAngle[z].PID.Kp,qAngle[z].PID.Ki,qAngle[z].PID.Kd);  //yaw
	
//���ٶȻ����Ʋ�����ʼ�� ��flash��ȡ
	qAngular[x].Init(qAngular[x].PID.Kp,qAngular[x].PID.Ki,qAngular[x].PID.Kd);  //roll
	qAngular[y].Init(qAngular[y].PID.Kp,qAngular[y].PID.Ki,qAngular[y].PID.Kd);  //pit
	qAngular[z].Init(qAngular[z].PID.Kp,qAngular[z].PID.Ki,qAngular[z].PID.Kd);  //yaw
}

//�ǶȻ�
void _CONTROL::Angle(float dt)
{
	qAngle[x].PID.expect=MAX_ANGLE*deathzoom(Rc.g_CH[rol],30)/500.f;
	qAngle[y].PID.expect=-MAX_ANGLE*deathzoom(Rc.g_CH[pit],30)/500.f;
	if(!Rc.g_Thr_Low)//����ֵ����
	{
	 qAngle[z].PID.expect+=MAX_YAW_ANGULAR*deathzoom(Rc.g_CH[yaw],50)/500.f*dt;
	}
	else
	{
   qAngle[z].PID.expect += 1 *3.14 *dt *( imu.angle.z -qAngle[z].PID.expect );           //��ͨ�˲�
	}
		qAngle[x].PID.expect = To_180_degrees(qAngle[x].PID.expect);      //�Ƕȹ鵽����180
		qAngle[y].PID.expect = To_180_degrees(qAngle[y].PID.expect);      //�Ƕȹ鵽����180
	  qAngle[z].PID.expect = To_180_degrees(qAngle[z].PID.expect);      //�Ƕȹ鵽����180
	
	
	//�õ����
	 qAngle[x].GetErr(imu.angle.x);
	 qAngle[y].GetErr(imu.angle.y);
	 qAngle[z].GetErr(imu.angle.z);
	
	 qAngle[x].PID.err = To_180_degrees(qAngle[x].PID.err);      //�Ƕ����鵽����180
	 qAngle[y].PID.err = To_180_degrees(qAngle[y].PID.err);      //�Ƕ����鵽����180
	 qAngle[z].PID.err = To_180_degrees(qAngle[z].PID.err);      //�Ƕ����鵽����180
		 
	//������
	qAngle[x].PID.errWgight=abs(qAngle[x].PID.err/MAX_ANGLE);//0- 1
  qAngle[y].PID.errWgight=abs(qAngle[y].PID.err/MAX_ANGLE);
	qAngle[z].PID.errWgight=abs(qAngle[z].PID.err/MAX_ANGLE);
	
	 VAL_LIMIT(qAngle[x].PID.errWgight,0,1);
	 VAL_LIMIT(qAngle[y].PID.errWgight,0,1);
	 VAL_LIMIT(qAngle[z].PID.errWgight,0,1);
	 
//===========================================================================================
	 //������̬�������� ���Խ��ȨֵԽ�ӽ�1 Ȼ�󽫱�����������
//   qAngle[x].PID.offsetKp= qAngle[x].PID.Kp*0.1*qAngle[x].PID.errWgight;     //10%
//	 qAngle[y].PID.offsetKp= qAngle[y].PID.Kp*0.1*qAngle[y].PID.errWgight;     //10%
//	 qAngle[z].PID.offsetKp= qAngle[z].PID.Kp*0.1*qAngle[z].PID.errWgight;     //10%
	//�޷�
	 //���ֶ�̬�������� ���Խ����ֵĲ���ҪԽС //�⻷I ��Ҫ��������������仯�跶Χ��Ҫ��Դ�Щ
//	 qAngle[x].PID.offsetKi= qAngle[x].PID.Ki*0.4*(1-qAngle[x].PID.errWgight);     //10%
//	 qAngle[y].PID.offsetKi= qAngle[x].PID.Ki*0.4*(1-qAngle[x].PID.errWgight);     //10%
//	 qAngle[z].PID.offsetKi= qAngle[x].PID.Ki*0.4*(1-qAngle[x].PID.errWgight);     //10%
	 
	 
	//�����޷�
	qAngle[x].PID.limitI=MAX_ANGLE*0.5*Rc.g_Weight[thr];
	qAngle[y].PID.limitI=MAX_ANGLE*0.5*Rc.g_Weight[thr];
	qAngle[z].PID.limitI=MAX_ANGLE*0.5*Rc.g_Weight[thr];
	//΢��
	qAngle[x].Err_d(dt); 
	qAngle[y].Err_d(dt);
	qAngle[z].Err_d(dt);
	
	if(Rc.g_CH[ksr]!=1)
	{
		for(u8 i=0;i<3;i++)
		{
			qAngle[i].PID.err_i=0;
		}
	}
	//�������
	qAngle[x].disErr_i(dt,qAngle[x].PID.limitI/3);
	qAngle[x].disErr_i(dt,qAngle[x].PID.limitI/3);
	qAngle[x].disErr_i(dt,qAngle[x].PID.limitI/3);
	//�ǶȻ����޷�
	VAL_LIMIT(qAngle[x].PID.err_i,-qAngle[x].PID.limitI,qAngle[x].PID.limitI);
	VAL_LIMIT(qAngle[y].PID.err_i,-qAngle[y].PID.limitI,qAngle[y].PID.limitI);
	VAL_LIMIT(qAngle[z].PID.err_i,-qAngle[z].PID.limitI,qAngle[z].PID.limitI);
	//����޷�
  VAL_LIMIT(qAngle[x].PID.err,-90,90);
  VAL_LIMIT(qAngle[y].PID.err,-90,90);
  VAL_LIMIT(qAngle[z].PID.err,-90,90);
	//
	qAngle[x].Cal();
	qAngle[y].Cal();
	qAngle[z].Cal();	

}

//���ٶȻ�
void _CONTROL::Angular(float dt)
{

   float gyr_Err[3]={0};
	 float gyr_Err_d[3]={0};
   static float gyr_Err_i[3]={0};
	 static float last_gyr_Err[3]={0};
	 	//���Ŷ�
   float Anti_interference[3]={0};
	 gyr_Err[x]=Mpu6050.gyrdeg.x-last_gyr_Err[x];
	 gyr_Err[y]=Mpu6050.gyrdeg.y-last_gyr_Err[y];
	 gyr_Err[z]=Mpu6050.gyrdeg.z-last_gyr_Err[z];
	 
	 gyr_Err_i[x]+=gyr_Err[x];
	 gyr_Err_i[y]+=gyr_Err[y];
	 gyr_Err_i[z]+=gyr_Err[z];
	 
	 last_gyr_Err[x]=Mpu6050.gyrdeg.x;
	 last_gyr_Err[y]=Mpu6050.gyrdeg.y;
	 last_gyr_Err[z]=Mpu6050.gyrdeg.z;
	
	 for(u8 i=0;i<3;i++)
	   Anti_interference[i]=qAngular[x].PID.Ki*gyr_Err_i[i]+ qAngular[x].PID.Kd*gyr_Err[i];
	 
	 
	
	//�ڻ�����
   qAngular[x].PID.expect=MAX_ROLL_PITCH_ANGULAR*(qAngle[x].PID.output/MAX_ANGLE);
	 qAngular[y].PID.expect=MAX_ROLL_PITCH_ANGULAR*(qAngle[y].PID.output/MAX_ANGLE);
	 qAngular[z].PID.expect=MAX_ROLL_PITCH_ANGULAR*(qAngle[z].PID.output/MAX_ANGLE);
	//�ڻ������޷�
	 VAL_LIMIT(  qAngular[x].PID.expect, -MAX_ROLL_PITCH_ANGULAR,MAX_ROLL_PITCH_ANGULAR);
	 VAL_LIMIT(  qAngular[x].PID.expect, -MAX_ROLL_PITCH_ANGULAR,MAX_ROLL_PITCH_ANGULAR);
	 VAL_LIMIT(  qAngular[x].PID.expect, -MAX_ROLL_PITCH_ANGULAR,MAX_ROLL_PITCH_ANGULAR);
	
	//�ڻ����
	//��������ϵ�ʹ���������ϵ��һ��
	 qAngular[x].GetErr(Mpu6050.gyrdeg.x*100);     // max 2000*0.017=34 ./s  =34   
	 qAngular[y].GetErr(-Mpu6050.gyrdeg.y*100);     //��λ̫С�Ŵ�100������������
	 qAngular[z].GetErr(-Mpu6050.gyrdeg.z*100);
	//�ڻ����Ȩ��
	 qAngular[x].PID.errWgight=abs(qAngular[x].PID.err)/MAX_ROLL_PITCH_ANGULAR;
	 qAngular[y].PID.errWgight=abs(qAngular[y].PID.err)/MAX_ROLL_PITCH_ANGULAR;
	 qAngular[z].PID.errWgight=abs(qAngular[z].PID.err)/MAX_YAW_ANGULAR;
	 //�ڻ������޷�
	//===============================================================================
	
	 VAL_LIMIT(qAngular[x].PID.errWgight,0,1);
	 VAL_LIMIT(qAngular[y].PID.errWgight,0,1);
	 VAL_LIMIT(qAngular[z].PID.errWgight,0,1);
	
//	 //������̬�������� ���Խ��ȨֵԽ�ӽ�1 Ȼ�󽫱�����������
//   qAngular[x].PID.offsetKp= qAngular[x].PID.Kp*0.1*qAngular[x].PID.errWgight;     //10%
//	 qAngular[y].PID.offsetKp= qAngular[y].PID.Kp*0.1*qAngular[y].PID.errWgight;     //10%
//	 qAngular[z].PID.offsetKp= qAngular[z].PID.Kp*0.1*qAngular[z].PID.errWgight;     //10%
//	//�޷�
//	 //���ֶ�̬�������� ���Խ����ֵĲ���ҪԽС
//	 qAngular[x].PID.offsetKi= qAngular[x].PID.Ki*0.1*(1-qAngular[x].PID.errWgight);     //10%
//	 qAngular[y].PID.offsetKi= qAngular[x].PID.Ki*0.1*(1-qAngular[x].PID.errWgight);     //10%
//	 qAngular[z].PID.offsetKi= qAngular[x].PID.Ki*0.1*(1-qAngular[x].PID.errWgight);     //10%
	 
	 
	 //����Խ�����Խ����Ҫ���������������ϵͳ��
   qAngular[x].PID.offsetKd= (0.55+0.45*Rc.g_Weight[thr]);
   qAngular[y].PID.offsetKd= (0.55+0.45*Rc.g_Weight[thr]);
   qAngular[z].PID.offsetKd= (0.55+0.45*Rc.g_Weight[thr]);
	 //===============================================�������߰����������Ҫ�滵 ========================
	 
	 qAngular[x].PID.limitI=MAX_ROLL_PITCH_ANGULAR*0.5*Rc.g_Weight[thr];
	 qAngular[y].PID.limitI=MAX_ROLL_PITCH_ANGULAR*0.5*Rc.g_Weight[thr];
	 qAngular[z].PID.limitI=MAX_YAW_ANGULAR*0.5*Rc.g_Weight[thr];
	
	 //�ڻ�΢��
	 qAngular[x].Err_d(dt);
	 qAngular[y].Err_d(dt);
	 qAngular[z].Err_d(dt);
	//�ڻ�����
	//�ڻ������޷� 
	qAngle[x].disErr_i(dt,qAngular[x].PID.limitI);
	qAngle[y].disErr_i(dt,qAngular[y].PID.limitI);
	qAngle[z].disErr_i(dt,qAngular[z].PID.limitI);	 
	//
   VAL_LIMIT( qAngular[x].PID.err_i, -qAngular[x].PID.limitI,qAngular[x].PID.limitI);
   VAL_LIMIT( qAngular[y].PID.err_i, -qAngular[y].PID.limitI,qAngular[y].PID.limitI);
   VAL_LIMIT( qAngular[z].PID.err_i, -qAngular[z].PID.limitI,qAngular[z].PID.limitI);
	 	if(Rc.g_CH[ksr]!=1)
	{
		for(u8 i=0;i<3;i++)
		{
			qAngular[i].PID.err_i=0;
		}
	}
	//�ڻ�����
	
	 qAngular[x].Cal();
	 qAngular[y].Cal();
 	 qAngular[z].Cal();
  //������
	 qAngular[x].PID.output= qAngular[x].PID.expect*(0.45+0.55*Rc.g_Weight[thr])*0.2+
	                          0.8*(qAngular[x].PID.output+Anti_interference[x]); //0-300
	 qAngular[y].PID.output= qAngular[y].PID.expect*(0.45+0.55*Rc.g_Weight[thr])*0.2+
	                            0.8*(qAngular[y].PID.output-Anti_interference[y]);
	 qAngular[z].PID.output= qAngular[z].PID.expect*(0.45+0.55*Rc.g_Weight[thr])*0.2+
	                             0.8*(qAngular[z].PID.output-Anti_interference[z]);
  //��̬���ŷֽ����ӳ�䵽�ĸ����pwm
	 DecompositionOutput();
	//���Ŷ�
  //Anti-interference
	
}

void _CONTROL::DecompositionOutput(void)
{
  float postureVal[4];
	float curve[4];
	u16 motor[4]={0};

 	postureVal[0] = -qAngular[x].PID.output +qAngular[y].PID.output + qAngular[z].PID.output ;
	postureVal[1] = +qAngular[x].PID.output +qAngular[y].PID.output - qAngular[z].PID.output ;
	postureVal[2] = +qAngular[x].PID.output -qAngular[y].PID.output + qAngular[z].PID.output ;
	postureVal[3] = -qAngular[x].PID.output -qAngular[y].PID.output - qAngular[z].PID.output ;	
	
	for(u8 i=0;i<4;i++)
	{
    VAL_LIMIT(postureVal[i], -1000,1000 );
	}
	
	
	curve[0] = (0.55f + 0.45f *abs(postureVal[0])/1000.0f) *postureVal[0] ;    
	curve[1] = (0.55f + 0.45f *abs(postureVal[1])/1000.0f) *postureVal[1] ;
	curve[2] = (0.55f + 0.45f *abs(postureVal[2])/1000.0f) *postureVal[2] ;
	curve[3] = (0.55f + 0.45f *abs(postureVal[3])/1000.0f) *postureVal[3] ;
	
	motor[0] = Rc.g_ThrVAL + Rc.g_Weight[thr] *curve[0] ;//����Խ����̬���Խ��
	motor[1] = Rc.g_ThrVAL + Rc.g_Weight[thr] *curve[1] ;
	motor[2] = Rc.g_ThrVAL + Rc.g_Weight[thr] *curve[2] ;
	motor[3] = Rc.g_ThrVAL + Rc.g_Weight[thr] *curve[3] ;



		for(u8 i=0;i<4;i++)
	{
    VAL_LIMIT(motor[i], 0,1500 );
	}
	
//	for(u8 i=0;i<4;i++)
//	{
//		motor[i] = constrain_uint16(postureVal[i],0,1500 );
//	}
//	if(Rc.qudrotorState==lock)
//	{
//		for(u8 i=0;i<4;i++)
//		{
//			motor[i] = 0;
//		}
//		for(u8 i=0;i<3;i++)
//		{
//		 //δ����ʱȥ����ЧӦ
//		 qAngle[i].PID.err=0;
//		 qAngular[i].PID.err=0;
//		}
//	}

	debug.aux6=Rc.qudrotorState;
	
	if(Rc.g_CH[ksr]!=1)
	{

		for(u8 i=0;i<3;i++)
		{
		 //δ����ʱȥ����ЧӦ
		 qAngle[i].PID.err=0;
		 qAngular[i].PID.err=0;
		}
		 for(u8 i=0;i<4;i++)
		{
			motor[i] = 0;
		}
	}
	debug.aux1=motor[0];
	debug.aux2=motor[1];
	debug.aux3=motor[2];
	debug.aux4=motor[3];
  Moto.SetPwm(motor);
}
