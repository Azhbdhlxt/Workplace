/*
定时器0工作模式1 16位定时计数
数码管动态显示0-10，每隔1秒显示一次
*/
/*********************************************************************************
* 【作    者】：	清翔电子:向量
* 【版    本】：	V1.0
* 【网    站】：	http://www.qxmcu.com/ 
* 【淘宝店铺】：	http://qxmcu.taobao.com/
* 【实验平台】：	清翔 QX-MCS51 单片机开发板
* 【外部晶振】： 	11.0592mhz	
* 【主控芯片】： 	STC89C52
* 【编译环境】： 	Keil μVisio4	
* 【程序功能】： 	定时器0工作模式1 16位定时计数，中断方式响应。
					三位数码管动态扫描，定时器0中断方式扫描。		   			            			    
* 【使用说明】： 	按下S2按键数码管值加一，最大加到120.
					按下S3数码管值减一，最小减到0.
**********************************************************************************/
#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char

sbit DU = P2^6;//数码管段选
sbit WE = P2^7;//数码管段选
sbit key_s2 = P3^0;//独立按键S2
sbit key_s3 = P3^1;//独立按键S3
uchar num;//数码管显示的值
uchar mSec, Sec;//毫秒和秒储存变量

//共阴数码管段选表0-9
uchar code SMGduan[]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,};
//数码管位选码
uchar code SMGwei[] = {0xfe, 0xfd, 0xfb};

/*====================================
函数	： delay(uint z)
参数	：z 延时毫秒设定，取值范围0-65535
返回值	：无
描述	：12T/Fosc11.0592M毫秒级延时
====================================*/
void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--); 		
} 

/*====================================
函数	：display(uchar i)
参数	：i 显示数值，取值范围0-255
返回值	：无
描述	：三位共阴数码管动态显示
====================================*/
void display(uchar i)
{
	static uchar wei; 		
	P0 = 0XFF;//清除断码
	WE = 1;//打开位选锁存器
	P0 = SMGwei[wei];
	WE = 0;//锁存位选数据
	switch(wei)
	{
		case 0: DU = 1; P0 = SMGduan[i / 100]; DU = 0; break;
		case 1: DU = 1; P0 = SMGduan[i % 100 / 10]; DU = 0; break;	
		case 2: DU = 1; P0 = SMGduan[i % 10]; DU = 0; break;		
	}
	wei++;
	if(wei == 3)
		wei = 0;
}
//定时器0初始化
void timer0Init()
{
	EA = 1;	//打开总中断
	ET0 = 1;//打开定时器0中断
	TR0 = 1;	 //启动定时器0
	TMOD = 0X01; //定时器工作模式1，16位定时模式
	TH0 = 0xED;
	TL0 = 0xFF; //定时5ms
}

void main()//main函数自身会循环
{	
	timer0Init();//定时器0初始化
	while(1)
	{
		if(key_s2 == 0)//判断S2是否被按下
		{
			delay(20);//按键消抖
			if(key_s2 == 0)
			{
				if(num != 120)
				num++;
				while(!key_s2);//松手检测
			}	
		}
		if(key_s3 == 0)//判断S3是否被按下
		{
			delay(20);//按键消抖
			if(key_s3 == 0)
			{
				if(num > 0)
					num--;
				while(!key_s3);//松手检测
			}	
		}
	}	
} 

//定时器0中断函数
void timer0() interrupt 1
{
	TH0 = 0xED;
	TL0 = 0xFF; //定时5ms
	display(num); //数码管显示函数	
} 