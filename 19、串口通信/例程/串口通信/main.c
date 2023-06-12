/*********************************************************************************
* 【作    者】：	清翔电子:向量
* 【版    本】：	V1.0
* 【网    站】：	http://www.qxmcu.com/ 
* 【淘宝店铺】：	http://qxmcu.taobao.com/
* 【实验平台】：	清翔 QX-MCS51 单片机开发板
* 【外部晶振】： 	11.0592mhz	
* 【主控芯片】： 	STC89C52
* 【编译环境】： 	Keil μVisio4	
* 【程序功能】： 	串口工作方式1,8位UART比特率9600。接收串口数据，数码管以十进制格
					式显示，并且把接收到的数据加1后通过串口发出		   			            			    
* 【使用说明】： 	打开串口助手（STC-ISP软件上有集成）选择对应COM号，设置波特率9600
					发送和接收格式选择HEX格式。
**********************************************************************************/
#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char

sbit DU = P2^6;//数码管段选
sbit WE = P2^7;//数码管段选
uchar num;//数码管显示的值

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
//void delay(uint z)
//{
//	uint x,y;
//	for(x = z; x > 0; x--)
//		for(y = 114; y > 0 ; y--); 		
//} 

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
	REN = 1;//允许串口接收
	TMOD |= 0X01; //定时器工作模式1，16位定时模式
	TH0 = 0xED;
	TL0 = 0xFF; //定时5ms
}
//串口初始化
void UARTInit()
{
	EA = 1;	//打开总中断
	ES = 1; //打开串口中断
	SM0 = 0;	SM1 = 1;//串口工作方式1,8位UART波特率可变
	REN = 1;//串口允许接收
	TR1 = 1;//启动定时器1
	TMOD |= 0x20;//定时器1，工作模式2 8位自动重装
	TH1 = 0xfd;
	TL1 = 0xfd;//设置比特率9600
}
void main()//main函数自身会循环
{	
	timer0Init();//定时器0初始化
	UARTInit();//串口初始化
	while(1);	
} 

//定时器0中断函数
void timer0() interrupt 1
{
	TH0 = 0xED;
	TL0 = 0xFF; //定时5ms
	display(num); //数码管显示函数	
}
//串口中断函数
void UART() interrupt 4
{
	uchar temp;
	if(RI)//判断接收是否完成
	{
		num = SBUF;//读SBUF，读出串口接收到的数据
		RI = 0;//软件清零接收标志位	
		temp = num;//
		SBUF = ++temp;//写SBUF，把要发送的数据送给发送缓存器
	}
	if(TI)//判断是否发送完成
		TI = 0;//清零发送完成标志位	
} 