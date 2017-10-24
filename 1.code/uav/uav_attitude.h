#ifndef UAV_ATTITUDE_H
#define UAV_ATTITUDE_H

#include "cal_vector3.h"
#include "imu.h"
#include "dev_ms5611.h"

class _ATTITUDE
{ 
	public:
	 float body_ms_SPE_z;
	 float body_ms_ACC_z_fc;
	 float body_ms_SPE_z_fc;
	 float body_ms_DIS_z_fc;
	
	 float body_ul_SPE_z;
	 float body_ul_SPE_z_fc;
	 float body_ul_DIS_z_fc;

	 u16    ms_flag;
   Vector3f bodyDIS; //����λ��
	 Vector3f bodyDIS_fc; //����λ��
	 Vector3f bodySPE; //�����ٶ�
	 Vector3f bodySPE_fc; //�����ٶ�
	 Vector3f bodyACC; //������ٶ�
   void Init(void);
   void GetDIS_SPE(float dt);
	 void DISControl(float dt);
	 void SPEControl(float dt);
	
		private:
};

extern _ATTITUDE Attitude;

#endif