#include <REGX51.H>
#include "Delay.h"
#include "LCD1602.h"	

void main()
{
	unsigned char LEDNUM=0;
	LCD_Init();
	LCD_ShowString(1,1,"Hello world");

/* 
	while (1)
       {
				 if (P3_1==0 || P3_0==0)    //��K1��K2��������
         {
					 P2_0=0;                  //LED�����Ϊ0������
         }
         else
         {
					 P2_0=1;                  //LED�����1��Ϩ��
         }
       }  
*/
	while (1)
  {
		if(P3_1==0) 			//��K1��������
		{
			Delay(20);      //��ʱ��������,��ע�ͣ���Ч��Ϊ��סK1����������˲�䣬LED�ƻᶶ��
			while(P3_1==0); //���ּ��
			Delay(20);
			
			P2_0=~P2_0;     //LED1ȡ�����ߵ�ƽ�÷�
		}
  }	
	
/*
	while (1)
  {
		if(P3_1==0) 			//��K1��������
		{
			Delay(20);      //��ʱ��������,��ע�ͣ���Ч��Ϊ��סK1����������˲�䣬LED�ƻᶶ��
			while(P3_1==0); //���ּ��
			Delay(20); 			//��ʱ����
			LEDNUM++;				//��������
			P2_0=~LEDNUM;     //����ȡ�������LED
		}
  }
*/
}