#include "uav_gps_navigation.h"
#include "math.h"
#define  gain         10000000.0f
#define sq(x) ((x)*(x))

void _GPS_NAVIGATION::CalcRatio(int32_t lat)
{
    float rads = (abs((float)lat)) * (0.0174532925f / gain);  //pi/180 �Ŵ���gain��
    Ratio = cosf(rads);
}



//�õ���������ͽǶ�
void _GPS_NAVIGATION:: Distance_cm_Bearing(int32_t * lat1, int32_t * lon1, int32_t * lat2, int32_t * lon2, uint32_t * dist, int32_t * bearing)
{
    float dLat = *lat2 - *lat1; // difference of latitude in 1/10 000 000 degrees  //γ��
    float dLon = (float) (*lon2 - *lon1) * Ratio;                                  //����
    *dist = sqrtf(sq(dLat) + sq(dLon)) * 1.113195f;               //С�Ƕȱ仯x=sinx=tanx

    *bearing = 9000.0f + atan2f(-dLat, dLon) * 5729.57795f;      // Convert the output radians to 100xdeg  //180/pi*100
    if (*bearing < 0)
        *bearing += 36000;
}
