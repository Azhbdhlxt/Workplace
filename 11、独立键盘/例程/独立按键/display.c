/*********************************************************************************
* 【作    者】：	清翔电子:向量
* 【版    本】：	V1.0
* 【网    站】：	http://www.qxmcu.com/ 
* 【淘宝店铺】：	http://qxmcu.taobao.com/
* 【实验平台】：	清翔 QX-MCS51 单片机开发板
* 【外部晶振】： 	11.0592mhz	
* 【主控芯片】： 	STC89C52
* 【编译环境】： 	Keil μVisio4	
* 【程序功能】： 	独立按键识别		   			            			    
* 【使用说明】： 	按下开发板左下角S2按键数码管值+1,最大到9
					按下S3按下，值减一，最小减到0
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

//共阴数码管段选表0-9
uchar  code tabel[]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,};


//毫秒级延时函数定义
void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--); 		
} 

void main()//main函数自身会循环
{
	WE = 1;//打开位选锁存器
	P0 = 0XFE; //1111 1110
	WE = 0;//锁存位选数据
	
	while(1)
	{
		if(key_s2 == 0)//判断S2是否被按下
		{
			delay(20);//按键消抖
			if(key_s2 == 0)
			{
				if(num != 9)//如果值不等于9则+1，功能把值限定为小于9
				num++;
				while(!key_s2);//松手检测
			}	
		}
		if(key_s3 == 0)//判断S3是否被按下
		{
			delay(20);//按键消抖
			if(key_s3 == 0)
			{
				if(num > 0)	//如果大于0则执行减一
					num--;
				while(!key_s3);//松手检测
			}	
		}
		//松手之后刷新显示
		DU = 1;//打开段选锁存器
		P0 = tabel[num];//
		DU = 0;//锁存段选数据
	}	
}  