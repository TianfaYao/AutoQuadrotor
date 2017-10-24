#include "imu.h"
#include "uav_filter.h"
#include "uav_rc.h"
#include "imu_dcm.h"
#include "imu_gradient.h"
#include "dev_ak8975.h"

//#define     USEDCM
//#define     USEGD
#define     USEEKF

#define accVelScale      9.80665f / ACC_1G / 10000.0f;
#define angle_z_offfset  15
IMU imu;

extern void  EKF(Vector3f gyr,Vector3f acc,Vector3f mag,uint8_t update_vect[3]	,float dt );
//��̬�����ʼ��
void IMU::Init(void)
{
  	//���ٶȶ��׵�ͨ�˲���ϵ������
	LPF_2nd_Factor_Cal(IMU_LOOP_TIME * 1e-6, ACC_LPF_CUT, &Acc_lpf_2nd);
	AccSpeedfactor=LPF_1st_Factor_Cal(IMU_LOOP_TIME * 1e-6,2);
	//�����˲���ϵ������
	factor = CF_Factor_Cal(IMU_LOOP_TIME * 1e-6, GYRO_CF_TAU);	
}

void IMU::Attitude( float dt)
{
	//�����Ǽ��ٶȼ����ݲ���
  Mpu6050.DataSample();
	
	static u8 count=0;
	count++;
	if(count==5)
	{
	dt=dt*5;
	//���ٶȶ��׵�ͨ�˲�
	Acc_lpf=filter.LPF_2nd(&Acc_lpf_2nd,Mpu6050.acc );
	//����ʵ�ʲ����ļ��ٶȺ��������ٶȵı�ֵ
	accRatio = Acc_lpf.length_squared() * 100 / (ACC_1G * ACC_1G);	
	
	//DCM ������̬
	#ifdef USEDCM
  dcm.DirectionConsineMatrix(Mpu6050.gyrdeg,Acc_lpf,accRatio,dt );
	#endif
	
	#ifdef USEGD
	static u8 initcount=0;
	initcount++;
	if(initcount<10)
	MadgwickAHRSinit(Acc_lpf.x,Acc_lpf.y,Acc_lpf.z,  //�ӿ쿪�����ٵ�λ
		               Ak8975.mag.x,	Ak8975.mag.y,	Ak8975.mag.z);
	else
		initcount=12;

//  MadgwickAHRSupdate(Mpu6050.gyrdeg.x,Mpu6050.gyrdeg.y,Mpu6050.gyrdeg.z,
//  Acc_lpf.x,Acc_lpf.y,Acc_lpf.z,
//	Ak8975.mag.x,	Ak8975.mag.y,	Ak8975.mag.z,dt);	
	  MadgwickAHRSupdate(Mpu6050.gyrdeg.x,Mpu6050.gyrdeg.y,Mpu6050.gyrdeg.z,
  Acc_lpf.x,Acc_lpf.y,Acc_lpf.z,
	0,	0,	0,dt);	
  Quaternion_to_euler(&angle.x,&angle.y,&angle.z);
	#endif 
		
	#ifdef USEEKF  
	uint8_t update_vect[3];
	update_vect[0]=1;
	update_vect[1]=1;
	update_vect[2]=Ak8975.update;
	//update_vect[2]=0;
  EKF(Mpu6050.gyrdeg,Acc_lpf,Ak8975.mag,update_vect, dt );
	Ak8975.update=0;
	#endif 
	//ȥ����������ƫ��
	imu.angle.z=imu.angle.z-angle_z_offfset;
	count=0;
 }
	
}




