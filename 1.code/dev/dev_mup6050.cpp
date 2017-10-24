#include "dev_mpu6050.h"
_MPU6050 Mpu6050;
#include "cal_vector3.h"
#include "uav_parameter.h"
//����������ϵ
/*
y
|
|
|----------------x


*/
void _MPU6050::Read(void)
{
	u8 get= MultRead(MPU6050_ADDR,MPU6050_RA_ACCEL_XOUT_H,14,buffer);
//  printf("get %d \n",get);
}

void _MPU6050::I2CwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
{
    u8 b;
    MultRead(dev, reg, 1, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	 
    check = !( SingleWrite(dev, reg, b) );
	//  printf("check %d",check);
}


void _MPU6050:: I2CwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{

    u8 b,mask;
    MultRead(dev, reg, 1, &b);
    mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
    data <<= (8 - length);
    data >>= (7 - bitStart);
    b &= mask;
    b |= data;
    SingleWrite(dev, reg, b);
}

//���ò���Ƶ��
void _MPU6050::SetSample(uint16_t hz)
{
    SingleWrite(MPU6050_ADDR, MPU6050_RA_SMPLRT_DIV,1000/hz - 1);
}


//����ʱ��Դ
void _MPU6050::SetClockSource(uint8_t source)
{
    I2CwriteBits(MPU6050_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);

}
//��������������

void _MPU6050::SetFullScaleGyroRange(uint8_t range)
	{
    I2CwriteBits(MPU6050_ADDR, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
    I2CwriteBits(MPU6050_ADDR, MPU6050_RA_GYRO_CONFIG,7, 3, 0x00);   
}
//���ü��ٶȼ�����
void _MPU6050:: SetFullScaleAccelRange(uint8_t range)
	{
    I2CwriteBits(MPU6050_ADDR, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
    I2CwriteBits(MPU6050_ADDR, MPU6050_RA_ACCEL_CONFIG,7, 3, 0x00);   
}


void _MPU6050::SetSleep(uint8_t enabled) 
	{
    I2CwriteBit(MPU6050_ADDR, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

//����ģʽ
void _MPU6050::SetI2CMasterMode(uint8_t enabled) 
	{
    I2CwriteBit(MPU6050_ADDR, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

//����ֱͨ
void _MPU6050::SetI2CBypass(uint8_t enabled)
	{
    I2CwriteBit(MPU6050_ADDR, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}
//���õ�ͨ�˲�����Ƶ��
void _MPU6050::SetDLPF(uint8_t hz)
{
    I2CwriteBits(MPU6050_ADDR, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, hz);
}

void  _MPU6050::Configuration(u16 lpf)
{
    u16 default_filter = 1;


    switch(lpf)
    {
    case 5:
        default_filter = MPU6050_DLPF_BW_5;
        break;
    case 10:
        default_filter = MPU6050_DLPF_BW_10;
        break;
    case 20:
        default_filter = MPU6050_DLPF_BW_20;
        break;
    case 42:
        default_filter = MPU6050_DLPF_BW_42;
        break;
    case 98:
        default_filter = MPU6050_DLPF_BW_98;
        break;
    case 188:
        default_filter = MPU6050_DLPF_BW_188;
        break;
    case 256:
        default_filter = MPU6050_DLPF_BW_256;
        break;
    default:
        default_filter = MPU6050_DLPF_BW_42;
        break;
    }


    SetSleep(0);
    Delay_ms(10);
    SetClockSource(MPU6050_CLOCK_PLL_ZGYRO); // 0x6b   0x03
    Delay_ms(10);
    SetSample(1000);  //1000hz
    Delay_ms(10);
    SetFullScaleGyroRange(MPU6050_GYRO_FS_2000);//���������� +-2000
    Delay_ms(10);
    SetFullScaleAccelRange(MPU6050_ACCEL_FS_8);	//���ٶ�����+-8G
    Delay_ms(10);
    SetDLPF(default_filter);  //42hz
    Delay_ms(10);
    SetI2CMasterMode(0);	 
    Delay_ms(10);
    SetI2CBypass(1);	 
    Delay_ms(10);
}

//���ݲ���
void _MPU6050:: DataSample(void)
{  //������ʱ�������
	  Vector3i  Tgyro,Tacc;
	 //���ݶ�ȡ
	  Read();
	  //��������ƫ����
	  GyroCalOffset();
	 //���ٶȼ���ƫ����
		AccCalOffset();
	 //��������ƫ�ϵ��Խ���
	  GyroAutoOffset(0);

	 
    Tacc.x = ((((int16_t)buffer[0]) << 8) | buffer[1]) ;
    Tacc.y = ((((int16_t)buffer[2]) << 8) | buffer[3]) ;
    Tacc.z = ((((int16_t)buffer[4]) << 8) | buffer[5]) ;

  	Tgyro.x = ((((int16_t)buffer[ 8]) << 8) | buffer[ 9]);
    Tgyro.y = ((((int16_t)buffer[10]) << 8) | buffer[11]);
    Tgyro.z = ((((int16_t)buffer[12]) << 8) | buffer[13]);
	

	  
	  accTem.x=(float)Tacc.x;
	  accTem.y=(float)Tacc.y;
		accTem.z=(float)Tacc.z;
	  
		gyrAdc.x=(float)Tgyro.x; 
		gyrAdc.y=(float)Tgyro.y; 
		gyrAdc.z=(float)Tgyro.z; 
	
	
  	acc=accTem-accOffset;
	  gyrTem=gyrAdc-gyrOffset; //�ֶ�������ƫ
    gyr=gyrTem-gyrAutoOffset;//�ϵ��Զ�������ƫ
	  //����������ת������/s
	  GetGyro2dps();
	
	  temperature=((((int16_t)buffer[6]) << 8) | buffer[7]);
	
  //printf("acc x_%d   y_%d   z_%d\n",Tacc.x,Tacc.y,Tacc.z);
// 	printf("acc x_%f   y_%f   z_%f\n",accTem.x,accTem.y,accTem.z);
//	printf("%d\n",temperature);
//	printf("gyr x_%f  y_%f   z_%f\n",gyrAdc.x,gyrAdc.y,gyrAdc.z);
} 



//���ٶ���ƫ����
void _MPU6050::AccCalOffset(void)
{
	if(g_CallSetAccOffset)
		{
			static Vector3f	tempAcc;
			static uint16_t cnt_a=0;

			if(cnt_a==0)
			{
				accOffset(0, 0, 0);
				tempAcc(0, 0, 0);
				cnt_a = 1;
				return;
			}			
			tempAcc.x += accTem.x;
			tempAcc.y += accTem.y;
			tempAcc.z += (accTem.z-ACC_1G); //��ǰ��ȥ���׳���
			if(cnt_a == 50)
			{
				tempAcc = tempAcc/cnt_a;
				cnt_a = 0;
				g_CallSetAccOffset = 0;
			 Pamameter.SaveAccOfferset(tempAcc);
			//������ݲ���
			 tempAcc(0,0,0);
			 Delay_ms(3);
			//���µ�ǰ��������
		 	Pamameter.ReadAccOfferset();
				return;
			}
			cnt_a++;		
		}	
}


//��������ƫ����
void _MPU6050::GyroCalOffset(void)
{
	if(g_CallSetGyrOffset)
	{
		static Vector3f	tempGyro;
		static uint16_t cnt_g=0;
		if(cnt_g==0)
		{
			gyrOffset(0, 0, 0);
			tempGyro(0, 0, 0);
			cnt_g = 1;
			return;
		}
		tempGyro += gyrAdc;
		if(cnt_g == 50)
		{
			tempGyro= tempGyro/cnt_g;
			cnt_g = 0;
			g_CallSetGyrOffset = 0;
			 Pamameter.SaveGyrOfferset(tempGyro);
			//������ݲ���
			 tempGyro(0,0,0);
			 Delay_ms(3);
			//���µ�ǰ��������
		 	Pamameter.ReadGyrOfferset();
			return;
		}
		cnt_g++;
	}
}



void _MPU6050::GyroAutoOffset(u8 flyReady)
{
	//if(!fly_ready)  //δ����
	if(1) 
		{
			 //x    ���ɻ��Ƿ��ڶ�
			 if(fabs(gyr.x-gyrLast.x)<5) //��
			 {
			   gyrSelfCheckFlage.x++;
			 }
			 else //һ��������ȫ��������
			 {
				 gyrSelfCheckFlage.x=0;
				 gyrSelfCheckFlage.y=0;
				 gyrSelfCheckFlage.z=0;
			 }
			  gyrLast.x=gyr.x;
			 //y
			 if(fabs(gyr.y-gyrLast.y)<5) //��
			 {
			   gyrSelfCheckFlage.y++;
			 }
			 else
			 {
			   gyrSelfCheckFlage.x=0;
				 gyrSelfCheckFlage.y=0;
				 gyrSelfCheckFlage.z=0;
			 }
			  gyrLast.y=gyr.y;
			 //z
			 if(fabs(gyr.z-gyrLast.z)<5) //��
			 {
			   gyrSelfCheckFlage.z++;
			 }
			 else
			 {
			  gyrSelfCheckFlage.x=0;
				 gyrSelfCheckFlage.y=0;
				 gyrSelfCheckFlage.z=0;
			 }
			  gyrLast.z=gyr.z;
			 //x
			 if(gyrSelfCheckFlage.x>OFFSET_AV_NUM*10)//����500��û�г��ֶ�  �����ڽ���������ҲҪ���ϼ���Ƿ� ��������ǳ����춯��������ֹʹ��֮ǰ��������
			 {
				 if(AutoCorrect.x==0)
				 {
					 gyrAutoOffsetCount.x++;
					 gyrAutoOffsetTemp.x+=gyr.x;
					 if(gyrAutoOffsetCount.x>=OFFSET_AV_NUM)
						 {
							
							gyrAutoOffset.x=(float)gyrAutoOffsetTemp.x/OFFSET_AV_NUM;
							 AutoCorrect.x=1;  //����Ѿ���������Ҫ�����������ظ�����
							 gyrAutoOffsetCount.x=0;
							 gyrAutoOffsetTemp.x=0;
						 }
					 }
				 
         if(gyrSelfCheckFlage.x>=6000)//��ֹ���
				 {
				  gyrSelfCheckFlage.x=OFFSET_AV_NUM;
				 }
			 }
			 else
			 {

				 if(AutoCorrect.x==0)
			    gyrAutoOffset.x=0;//����ʱ�����춯�����߻�û��ʼ������ʱ������flash ֮ǰ���µľ���
					gyrAutoOffsetCount.x=0;
			 }
			 //y
			 if(gyrSelfCheckFlage.y>OFFSET_AV_NUM*10)//����500��û�г��ֶ�  �����ڽ���������ҲҪ���ϼ���Ƿ� ��������ǳ����춯��������ֹʹ��֮ǰ��������
			 {
				 if(AutoCorrect.y==0)
				 {
					 gyrAutoOffsetCount.y++;
					 gyrAutoOffsetTemp.y+=gyr.y;
					 if(gyrAutoOffsetCount.y>=OFFSET_AV_NUM)
						 {
							
							gyrAutoOffset.y=(float)gyrAutoOffsetTemp.y/OFFSET_AV_NUM;
							 AutoCorrect.y=1;  //����Ѿ���������Ҫ�����������ظ�����
							 gyrAutoOffsetCount.y=0;
							 gyrAutoOffsetTemp.y=0;
						 }
					 }
				 
         if(gyrSelfCheckFlage.y>=6000)//��ֹ���
				 {
				  gyrSelfCheckFlage.y=OFFSET_AV_NUM;
				 }
			 }
			 else
			 {
				  if(AutoCorrect.y==0)
			    gyrAutoOffset.y=0;//����ʱ�����춯�����߻�û��ʼ������ʱ������flash ֮ǰ���µľ���
					gyrAutoOffsetCount.y=0;
			 }
			 //z
			 			 if(gyrSelfCheckFlage.z>OFFSET_AV_NUM*10)//����500��û�г��ֶ�  �����ڽ���������ҲҪ���ϼ���Ƿ� ��������ǳ����춯��������ֹʹ��֮ǰ��������
			 {
				 if(AutoCorrect.z==0)
				 {
					 gyrAutoOffsetCount.z++;
					 gyrAutoOffsetTemp.z+=gyr.z;
					 if(gyrAutoOffsetCount.z>=OFFSET_AV_NUM)
						 {
							
							gyrAutoOffset.z=(float)gyrAutoOffsetTemp.z/OFFSET_AV_NUM;
							 AutoCorrect.z=1;  //����Ѿ���������Ҫ�����������ظ�����
							 gyrAutoOffsetCount.z=0;
							 gyrAutoOffsetTemp.z=0;
						 }
					 }
				 
         if(gyrSelfCheckFlage.z>=6000)//��ֹ���
				 {
				  gyrSelfCheckFlage.z=OFFSET_AV_NUM;
				 }
			 }
			 else
			 {
				  if(AutoCorrect.z==0)
			    gyrAutoOffset.z=0;//����ʱ�����춯�����߻�û��ʼ������ʱ������flash ֮ǰ���µľ���
					gyrAutoOffsetCount.z=0;
			 }

    }
 }

 //����ֵת���� ��/s
 void  _MPU6050::GetGyro2dps(void)
{
	gyrdeg.x = radians(gyr.x * MPU6050G_s2000dps);   // dps
	gyrdeg.y = radians(gyr.y * MPU6050G_s2000dps);   // dps
	gyrdeg.z = radians(gyr.z * MPU6050G_s2000dps);   // dps	
}


