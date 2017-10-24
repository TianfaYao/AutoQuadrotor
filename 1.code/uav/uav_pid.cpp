/*
  ******************************************************************************
  * @file   
  * @author  ytf
  * @version 
  * @date   
  * @brief    λ��ʽpid
  *    
  *****************************************************************************
*/

#include "uav_pid.h"
#include "cal_math.h"

 
void _PID::Init( const float kp, const float ki,const float kd)
{
    PID.Kp=kp ; PID.Ki=ki;      PID.Kd=kd;\
	  PID.offsetKp=0 ;  PID.offsetKi=0 ;      PID.offsetKd=0 ;\
	  PID.err=0;   PID.lasterr=0;  PID.ec=0; PID.lastec=0;\
	  PID.current=0; PID.output=0;   PID.err_i=0;\
	  PID.errWgight=0;
}

//�õ����
void _PID::GetErr(float current)
{
  PID.err=(PID.expect-current);
}
//�õ����΢��
//�������err_i ����ǰ�棬��Ȼ��һֱ���³����ǲ����
void _PID::Err_d(float dt)
{
	  float pid_dFactor =dt / (dt + 1 / (2 * M_PI * 40));//��ͨ��ϵͳ���ȶ�������Ҫ,�ο������ǲ�����ͨƵ��
    PID.ec=( PID.err- PID.lasterr)*(0.01/dt);//�����������ڵĵ�λ��s����һ�²��������ر����
	  PID.ec= PID.ec*pid_dFactor+ (1-pid_dFactor)*PID.lastec;
	  PID.lastec=PID.ec;
    
}
//�õ�������
void _PID::Err_i(float dt)
{
   PID.err_i+=  (PID.err*dt);
	 PID.lasterr= PID.err;
}
//�������
void _PID::disErr_i(float dt, float max)
{
	if(PID.err>max)
	{
	  PID.err_i+=((PID.Ki+PID.offsetKi)*max*dt);
	}
	else if(PID.err<-max)
	{
	 PID.err_i+=(-(PID.Ki+PID.offsetKi)*max*dt);
	}
	else
	{
	 PID.err_i+=((PID.Ki+PID.offsetKi)*PID.err*dt);
	}
	 PID.lasterr= PID.err;
}

//����ʵ��
 void  _PID::Cal(void)
 {
	PID.output= ( PID.Kp+PID.offsetKp)* PID.err+PID.err_i+ (PID.Kd*PID.offsetKd)*PID.ec;
	                            
 }
