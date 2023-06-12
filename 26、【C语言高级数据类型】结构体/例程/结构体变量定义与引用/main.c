/*
****************结构体变量定义与引用************************
打开串口后比特率设置为9600
接收模式为文本模式
*/
#include <reg52.h>
#include <stdio.h>

struct date{ //定义结构体类型
	unsigned int year;
	unsigned char month, day;	
};
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
	struct date d1;//定义结构体变量
	UART_Init(); //串口初始化
	TI = 1;		//使用printf()puts()前都应软件置位TI = 1；
	d1.year = 2017;//引用结构体元素
	d1.month = 	5;
	d1.day = 13;
	while(1)
	{
		/*在Keil C51中用printf输出一个单字节数据时要使用%bd,输出十六位数据要用%hd，输出三十二位数据%ld*/
		printf(" year: %hd \n month: %bd \n day: %bd \n", d1.year, d1.month, d1.day);
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