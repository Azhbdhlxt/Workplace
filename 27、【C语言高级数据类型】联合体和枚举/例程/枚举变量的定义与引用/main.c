/*
****************枚举变量的定义与引用************************
打开串口后比特率设置为9600
接收模式为文本模式
*/
#include <reg52.h>
#include <stdio.h>

enum eStaSystem {  //枚举类型定义
    DIS_DATE, DIS_TIME
};
//系统运行状态
enum eStaSystem staSystem = DIS_TIME;  //枚举变量定义与赋值
/*毫秒级延时函数*/
void Delay_Ms(unsigned int z)
{
	unsigned int x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}	
/*串口初始化，比特率9600*/
void UART_Init()
{
	TMOD |= 0x20;  	//T1工作模式2  8位自动重装
	TH1 = 0xfd;
	TL1 = 0xfd; 	//比特率9600
	TR1 = 1;		//启动T1定时器
	SM0 = 0;
	SM1 = 1; 		//串口工作方式1 10位异步
}

void main()
{
	UART_Init(); //串口初始化
	TI = 1;//使用printf()puts()前都应软件置位TI = 1；
	while(1)
	{
		if (staSystem == DIS_DATE)
		{
			printf("日期：2017-8-8 \n");
		}
		if (staSystem == DIS_TIME)
		{
			printf("时间：08:30:50 \n");
		}
		Delay_Ms(5000);//5秒发送一次
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

所第调用putchar前没TI=1 永远等待
*************************************/