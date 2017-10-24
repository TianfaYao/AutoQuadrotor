#include "cal_math.h"


float invSqrt(float x)  
{  
    float xhalf = 0.5f * x;  
    int i = *(int*)&x;          // get bits for floating value  
    i =  0x5f375a86 - (i>>1);    // gives initial guess  
    x = *(float*)&i;            // convert bits back to float  
    x = x * (1.5f - xhalf*x*x); // Newton step  
    return x;  
}  



//�Ƕ�ת����
float radians(float deg) {
	return deg * DEG_TO_RAD;
}

//����ת�Ƕ�
float degrees(float rad) {
	return rad * RAD_TO_DEG;
}

//��ƽ��
float sq(float v) {
	return v*v;
}

//2ά��������
float pythagorous2(float a, float b) {
	return sqrtf(sq(a)+sq(b));
}

//3ά��������
float pythagorous3(float a, float b, float c) {
	return sqrtf(sq(a)+sq(b)+sq(c));
}

//4ά��������
float pythagorous4(float a, float b, float c, float d) {
	return sqrtf(sq(a)+sq(b)+sq(c)+sq(d));
}
//��������
 float deathzoom(float x,float zoom)
{
	float t;
	if(x>0)
	{
		t = x - zoom;
		if(t<0)
		{
			t = 0;
		}
	}
	else
	{
		t = x + zoom;
		if(t>0)
		{
			t = 0;
		}
	}
  return (t);
}
//
float To_180_degrees(float x)//�Ƕȹ鵽����180
{
	return (x>180?(x-360):(x<-180?(x+360):x));
}


