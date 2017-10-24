
#include "uav_parameter_sd.h"
#include "ff.h"
#include "stdio.h"
#include "string.h"
#include "dev_clock.h"
#include "string.h"
PARAMETER_SD parameter_sd;

static  FATFS fs;
static 	FIL file;
static 	DIR DirInf;

char  acc[]="ACC_X,ACC_Y,ACC_Z,";
char  gyr[]="GYR_X,GYR_Y,GYR_Z,";
char  mag[]="MAG_X,MAG_Y,MAG_Z,";
char  gps[]="GPS_X,GPS_Y,GPS_Z,";
char  tr[]="\r\n";//����



u32 sd_count =0;

//���� ƫ��
int32_t Write2File(char *data,u8 len,u32 offset)
{
	FRESULT result;
	uint32_t bw;
   //�����ļ�ϵͳ

	result = f_mount(&fs, "0:", 0);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		//����ʧ�ܸ�ʽ��
		result = f_mkfs("0:",0,0);
		if (result != FR_OK)
		{
			 return -1;     
		}
		else
		{
		  //���¹���
			result = f_mount(&fs, "0:", 0);			
	   if (result != FR_OK)
      {
			  //�ٴ�ʧ�ܱ�ж��
	      f_mount(NULL, "0:", 0);
			 return -2 ;
			}
		}
	}


	  //���ļ�
	result = f_open(&file, "GPS.csv", FA_CREATE_ALWAYS | FA_WRITE|FA_READ );
	if (result !=  FR_OK)
	{
	  //��ʧ��ж��
	  f_mount(NULL, "0:", 0);
		return -4;
	}
	f_lseek(&file, offset);
	result=f_write(&file,data,len,&bw);
	if(result == FR_OK)
		{
	    f_close(&file);
	    f_mount(NULL, "0:", 0);
			return 1;
		}else
		{
	   f_close(&file);
	    f_mount(NULL, "0:", 0);
			return -4;
		}

		
//=====================pfintf
//	  //д���ļ�
//	 int res=100;
//   res=f_lseek(&file, sd_count*8); 
//	if(res==FR_OK)
//		printf("ƫ�Ƴɹ�\n");
//	else
//		printf("ƫ��ʧ��");
//	 int tr;
//	 tr= f_printf(&file, "%d",1); 
//	     f_lseek(&file, 1);
//	    f_printf(&file, "%c", ',');
//	 if(tr!=-1) 
//	 {
//	   f_close(&file);
//		 f_mount(NULL, "0:", 0);
//		 printf("д��ɹ�\n");
//		 return  1;
//	 }
//	 else
//	 {    
//		   //��ʽ��
//		    f_mkfs("0:",0,0);
//	     //д��ʧ��
//	      f_mount(NULL, "0:", 0);
//		    printf("����д��ʧ�ܸ�ʽ�� \n");
//		    f_close(&file);
//	 }

//================================write
	  
}









u8 gt_cout; u8 gt_tr;
u32 sd_adrr=0;
void AdrrCal(u16 addin);
void PARAMETER_SD::SaveDataInit(void)
{
	     gt_cout=sizeof(acc);
	     gt_tr=sizeof(tr);
	     AdrrCal(0);
       Write2File(acc,gt_cout,sd_adrr);
	     //
	     AdrrCal(gt_cout);
			 Write2File(gyr,gt_cout,sd_adrr);
	     //
	     AdrrCal(gt_cout);
			 Write2File(mag,gt_cout,sd_adrr);
	     //
       AdrrCal(gt_cout);
		   Write2File(gps,gt_cout,sd_adrr);
	     //����
	     AdrrCal(gt_tr+16);//ʱ��ʱ��
  		 Write2File(tr, gt_tr,sd_adrr);  
	     //
       AdrrCal(gt_cout);
       Write2File(acc,gt_cout,sd_adrr);
}

void AdrrCal(u16 addin)
{
 sd_adrr=sd_adrr+addin;
}


