#include <reg52.h> 

sbit LED1 = P1^0;
void UART_INIT()
{
	SM0 = 0;
	SM1 = 1;//串口工作方式1
	REN = 1;//允许串口接收
	EA = 1;//开总中断
	ES = 1;//开串口中断
	TMOD = 0x20;//8位自动重装模式
	TH1 = 0xfd;
	TL1 = 0xfd;//9600波特率
	TR1 = 1;//启动定时器1
}
//串口中断
void UART_SER() interrupt 4
{
	if(RI)
	{
		RI = 0;//清除接收标志
		switch(SBUF)
		{
			case 0x01: LED1 = 0; break;//开灯
			case 0x02: LED1 = 1; break;//关灯 
		}
	}
}
void main()
{
	UART_INIT();//串口初始化
	while(1);	
}