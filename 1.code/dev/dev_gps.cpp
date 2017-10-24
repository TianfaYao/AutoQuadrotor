#include "dev_gps.h"
#include <stdlib.h>

#define GPSUSART_MAX_RECV_LEN		800					//�����ջ����ֽ���
#define GPSUSART_RX_BUF_old_LEN 800
#define GPSUSART_MAX_SEND_LEN		300					//����ͻ����ֽ���
#define GPSUSART_RX_EN 					1						//0,������;1,����.

nmea_msg gpsx; 											         //GPS��Ϣ

u8 GPSUSART_RX_ready =0;
u8 GPSUSART_RX_BUF[GPSUSART_MAX_RECV_LEN];
u8 GPSUSART_RX_BUF_old[GPSUSART_RX_BUF_old_LEN];
u8 GPSUSART_TX_BUF[GPSUSART_MAX_SEND_LEN]; 		//���ջ����������GPSUSART_MAX_RECV_LEN�ֽ�
uint16_t GPSUSART_RX_STA=0;

//����������ֻ�Ǵ���İ��˹�
/*====================================================================================================*
**����ԭ�� :u8 NMEA_Comma_Pos(u8 *buf,u8 cx)
**��    �� : 
**��    �� : None
**��    �� : None
**��    ע :  //��buf����õ���cx���������ڵ�λ��
             //����ֵ:0~0XFE,����������λ�õ�ƫ��.
            //       0XFF,�������ڵ�cx������			
**====================================================================================================*/
/*====================================================================================================*/				  
u8 NMEA_Comma_Pos(u8 *buf,u8 cx)
{	 		    
	u8 *p=buf;
	while(cx)
	{		 
		if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;//����'*'���߷Ƿ��ַ�,�򲻴��ڵ�cx������
		if(*buf==',')cx--;
		buf++;
	}
	return buf-p;	 
}


/*====================================================================================================*/
/*====================================================================================================*
**����ԭ�� : u32 NMEA_Pow(u8 m,u8 n)
**��    �� : m^n�η�.		
**��    �� : None
**��    �� : None
**��    ע :  //m^n����
              //����ֵ:m^n�η�.		
**====================================================================================================*/
/*====================================================================================================*/	

u32 NMEA_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}



/*====================================================================================================*/
/*====================================================================================================*
**����ԭ�� : int NMEA_Str2num(u8 *buf,u8*dx)
**��    �� : LED��ʼ��
**��    �� : None
**��    �� : None
**��    ע :  //strת��Ϊ����,��','����'*'����
             //buf:���ִ洢��
            //dx:С����λ��,���ظ����ú���
           //����ֵ:ת�������ֵ	
**====================================================================================================*/
/*====================================================================================================*/	

int NMEA_Str2num(u8 *buf,u8*dx)
{
	u8 *p=buf;
	u32 ires=0,fres=0;
	u8 ilen=0,flen=0,i;
	u8 mask=0;
	int res;
	while(1) //�õ�������С���ĳ���
	{
		if(*p=='-'){mask|=0X02;p++;}//�Ǹ���
		if(*p==','||(*p=='*'))break;//����������
		if(*p=='.'){mask|=0X01;p++;}//����С������
		else if(*p>'9'||(*p<'0'))	//�зǷ��ַ�
		{	
			ilen=0;
			flen=0;
			break;
		}	
		if(mask&0X01)flen++;
		else ilen++;
		p++;
	}
	if(mask&0X02)buf++;	//ȥ������
	for(i=0;i<ilen;i++)	//�õ�������������
	{  
		ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
	}
	if(flen>5)flen=5;	//���ȡ5λС��
	*dx=flen;	 		//С����λ��
	for(i=0;i<flen;i++)	//�õ�С����������
	{  
		fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
	} 
	res=ires*NMEA_Pow(10,flen)+fres;
	if(mask&0X02)res=-res;		   
	return res;
}




/*====================================================================================================*/
/*====================================================================================================*
**����ԭ�� : void NMEA_GPGSV_Analysis(nmea_msg *gpsx,u8 *buf)
**��    �� : GSV
**��    �� : None
**��    �� : None
**��    ע :  //����GPGSV��Ϣ
             //gpsx:nmea��Ϣ�ṹ��
            //buf:���յ���GPS���ݻ������׵�ַ
**====================================================================================================*/
/*====================================================================================================*/	

