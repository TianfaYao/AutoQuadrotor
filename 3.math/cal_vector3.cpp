#include "cal_vector3.h"
#include "cal_matrix3.h"
#include "cal_math.h"
#include "math.h"

#define HALF_SQRT_2 0.70710678118654757f	//����2��ֵ


//����������
template <typename T>
float Vector3<T>::length(void) const
{
    return pythagorous3(x, y, z);
}

//������������֮��ļн�
template <typename T>
float Vector3<T>::angle(const Vector3<T> &v2) const
{
    return acosf(((*this)*v2) / (this->length()*v2.length()));
}

//�������
template <typename T>
Vector3<T> Vector3<T>::operator %(const Vector3<T> &v) const
{
    Vector3<T> temp(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    return temp;
}

//�������
template <typename T>
T Vector3<T>::operator *(const Vector3<T> &v) const
{
    return x*v.x + y*v.y + z*v.z;
}

//��������
template <typename T>
Vector3<T> Vector3<T>::operator *(const T num) const
{
    return Vector3<T>(x*num, y*num, z*num);
}

template <typename T>
Vector3<T> &Vector3<T>::operator *=(const T num)
{
    x*=num; y*=num; z*=num;
    return *this;
}

//��������
template <typename T>
Vector3<T> &Vector3<T>::operator /=(const T num)
{
    x /= num; y /= num; z /= num;
    return *this;
}

template <typename T>
Vector3<T> Vector3<T>::operator /(const T num) const
{
    return Vector3<T>(x/num, y/num, z/num);
}

//����ȡ��
template <typename T>
Vector3<T> Vector3<T>::operator -(void) const
{
    return Vector3<T>(-x,-y,-z);
}

//�������
template <typename T>
Vector3<T> Vector3<T>::operator +(const Vector3<T> &v) const
{
    return Vector3<T>(x+v.x, y+v.y, z+v.z);
}

template <typename T>
Vector3<T> &Vector3<T>::operator +=(const Vector3<T> &v)
{
    x+=v.x; y+=v.y; z+=v.z;
    return *this;
}

//�������
template <typename T>
Vector3<T> Vector3<T>::operator -(const Vector3<T> &v) const
{
    return Vector3<T>(x-v.x, y-v.y, z-v.z);
}

template <typename T>
Vector3<T> &Vector3<T>::operator -=(const Vector3<T> &v)
{
    x -= v.x; y -= v.y; z -= v.z;
    return *this;
}

//�Ա����������Ƿ����
template <typename T>
bool Vector3<T>::operator ==(const Vector3<T> &v) const
{
    return (x==v.x && y==v.y && z==v.z);
}

//�Ա����������Ƿ����
template <typename T>
bool Vector3<T>::operator !=(const Vector3<T> &v) const
{
    return (x!=v.x && y!=v.y && z!=v.z);
}

//�������һ�����������õ�һ��������
template <typename T>
Vector3<T> Vector3<T>::operator *(const Matrix3<T> &m) const
{
    return Vector3<T>(*this * m.colx(),
                      *this * m.coly(),
                      *this * m.colz());
}

// ����������һ��������,�õ�3x3����
template <typename T>
Matrix3<T> Vector3<T>::mul_rowcol(const Vector3<T> &v2) const
{
    const Vector3<T> v1 = *this;
    return Matrix3<T>(v1.x * v2.x, v1.x * v2.y, v1.x * v2.z,
                      v1.y * v2.x, v1.y * v2.y, v1.y * v2.z,
                      v1.z * v2.x, v1.z * v2.y, v1.z * v2.z);
}

template <typename T>

void Vector3<T>::get_rollpitch(Vector3<T> &angle)
{
	//angle.x = degrees(atan2f(y, sqrtf(x * x + z* z)));
  angle.x = degrees(atan2f(y,z));	
	VAL_LIMIT(angle.x,-180,180);
	angle.y = degrees(atan2f(x, sqrtf(y * y + z* z)));	
	VAL_LIMIT(angle.y,-180,180);
}

template <typename T>
void Vector3<T>::get_yaw(Vector3<T> &angle)
{
	angle.z = degrees(atan2f(y,x));
	VAL_LIMIT(angle.z,-180,180);
}

template <typename T>
bool Vector3<T>::is_nan(void) const
{
    return isnan(x) || isnan(y) || isnan(z);
}

		
//ֻ�������Ͷ�����

template float Vector3<float>::length(void) const;
template Vector3<float> Vector3<float>::operator %(const Vector3<float> &v) const;
template float Vector3<float>::operator *(const Vector3<float> &v) const;
template Vector3<float> Vector3<float>::operator *(const Matrix3<float> &m) const;

template Vector3<float> &Vector3<float>::operator *=(const float num);
template Vector3<float> &Vector3<float>::operator /=(const float num);
template Vector3<float> &Vector3<float>::operator -=(const Vector3<float> &v);
template Vector3<float> &Vector3<float>::operator +=(const Vector3<float> &v);
template Vector3<float> Vector3<float>::operator /(const float num) const;
template Vector3<float> Vector3<float>::operator *(const float num) const;
template Vector3<float> Vector3<float>::operator +(const Vector3<float> &v) const;
template Vector3<float> Vector3<float>::operator -(const Vector3<float> &v) const;
template Vector3<float> Vector3<float>::operator -(void) const;
template bool Vector3<float>::operator ==(const Vector3<float> &v) const;
template bool Vector3<float>::operator !=(const Vector3<float> &v) const;
template float Vector3<float>::angle(const Vector3<float> &v) const;
template void Vector3<float>::get_rollpitch(Vector3<float> &angle);
template void Vector3<float>::get_yaw(Vector3<float> &angle);
template bool Vector3<float>::is_nan(void) const;


//˫���ȸ����͵Ķ���
template Vector3<double> Vector3<double>::operator %(const Vector3<double> &v) const;
template double Vector3<double>::operator *(const Vector3<double> &v) const;
template Vector3<double> &Vector3<double>::operator *=(const double num);
template Vector3<double> &Vector3<double>::operator /=(const double num);
template Vector3<double> &Vector3<double>::operator -=(const Vector3<double> &v);
template Vector3<double> &Vector3<double>::operator +=(const Vector3<double> &v);
template Vector3<double> Vector3<double>::operator /(const double num) const;
template Vector3<double> Vector3<double>::operator *(const double num) const;
template Vector3<double> Vector3<double>::operator +(const Vector3<double> &v) const;
template Vector3<double> Vector3<double>::operator -(const Vector3<double> &v) const;
template Vector3<double> Vector3<double>::operator -(void) const;
template bool Vector3<double>::operator ==(const Vector3<double> &v) const;
template bool Vector3<double>::operator !=(const Vector3<double> &v) const;
template float Vector3<double>::angle(const Vector3<double> &v) const;
template void Vector3<double>::get_rollpitch(Vector3<double> &angle);
template void Vector3<double>::get_yaw(Vector3<double> &angle);
template bool Vector3<double>::is_nan(void) const;

