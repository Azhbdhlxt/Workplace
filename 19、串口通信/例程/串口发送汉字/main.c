/*********************************************************************************
* 【作    者】：	清翔电子:向量
* 【版    本】：	V1.0
* 【网    站】：	http://www.qxmcu.com/ 
* 【淘宝店铺】：	http://qxmcu.taobao.com/
* 【实验平台】：	清翔 QX-MCS51 单片机开发板
* 【外部晶振】： 	11.0592mhz	
* 【主控芯片】： 	STC89C52
* 【编译环境】： 	Keil μVisio4	
* 【程序功能】： 	串口工作方式1,8位UART比特率9600。发送“大家好！欢迎学习单片机”		   			            			    
* 【使用说明】： 	打开串口助手（STC-ISP软件上有集成）选择对应COM号，设置波特率9600
					接收格式选择【文本格式】。
**********************************************************************************/
#include <reg52.h>
#include <stdio.h>

#define uint unsigned int
#define uchar unsigned char

//毫秒级延时函数定义
void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--); 		
} 

//串口初始化
void UARTInit()
{
	SM0 = 0;	SM1 = 1;//串口工作方式1,8位UART波特率可变
	TR1 = 1;//启动定时器1
	TMOD |= 0x20;//定时器1，工作模式2 8位自动重装
	TH1 = 0xfd;
	TL1 = 0xfd;//设置比特率9600
}

void main()
{
	UARTInit(); //串口初始化
	while(1)
	{
		TI = 1;
		puts("大家好！欢迎学习单片机");
		while(!TI);
		TI = 0;
		delay(1000);
	}	
}
/*************************************
备注：
使用printf()puts()前都应软件置位TI = 1；
printf()puts()  使用putchar函数发送字节

void putchar(uchar sbyte )
{
    while(！TI);   //等待发送完

    SBUF=sbyte;

}

所以第一次调用putchar前没TI=1 永远等待
*************************************/