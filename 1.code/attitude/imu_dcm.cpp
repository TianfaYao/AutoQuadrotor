#include "imu_dcm.h"
#include "imu.h"

_DCM dcm;


//方向余弦更新姿态 DCM
void _DCM:: DirectionConsineMatrix(Vector3f gyro,Vector3f acc,float accRatio ,float dt)
{
	//==============
	 static float vel=0;
   static float EacOffset;
	 static  u8 AccReadFlag=0;
	 static  Vector3i AccReady(0,0,0);
	 static  Vector3f lastAcc;
	
	 //AccReady++;            //加速度数据开始的时候出来是飘的不稳定
	//======================
  static Vector3f LastGyro,deltaGyroAngle;
  static Vector3f Vector_G(0, 0, ACC_1G), Vector_M(1, 0.f, 0.f);  //是迭代的，不然会解算不出来
	Matrix3f dcm;
	
	
	//计算陀螺仪角度变化，二阶龙格库塔积分	
	deltaGyroAngle = (gyro + LastGyro) * 0.5 * dt;
	LastGyro = gyro;
	 
	//计算表示单次旋转的余弦矩阵
	dcm.from_euler(deltaGyroAngle);
	
	//利用余弦矩阵更新重力向量在机体坐标系的投影
	Vector_G = dcm * Vector_G;
	//重力向量长度归一化
	Vector_G=Vector_G/Vector_G.length()*4096;
 
 
//	 //z 计算惯性坐标系下z轴速度
//	 imu.Eacc.z=(((Acc_lpf*Vector_G)/Vector_G.length()-Vector_G.length()));
//	//angle.x=rol ,  angle.y=pit , angle.x=yaw
//	 Eacc.y=(Acc_lpf.y-4096*sin(radians(angle.x)))*cos(radians(angle.x)); 
//	 Eacc.x=(Acc_lpf.x-4096*sin(radians(angle.y)))*cos(radians(angle.y));
//	 Eacc=Eacc*986/4096;  //cm/s*s
//	 //Eacc*=0.99f;         //保证静止时其没有零飘
//   displancement.x=Vector_G.length();
//  if(Rc.g_CH[ksr]!=1)
//	{
//	  speed.z*=0.9;
//		displancement(0,0,0);
//	}
//	else
//	{
//		//油门权重作为参考，其收敛速度
//	 speed=speed+Eacc*dt;
//	}
	
	//利用余弦矩阵更新地磁向量在机体坐标系的投影
	Vector_M = dcm * Vector_M;
	 //罗盘长度单位化
	Vector_M=Vector_M/Vector_M.length();
	//互补滤波，使用加速度测量值矫正角速度积分漂移
	if (72< (uint16_t)accRatio && (uint16_t)accRatio < 133) //加速或着减速时相信
	{
	 Vector_G = filter.CF_1st(Vector_G, acc, 0.99);
	}
	//向量归一化
  //罗盘数据融合
 static u16 count=0;
	count++;
//	if(count<=100)//上电瞬间快速纠正
//	Vector_M =  filter.CF_1st(Vector_M, Ak8975.mag, 0.5);   //罗盘上电速度反应较慢	
//	if(count>100)
//	{
//		count=105;
//	 Vector_M =  filter.CF_1st(Vector_M, Ak8975.mag, 0.99);   //罗盘上电速度反应较慢	
//	}
	//计算飞行器的ROLL和PITCH
	Vector_G.get_rollpitch(imu.angle);	
	
	//计算飞行器的YAW 
  Vector_M.get_yaw(imu.angle);
}