void NMEA_GPGSV_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p,*p1,dx;
	u8 len,i,j,slx=0;
	u8 posx;   	 
	p=buf;
	p1=(u8*)strstr((const char *)p,"$GPGSV");
	len=p1[7]-'0';								//�õ�GPGSV������
	posx=NMEA_Comma_Pos(p1,3); 					//�õ��ɼ���������
	if(posx!=0XFF)gpsx->svnum=NMEA_Str2num(p1+posx,&dx);
	for(i=0;i<len;i++)
	{	 
		p1=(u8*)strstr((const char *)p,"$GPGSV");  
		for(j=0;j<4;j++)
		{	  
			posx=NMEA_Comma_Pos(p1,4+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].num=NMEA_Str2num(p1+posx,&dx);	//�õ����Ǳ��
			else break; 
			posx=NMEA_Comma_Pos(p1,5+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].eledeg=NMEA_Str2num(p1+posx,&dx);//�õ��������� 
			else break;
			posx=NMEA_Comma_Pos(p1,6+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].azideg=NMEA_Str2num(p1+posx,&dx);//�õ����Ƿ�λ��
			else break; 
			posx=NMEA_Comma_Pos(p1,7+j*4);
			if(posx!=0XFF)gpsx->slmsg[slx].sn=NMEA_Str2num(p1+posx,&dx);	//�õ����������
			else break;
			slx++;	   
		}   
 		p=p1+1;//�л�����һ��GPGSV��Ϣ
	}   
}


/*====================================================================================================*/
/*====================================================================================================*
**����ԭ�� : void NMEA_GPGGA_Analysis(nmea_msg *gpsx,u8 *buf)
**��    �� : GGA
**��    �� : None
**��    �� : None
**��    ע :  //����GPGGA��Ϣ
             //gpsx:nmea��Ϣ�ṹ��
            //buf:���յ���GPS���ݻ������׵�ַ
**====================================================================================================*/
/*====================================================================================================*/	

void NMEA_GPGGA_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;    
	p1=(u8*)strstr((const char *)buf,"$GPGGA");
	posx=NMEA_Comma_Pos(p1,6);								//�õ�GPS״̬
	if(posx!=0XFF)gpsx->gpssta=NMEA_Str2num(p1+posx,&dx);	
	posx=NMEA_Comma_Pos(p1,7);								//�õ����ڶ�λ��������
	if(posx!=0XFF)gpsx->posslnum=NMEA_Str2num(p1+posx,&dx); 
	posx=NMEA_Comma_Pos(p1,9);								//�õ����θ߶�
	if(posx!=0XFF)gpsx->altitude=NMEA_Str2num(p1+posx,&dx);  
}





/*====================================================================================================*/
/*====================================================================================================*
**����ԭ�� : void NMEA_GPGSA_Analysis(nmea_msg *gpsx,u8 *buf)
**��    �� : GSA
**��    �� : None
**��    �� : None
**��    ע :  //����GPGSA��Ϣ
             //gpsx:nmea��Ϣ�ṹ��
            //buf:���յ���GPS���ݻ������׵�ַ
**====================================================================================================*/
/*====================================================================================================*/	

void NMEA_GPGSA_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx; 
	u8 i;   
	p1=(u8*)strstr((const char *)buf,"$GPGSA");
	posx=NMEA_Comma_Pos(p1,2);								//�õ���λ����
	if(posx!=0XFF)gpsx->fixmode=NMEA_Str2num(p1+posx,&dx);	
	for(i=0;i<12;i++)										      //�õ���λ���Ǳ��
	{
		posx=NMEA_Comma_Pos(p1,3+i);					 
		if(posx!=0XFF)gpsx->possl[i]=NMEA_Str2num(p1+posx,&dx);
		else break; 
	}				  
	posx=NMEA_Comma_Pos(p1,15);								//�õ�PDOPλ�þ�������
	if(posx!=0XFF)gpsx->pdop=NMEA_Str2num(p1+posx,&dx);  
	posx=NMEA_Comma_Pos(p1,16);								//�õ�HDOPλ�þ�������
	if(posx!=0XFF)gpsx->hdop=NMEA_Str2num(p1+posx,&dx);  
	posx=NMEA_Comma_Pos(p1,17);								//�õ�VDOPλ�þ�������
	if(posx!=0XFF)gpsx->vdop=NMEA_Str2num(p1+posx,&dx);  
}



/*====================================================================================================*/
/*====================================================================================================*
**����ԭ�� : void NMEA_GPRMC_Analysis(nmea_msg *gpsx,u8 *buf)
**��    �� : GPRMC
**��    �� : None
**��    �� : None
**��    ע :  //����GPRMC��Ϣ
             //gpsx:nmea��Ϣ�ṹ��
            //buf:���յ���GPS���ݻ������׵�ַ
**====================================================================================================*/
/*====================================================================================================*/	

