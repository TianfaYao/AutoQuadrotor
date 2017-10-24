#include "dev_ms5611.h"
#include "dev_clock.h"
#include "uav_filter.h"
_MS5611 Ms5611;
void _MS5611::Reset(void)
{
  SingleWrite(MS5611_ADDR, CMD_RESET, 1);
}

//����0 ˵����ȡʧ�� 1��ȡ�ɹ�
u8 _MS5611::ReadProm(void)
{
  uint8_t  rxbuf[2]={0,0};
	u8 readcheck=0;
	for(u8 i=0;i<PROM_NB;i++)
	{
	 readcheck+=MultRead(MS5611_ADDR, CMD_PROM_RD + i * 2, 2, rxbuf);
	 Prom[i]=rxbuf[0]<<8|rxbuf[1];
   rxbuf[0]=0;rxbuf[1]=0;
		
	}
	if(readcheck==PROM_NB)
		return 0;
	else
		return 1;
}
//��ѹ���ݲ���
void  _MS5611::PressureSample(void)
{
 MultRead(MS5611_ADDR, CMD_ADC_READ, 3, PressureRx); 
}
//�¶����ݲ���
void  _MS5611::TemperatureSample(void)
{
MultRead( MS5611_ADDR, CMD_ADC_READ, 3, TemperatureRx ); 
}
//��ѹ��������
void _MS5611::PressureSampleTriger(void)
{
	SingleWrite(MS5611_ADDR, CMD_ADC_CONV + CMD_ADC_D1 + MS5611_OSR, 1); // D1 (pressure) conversion start!
}
//�¶Ȳ�������
void _MS5611::TemperatureSampleTriger(void)
{
SingleWrite(MS5611_ADDR, CMD_ADC_CONV + CMD_ADC_D2 + MS5611_OSR, 1); // D2 (temperature) conversion start!
}
	

void _MS5611::Configuration(void)
{
	
	//
	Delay_ms(10);
	//��������λ
	Reset();
	Delay_ms(3);
	SensorReady = ReadProm() ;
	//��ʼ��ȡ�¶�
	TemperatureSampleTriger();
	//PressureSampleTriger();
}

u8 _MS5611::DataUpdate(void)
{
 static u8 state=0;
 I2C_FastMode = 0;
	if (state) 
	{
			PressureSample();
			TemperatureSampleTriger();
		  BaroAltCalculate();
			state = 0;
	} 
	else 
	{
		//�¶ȶ�ȡ��ѹ��������
			TemperatureSample();
			PressureSampleTriger();
			state = 1;
	}
	return (state); //1 ��ѹ 0 �¶�
}




void _MS5611::BaroAltCalculate(void)
{

	static u8 baro_start;
	float MS5611Speedfactor;  //��ѹ�ٶȵ�ͨϵ��
	float MS5611factor;
	
  int32_t temperature, off2 = 0, sens2 = 0, delt;
  int32_t pressure;
	float alt_3;
	static u8 get=0;
	int32_t dT;
	int64_t off;
	int64_t sens;
	
	static vs32 sum_tmp_5611 = 0;
	static u8 sum_cnt = BARO_CAL_CNT+10 ;
	
		MeasurTemperature = (TemperatureRx[0] << 16) | (TemperatureRx[1] << 8) | TemperatureRx[2];
		MeasurPressure = (PressureRx[0] << 16) | (PressureRx[1] << 8) | PressureRx[2];
		
    dT = MeasurTemperature - ((uint32_t)Prom[5] << 8);
	
    off = ((uint32_t)Prom[2] << 16) + (((int64_t)dT * Prom[4]) >> 7);
	
    sens = ((uint32_t)Prom[1] << 15) + (((int64_t)dT * Prom[3]) >> 8);
	
    temperature = 2000 + (((int64_t)dT * Prom[6]) >> 23);

    if (temperature < 2000) { // temperature lower than 20degC 
        delt = temperature - 2000;
        delt = delt * delt;
        off2 = (5 * delt) >> 1;
        sens2 = (5 * delt) >> 2;
        if (temperature < -1500) { // temperature lower than -15degC
            delt = temperature + 1500;
            delt = delt * delt;
            off2  += 7 * delt;
					  sens2 += (11 * delt) >> 1;
					  
        }
    }
    off  -= off2; 
    sens -= sens2;
    pressure = (((MeasurPressure * sens ) >> 21) - off) >> 15;
		
		
		alt_3 = (101000 - pressure)/1000.0f;
		pressure = 0.0082f *alt_3 * alt_3 *alt_3 + 0.09f *(101000 - pressure)*100.0f ;
		
	 if(sum_cnt)
		{
			sum_cnt--;
 			if(sum_cnt < BARO_CAL_CNT)
 				sum_tmp_5611 += pressure;
 			if(sum_cnt==0)
			{
//					if(get==1)
//					{
 				    BaroOffset = sum_tmp_5611 / (BARO_CAL_CNT );
						
						//BaroOffsetVAL=BaroOffset-BaroOffsetlast;
						//BaroOffsetlast=BaroOffset;
					
//					}
//				 get=2;
			}
		}
		
		BaroAlt = 10 *(s32)( 0.1f *( pressure - BaroOffset) ) ; //cm
		//�ٶ�
	 	BaroAltSpeed += 5 *0.02 *3.14 *( 50 *( BaroAlt - BaroAltOld ) - BaroAltSpeed ); // 20ms һ�� /0.02 = *50 ��λcm/s
		//����ok
		filter.Moving_Average( (float)( BaroAltSpeed*10),baro_speed_arr,BARO_SPEED_NUM, baro_cnt ,&BaroAltSpeedMove ); //��λmm/s

		//BaroAltSpeed=(BaroAlt-BaroAltOld)/0.01;
//		BaroAltSpeedsou=BaroAltSpeed;  //����ʹ��
//		MS5611Speedfactor= 0.005 / (0.005 + 1 / (2 * M_PI * 5));
//	  BaroAltSpeed=lastBaroAltSpeed * (1 - MS5611Speedfactor) + BaroAltSpeed * MS5611Speedfactor;
//		lastBaroAltSpeed=BaroAltSpeed;
		
//		BaroAltsou=BaroAlt;  //����ʹ��
		MS5611factor=0.005 / (0.005 + 1 / (2 * M_PI * 2));
		BaroAlt=BaroAltOld * (1 - MS5611factor) + BaroAlt * MS5611factor;
		BaroAltOld = BaroAlt;
		

			//�ȴ������ȶ�
			if( baro_start < 200 )
			{
				baro_start=230;//ǯס
				baro_start++;
				BaroAltSpeed = 0;
				BaroAlt = 0;
				BaroAltOld=0;
				get=1;
			}	
			
	Temperature_5611 += 0.01f *( ( 0.01f *temperature ) - Temperature_5611 );
}




