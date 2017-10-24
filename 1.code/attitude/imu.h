#ifndef IMU_H
#define IMU_H
//#include "includes.h"
#include "uav_filter.h"
class Filter;
class IMU : public Filter
{
	public:
			//欧拉角表示的飞行器姿态
	  Vector3f Eacc;      //惯性坐标系数下各个反向的加速度
	  Vector3f Eacc_lpf;  //一阶低通
	  Vector3f lastEacc;
	  Vector3f speed;      //速度
    Vector3f displancement; //位移

	  Vector3f angle;
    Vector3f Acc_lpf;
	  
	  void Init(void);
    void Attitude( float dt);
	  IMU(){}
	private:
		int32_t accRatio;
	  float  factor;                    //互补滤波系数
		float  AccSpeedfactor;            //互补滤波系数
	  LPF2ndData_t Acc_lpf_2nd; //加速度二阶滤波
	
	
};

extern IMU imu;

#endif