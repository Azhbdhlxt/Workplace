/*********************************************************************************
* 【作    者】：	清翔电子:向量
* 【版    本】：	V1.0
* 【网    站】：	http://www.qxmcu.com/ 
* 【淘宝店铺】：	http://qxmcu.taobao.com/
* 【实验平台】：	清翔 QX-MCS51 单片机开发板
* 【外部晶振】： 	11.0592mhz	
* 【主控芯片】： 	STC89C52
* 【编译环境】： 	Keil μVisio4	
* 【程序功能】： 	板子有源蜂鸣器报警，4位LED移动点亮		   			            			    
* 【使用说明】： 
**********************************************************************************/
#include <reg52.h>	//包含51头文件
#include <intrins.h>//包含移位标准库函数头文件

#define uint unsigned int
#define uchar unsigned char
sbit beep = P2^3;//蜂鸣器

uchar temp;//LED灯相关变量
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

void main()//main函数自身会循环
{
	temp = 0xf0;//
	P1 = temp; //1111 0000 点亮4位LED
	delay(100);//毫秒级延时
	while(1)
	{
		temp = _crol_(temp, 1);//循环左移
		P1 = temp;
		beep = ~beep;//	蜂鸣器发出滴滴声
		delay(100);
	}	
}  