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
				 if (P3_1==0 || P3_0==0)    //若K1或K2按键按下
         {
					 P2_0=0;                  //LED灯输出为0，点亮
         }
         else
         {
					 P2_0=1;                  //LED灯输出1，熄灭
         }
       }  
*/
	while (1)
  {
		if(P3_1==0) 			//若K1按键按下
		{
			Delay(20);      //延时消除抖动,若注释，则效果为按住K1键，在松手瞬间，LED灯会抖动
			while(P3_1==0); //松手检测
			Delay(20);
			
			P2_0=~P2_0;     //LED1取反，高电平置反
		}
  }	
	
/*
	while (1)
  {
		if(P3_1==0) 			//若K1按键按下
		{
			Delay(20);      //延时消除抖动,若注释，则效果为按住K1键，在松手瞬间，LED灯会抖动
			while(P3_1==0); //松手检测
			Delay(20); 			//延时消抖
			LEDNUM++;				//变量自增
			P2_0=~LEDNUM;     //变量取反输出给LED
		}
  }
*/
}