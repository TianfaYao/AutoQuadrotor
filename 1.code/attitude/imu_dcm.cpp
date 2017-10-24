#include "imu_dcm.h"
#include "imu.h"

_DCM dcm;


//�������Ҹ�����̬ DCM
void _DCM:: DirectionConsineMatrix(Vector3f gyro,Vector3f acc,float accRatio ,float dt)
{
	//==============
	 static float vel=0;
   static float EacOffset;
	 static  u8 AccReadFlag=0;
	 static  Vector3i AccReady(0,0,0);
	 static  Vector3f lastAcc;
	
	 //AccReady++;            //���ٶ����ݿ�ʼ��ʱ�������Ʈ�Ĳ��ȶ�
	//======================
  static Vector3f LastGyro,deltaGyroAngle;
  static Vector3f Vector_G(0, 0, ACC_1G), Vector_M(1, 0.f, 0.f);  //�ǵ����ģ���Ȼ����㲻����
	Matrix3f dcm;
	
	
	//���������ǽǶȱ仯�����������������	
	deltaGyroAngle = (gyro + LastGyro) * 0.5 * dt;
	LastGyro = gyro;
	 
	//�����ʾ������ת�����Ҿ���
	dcm.from_euler(deltaGyroAngle);
	
	//�������Ҿ���������������ڻ�������ϵ��ͶӰ
	Vector_G = dcm * Vector_G;
	//�����������ȹ�һ��
	Vector_G=Vector_G/Vector_G.length()*4096;
 
 
//	 //z �����������ϵ��z���ٶ�
//	 imu.Eacc.z=(((Acc_lpf*Vector_G)/Vector_G.length()-Vector_G.length()));
//	//angle.x=rol ,  angle.y=pit , angle.x=yaw
//	 Eacc.y=(Acc_lpf.y-4096*sin(radians(angle.x)))*cos(radians(angle.x)); 
//	 Eacc.x=(Acc_lpf.x-4096*sin(radians(angle.y)))*cos(radians(angle.y));
//	 Eacc=Eacc*986/4096;  //cm/s*s
//	 //Eacc*=0.99f;         //��֤��ֹʱ��û����Ʈ
//   displancement.x=Vector_G.length();
//  if(Rc.g_CH[ksr]!=1)
//	{
//	  speed.z*=0.9;
//		displancement(0,0,0);
//	}
//	else
//	{
//		//����Ȩ����Ϊ�ο����������ٶ�
//	 speed=speed+Eacc*dt;
//	}
	
	//�������Ҿ�����µش������ڻ�������ϵ��ͶӰ
	Vector_M = dcm * Vector_M;
	 //���̳��ȵ�λ��
	Vector_M=Vector_M/Vector_M.length();
	//�����˲���ʹ�ü��ٶȲ���ֵ�������ٶȻ���Ư��
	if (72< (uint16_t)accRatio && (uint16_t)accRatio < 133) //���ٻ��ż���ʱ����
	{
	 Vector_G = filter.CF_1st(Vector_G, acc, 0.99);
	}
	//������һ��
  //���������ں�
 static u16 count=0;
	count++;
//	if(count<=100)//�ϵ�˲����پ���
//	Vector_M =  filter.CF_1st(Vector_M, Ak8975.mag, 0.5);   //�����ϵ��ٶȷ�Ӧ����	
//	if(count>100)
//	{
//		count=105;
//	 Vector_M =  filter.CF_1st(Vector_M, Ak8975.mag, 0.99);   //�����ϵ��ٶȷ�Ӧ����	
//	}
	//�����������ROLL��PITCH
	Vector_G.get_rollpitch(imu.angle);	
	
	//�����������YAW 
  Vector_M.get_yaw(imu.angle);
}
