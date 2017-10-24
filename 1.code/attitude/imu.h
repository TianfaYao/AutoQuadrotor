#ifndef IMU_H
#define IMU_H
//#include "includes.h"
#include "uav_filter.h"
class Filter;
class IMU : public Filter
{
	public:
			//ŷ���Ǳ�ʾ�ķ�������̬
	  Vector3f Eacc;      //��������ϵ���¸�������ļ��ٶ�
	  Vector3f Eacc_lpf;  //һ�׵�ͨ
	  Vector3f lastEacc;
	  Vector3f speed;      //�ٶ�
    Vector3f displancement; //λ��

	  Vector3f angle;
    Vector3f Acc_lpf;
	  
	  void Init(void);
    void Attitude( float dt);
	  IMU(){}
	private:
		int32_t accRatio;
	  float  factor;                    //�����˲�ϵ��
		float  AccSpeedfactor;            //�����˲�ϵ��
	  LPF2ndData_t Acc_lpf_2nd; //���ٶȶ����˲�
	
	
};

extern IMU imu;

#endif