#include <Delay.h>
#include <reg51.h>
#include <lcd1602.h>

/***********************引脚定义************************/
sbit K1=P2^3;					//切换。=1调整时间、=2调整闹钟、=0正常运行
sbit K2=P2^4;					//下一个
sbit K3=P2^5;					//加一
sbit K4=P2^6;					//减一
sbit beep=P2^7;					//蜂鸣器
/***********************变量定义************************/
unsigned char moshi=0;					//用来指示当前处在哪个功能状态
unsigned int  timer=0;					//系统定时变量
unsigned char miao=0,fen=10,shi=6;		//初始时间
unsigned char buzzr_fen=12,buzzr_shi=6;	//闹钟时间
unsigned char two=0;					//指示每个功能下对应的时间
unsigned char time[]={"06:10:10"};		//当前时间显示空间
unsigned char buzzr[]={"06:12"};		//闹钟缓存
/*********************************************************
函数名称：delay
函数作用：延时函数
输入变量：unsigned int i延时时间
返回值：  无
***********************************************************/

/*********************************************************
函数名称：main
函数作用：主函数
输入变量：无
返回值：  无
***********************************************************/
void main()
{
	TMOD|=0X01;							//T0工作方式1
	TH0=0XfC;							//设置初值1ms
	TL0=0X18;	
	ET0=1;								//打开定时器0中断允许
	EA=1;								//打开总中断
	TR0=1;								//打开定时器
	LCD_Init();						//LCD1602初始化
	LCD_ShowString(1,0,"TIME:");
	LCD_ShowString(2,0,"CLOCK:");
	LCD_ShowString(1,5,time);				//显示初始时间
	LCD_ShowString(2,6,buzzr);			//显示闹钟时间
	LCD_WriteCommand(0x0c);					//关闭光标
while(1)								//主循环
{
	 if(!K1)							//按钮1功能切换
	 {
	 Delay(10000);						//延时防抖
		  if(!K1)
		  {
				two=0;					//功能时间指示变量清零
		  	  	if(moshi<2)				//模式切换
			  		moshi++;
				else
					moshi=0;
				switch(moshi)			//设置光标位置
				{
				case 0:	LCD_WriteCommand(0x0c);//正常模式，关闭光标
						break;
				case 1:	LCD_WriteCommand(0x0e);//模式1，打开光标
						LCD_SetCursor(1,6);	//设置光标位置
						break;
				case 2:	LCD_WriteCommand(0x0e);//模式2，打开光标
						LCD_SetCursor(2,6);	//设置光标位置
				}
		  }
		  while(!K1);				   	//等待按钮松开
	 }
	 if(moshi==1)						//模式1，调整时间
	 {
			if(!K2)						//下一个
			{
			   Delay(10000);			//延时防抖
			   	if(!K2)
				{
					if(two<2)
						two++;
					else
						two=0;
				}
				if(two==0)				//光标设置在小时位置
					{
					LCD_WriteCommand(0x0e);
					LCD_SetCursor(1,6);
					}
				if(two==1)					//光标设置在分钟位置
					{
					LCD_WriteCommand(0x0e);
					LCD_SetCursor(1,9);
					}
				if(two==2)					//光标设置在秒位置
					{
					LCD_WriteCommand(0x0e);
					LCD_SetCursor(1,12);
					}
				while(!K2);				//等待按钮松开
			}
			if(two==0)
				{
				if(!K3) 				//小时加一
					{
					Delay(10000);		//延时防抖
						if(!K3)
						{
							if(shi<23)	//限制小时最大值为23
							   shi++;	//小时加一
							else
								shi=0;
						}
					time[0]=shi/10+0x30;//计算
					time[1]=shi%10+0x30;
					LCD_ShowString(1,5,time);//显示
					LCD_SetCursor(1,6);		//保持光标位置
					while(!K3);
					}
				 if(!K4) 				//小时减一
					{
					Delay(10000);		//延时防抖
						if(!K4)
						{
							if(shi>0)
							   shi--;
							else
								shi=23;
						}
					time[0]=shi/10+0x30;//计算
					time[1]=shi%10+0x30;
					LCD_ShowString(1,5,time);//显示
					LCD_SetCursor(1,6);
					while(!K4);
					}
				}
			if(two==1)
				{
				if(!K3) 				//分钟加一
					{
					Delay(10000);		//延时防抖
						if(!K3)
						{
							if(fen<59)	//限制分钟最大值为59
							   	fen++;
							else
								fen=0;
						}
					time[3]=fen/10+0x30;//计算
					time[4]=fen%10+0x30;
					LCD_ShowString(1,5,time);//显示
					LCD_SetCursor(1,9);
					while(!K3);
					}
				 if(!K4) 				//分钟减一
					{
					Delay(10000);		//延时防抖
						if(!K4)
						{
							if(fen>0)
							   fen--;
							else
								fen=59;
						}
					time[3]=fen/10+0x30;//计算
					time[4]=fen%10+0x30;
					LCD_ShowString(1,5,time);//显示
					LCD_SetCursor(1,9);
					while(!K4);
					}
				}
			if(two==2)
				{
				if(!K3) 				//秒加一
					{
					Delay(10000);		//延时防抖
						if(!K3)
						{
							if(miao<59)	//限制分钟最大值为59
							   	miao++;
							else
								miao=0;
						}
					time[3]=miao/10+0x30;//计算
					time[4]=miao%10+0x30;
					LCD_ShowString(1,5,time);//显示
					LCD_SetCursor(1,9);
					while(!K3);
					}
				 if(!K4) 				//秒减一
					{
					Delay(10000);		//延时防抖
						if(!K4)
						{
							if(miao>0)
							   miao--;
							else
								miao=59;
						}
					time[3]=miao/10+0x30;//计算
					time[4]=miao%10+0x30;
					LCD_ShowString(1,5,time);//显示
					LCD_SetCursor(1,9);
					while(!K4);
					}
				}
		}
		if(moshi==2)					//模式2，调整闹钟
		{
			if(!K2)						//下一个
			{
			   Delay(10000);			//延时防抖
			   	if(!K2)
				{
					two=!two;			//切换次序
				}
				if(two==0)				//光标设置在小时
					{
					LCD_WriteCommand(0x0e);	//开启光标
					LCD_SetCursor(2,6);
					}
				else					//光标设置在分钟
					{
					LCD_WriteCommand(0x0e);	//开启光标
					LCD_SetCursor(2,9);
					}
				while(!K2);
			}
			if(two==0)
				{
				if(!K3) 				//小时加一
					{
					Delay(10000);		//延时防抖
						if(!K3)
						{
							if(buzzr_shi<23)//限制小时最大值为23
							   	buzzr_shi++;
							else
								buzzr_shi=0;
						}
					buzzr[0]=buzzr_shi/10+0x30;//计算
					buzzr[1]=buzzr_shi%10+0x30;
					LCD_ShowString(2,6,buzzr);//显示
					LCD_SetCursor(2,6);
					while(!K3);
					}
				 if(!K4) 				//小时减一
					{
					Delay(10000);		//延时防抖
						if(!K4)
						{
							if(buzzr_shi>0)
							   buzzr_shi--;
							else
								buzzr_shi=23;
						}
					//buzzr[0]=buzzr_sh;
					}}
				}}}	