#include "dev_clock.h"

volatile uint32_t sysTickUptime = 0;

#define TICK_PER_SECOND 1000 
#define TICK_US	(1000000/TICK_PER_SECOND)
#define  GET_TIME_NUM             4 

void  SysTick_Configuration(void)
{
	RCC_ClocksTypeDef  rcc_clocks;
	uint32_t         cnts;

	RCC_GetClocksFreq(&rcc_clocks);

	cnts = (uint32_t)rcc_clocks.HCLK_Frequency / TICK_PER_SECOND;
	cnts = cnts / 8;

	SysTick_Config(cnts);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}



uint32_t GetSysTime_us(void) 
{
	register uint32_t ms;
	u32 value;
	ms = sysTickUptime;
	value = ms * TICK_US + (SysTick->LOAD - SysTick->VAL) * TICK_US / SysTick->LOAD;
	return value;
}

void Delay_us(uint32_t us)
{
    uint32_t now = GetSysTime_us();
    while (GetSysTime_us() - now < us);
}

void Delay_ms(uint32_t ms)
{
    while (ms--)
        Delay_us(1000);
}

volatile float Cycle_T[GET_TIME_NUM][3];

enum
{
	NOW = 0,
	OLD,
	NEW,
};

float Get_Cycle_T(u8 item)	
{
	Cycle_T[item][OLD] = Cycle_T[item][NOW];	//��һ�ε�ʱ��
	Cycle_T[item][NOW] = GetSysTime_us()/1000000.0f; //���ε�ʱ��
	Cycle_T[item][NEW] = ( ( Cycle_T[item][NOW] - Cycle_T[item][OLD] ) );//�����ʱ�䣨���ڣ�
	return Cycle_T[item][NEW];
}

void Cycle_Time_Init()
{
	u8 i;
	for(i=0;i<GET_TIME_NUM;i++)
	{
		Get_Cycle_T(i);
	}
}

//һ�����ж�һ��
loop_t loop;
void Loop_check()  //TIME INTTERRUPT
{
	loop.time++; 
	loop.cnt_2ms++;
	loop.cnt_5ms++;
	loop.cnt_10ms++;
	loop.cnt_20ms++;
	loop.cnt_50ms++;

	if( loop.check_flag == 1)
	{
		loop.err_flag ++;     //ÿ�ۼ�һ�Σ�֤��������Ԥ��������û�����ꡣ
	}
	else
	{	
		loop.check_flag = 1;	//�ñ�־λ��ѭ�����������
	}
}

s16 loop_cnt;

int time_1h,time_1m,time_1s,time_1ms;

void sys_time()
{ 

  if(time_1ms < 999)
	{
    time_1ms++;

		Loop_check();
	}
	else
	{
    time_1ms =0;
	  if(time_1s<59)
      time_1s++;
		else
		{
			time_1s = 0;
			if(time_1m<59)
				time_1m++;
			else
			{
				time_1m = 0;
				if(time_1h<23)
					time_1h++;
				else
					time_1h = 0;
			}
		}
	}
}


 extern "C" void SysTick_Handler(void)
{
	sysTickUptime++;
	sys_time();
}