void NMEA_GPRMC_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;     
	u32 temp;	   
	float rs;  
	p1=(u8*)strstr((const char *)buf,"GPRMC");//"$GPRMC",������&��GPRMC�ֿ������,��ֻ�ж�GPRMC.
	posx=NMEA_Comma_Pos(p1,1);								//�õ�UTCʱ��
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);	 	//�õ�UTCʱ��,ȥ��ms
		gpsx->utc.hour=temp/10000;
		gpsx->utc.min=(temp/100)%100;
		gpsx->utc.sec=temp%100;	 	 
	}	
	posx=NMEA_Comma_Pos(p1,3);								//�õ�γ��
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->latitude=temp/NMEA_Pow(10,dx+2);	//�õ���
		rs=temp%NMEA_Pow(10,dx+2);				//�õ�'		 
		gpsx->latitude=gpsx->latitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//ת��Ϊ�� 
	}
	posx=NMEA_Comma_Pos(p1,4);								//��γ���Ǳ�γ 
	if(posx!=0XFF)gpsx->nshemi=*(p1+posx);					 
 	posx=NMEA_Comma_Pos(p1,5);								//�õ�����
	if(posx!=0XFF)
	{												  
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->longitude=temp/NMEA_Pow(10,dx+2);	//�õ���
		rs=temp%NMEA_Pow(10,dx+2);				//�õ�'		 
		gpsx->longitude=gpsx->longitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//ת��Ϊ�� 
	}
	posx=NMEA_Comma_Pos(p1,6);								//������������
	if(posx!=0XFF)gpsx->ewhemi=*(p1+posx);		 
	posx=NMEA_Comma_Pos(p1,9);								//�õ�UTC����
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//�õ�UTC����
		gpsx->utc.date=temp/10000;
		gpsx->utc.month=(temp/100)%100;
		gpsx->utc.year=2000+temp%100;	 	 
	} 
}



/*====================================================================================================*/
/*====================================================================================================*
**����ԭ�� : void NMEA_GPVTG_Analysis(nmea_msg *gpsx,u8 *buf)
**��    �� : GPVTG
**��    �� : None
**��    �� : None
**��    ע :  //����GPVTG��Ϣ
             //gpsx:nmea��Ϣ�ṹ��
            //buf:���յ���GPS���ݻ������׵�ַ
**====================================================================================================*/
/*====================================================================================================*/	

void NMEA_GPVTG_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;    
	p1=(u8*)strstr((const char *)buf,"$GPVTG");							 
	posx=NMEA_Comma_Pos(p1,7);								//�õ���������
	if(posx!=0XFF)
	{
		gpsx->speed=NMEA_Str2num(p1+posx,&dx);
		if(dx<3)gpsx->speed*=NMEA_Pow(10,3-dx);	 	 		//ȷ������1000��
	}
}  


/*====================================================================================================*/
/*====================================================================================================*
**����ԭ�� : void GPS_Analysis(nmea_msg *gpsx,u8 *buf)
**��    �� : GPVTG
**��    �� : None
**��    �� : None
**��    ע :  
**====================================================================================================*/
/*====================================================================================================*/

void GPS_Analysis(nmea_msg *gpsx,u8 *buf)
{
	NMEA_GPGSV_Analysis(gpsx,buf);	//GPGSV����
	NMEA_GPGGA_Analysis(gpsx,buf);	//GPGGA���� 	
	NMEA_GPGSA_Analysis(gpsx,buf);	//GPGSA����
	NMEA_GPRMC_Analysis(gpsx,buf);	//GPRMC����
	NMEA_GPVTG_Analysis(gpsx,buf);	//GPVTG����
}


/*====================================================================================================*/
/*====================================================================================================*
**����ԭ�� : void Call_GPS(void)
**��    �� : GPS ���ݽ�������
**��    �� : None
**��    �� : None
**��    ע :  
**====================================================================================================*/
/*====================================================================================================*/

void GPS_Call(void)
{
	  u16 i,rxlen;
    if(GPSUSART_RX_STA>400)//��ֹ��������
    {
    rxlen=GPSUSART_RX_STA;	//�õ����ݳ���
    for(i=0;i<rxlen;i++)GPSUSART_RX_BUF_old[i]=GPSUSART_RX_BUF[i];
    GPSUSART_RX_STA=0;		   	//������һ�ν���
    GPSUSART_RX_BUF_old[i]=0;	//�Զ���ӽ�����
    GPS_Analysis(&gpsx,(u8*)GPSUSART_RX_BUF_old);//�����ַ���
    }
	
}

/*====================================================================================================*/
/*====================================================================================================*
**����ԭ�� : void GPS_DataCacheCall(u8 data)
**��    �� : GPS ���ݻ���ص�����
**��    �� : ���ڽ�������
**��    �� : None
**��    ע :  
**====================================================================================================*/
/*====================================================================================================*/
void GPS_DataCacheCall(u8 data)
{
		if(GPSUSART_RX_STA<GPSUSART_MAX_RECV_LEN)		//�����Խ�������
		{
			GPSUSART_RX_BUF[GPSUSART_RX_STA++]=data;		//��¼���յ���ֵ
		}
		else
		{
			GPSUSART_RX_STA=0;
			GPSUSART_RX_BUF[GPSUSART_RX_STA++]=data;		//��¼���յ���ֵ
		}
}


