/*********************************************************************************
* 【作    者】：	清翔电子
* 【版    本】：	V1.1
* 【网    站】：	http://www.qxmcu.com/ 
* 【淘宝店铺】：	http://qxmcu.taobao.com/
* 【实验平台】：	清翔 QX-MCS51 单片机开发板
* 【外部晶振】： 	11.0592mhz	
* 【主控芯片】： 	STC89C52
* 【编译环境】： 	Keil μVisio4	
* 【程序功能】： 	根据光线亮度调节LED10亮度，光亮-LED10暗，光暗-LED10亮		   			            			    
* 【使用说明】： 	用手遮挡光敏电阻，LED10亮度会发生变化 J6跳线帽把LED-DOUT短接
********************************************************************************/
#include <reg52.h>
#include <intrins.h>

#define MAIN_Fosc		11059200UL	//宏定义主时钟HZ

sbit CS = P3^7;
sbit DCLK = P2^1;
sbit DIN = 	P2^0;
sbit DOUT = P2^5;
sbit DU = P2^6;//数码管段选
sbit WE = P2^7;//数码管段选
sfr P4 = 0xe8;  /*C语言中声明P4口特殊功能寄存器地址*/
sbit DAC_DATA = P4^4;//DA数据口
unsigned char DAC_VAL;//占空比参数 模拟8位DA输出，取值范围0-255
unsigned char pwm_t;//周期
/*====================================
使用typedef给已有数据类型取别名
====================================*/
typedef unsigned char INT8U;
typedef unsigned char uchar;
typedef unsigned char u8;

typedef unsigned int INT16U;
typedef unsigned int uint;
typedef unsigned int u16;

uint voltage;//电压值

#define AD_CH0 0x94
#define AD_CH1 0xd4
#define AD_CH2 0xa4
#define AD_CH3 0xe4 
//通道0光敏cmd：0x94  通道1热敏cmd：0xd4  通道2电位器cmd：0xa4 通道3外部输入AIN3 cmd：0xe4

//共阴数码管段选表0-9
uchar code SMGduan[]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,};
//数码管位选码
uchar code SMGwei[] = {0xfe, 0xfd, 0xfb, 0xf7};

/*====================================
函数：void Delay_Ms(INT16U ms)
参数：ms，毫秒延时形参
描述：12T 51单片机自适应主时钟毫秒级延时函数
====================================*/
void Delay_Ms(INT16U ms)
{
     INT16U i;
	 do{
	      i = MAIN_Fosc / 96000; 
		  while(--i);   //96T per loop
     }while(--ms);
}

/*====================================
函数	：display(uchar i)
参数	：i 显示变量取值0-9999 
返回值	：无
描述	：数码管动态显示函数
====================================*/
void display(uint i)
{
	uchar q, b, s, g;
	static uchar wei;
	q = i / 1000;
	b = i % 1000 / 100;
	s = i % 100 / 10;
	g = i % 10;		
	P0 = 0XFF;//清除断码
	WE = 1;//打开位选锁存器
	P0 = SMGwei[wei];
	WE = 0;//锁存位选数据
	P0 = 0XFF;//清除断码
	switch(wei)
	{
		case 0: DU = 1; P0 = SMGduan[q] | 0x80;	DU = 0; break;//0x80加上小数点
		case 1: DU = 1; P0 = SMGduan[b]; 	DU = 0; break;	
		case 2: DU = 1; P0 = SMGduan[s]; 	DU = 0; break;
		case 3: DU = 1; P0 = SMGduan[g]; 	DU = 0; break;		
	}
	wei++;
	if(wei == 4)
		wei = 0;
}

/*====================================
函数	：SPI_Write(uchar DAT)
参数	：DAT需要发送的数据
返回值	：无
描述	：发送一个字节数据
====================================*/
void SPI_Write(uchar DAT)
{
	uchar i; 
	for(i=0; i<8; i++) //分别写8次，每次写1位
	{
		DCLK = 0;//拉低时钟总线，允许DIN变化
		if(DAT & 0x80)//先写数据最高位
			DIN = 1;  //写1
		else
			DIN = 0;  //写0
		DCLK = 1;	  //拉高时钟，让从机读DIN
		DAT <<= 1;	  //为发送下一位左移1位
	}
}
/*====================================
函数	：ReadByte()
参数	：无
返回值	：返回读出的数据
描述	：
====================================*/
uint SPI_Read()
{
	uchar i; 
	uint DAT;
	for(i=0; i<12; i++)//分别读12次，每次读一位
	{
		DAT <<= 1; //数据左移1位，准备接收一位
		DCLK = 1;   //拉高时钟总线，读取SDA上的数据
		DCLK = 0;   //拉低时钟总线，允许从机控制SDA变化
		if(DOUT)
			DAT |= 0X01;//为1则写1，否则不行执行写1，通过左移补0
	}
	return(DAT); //返回读出的数据
}

/*====================================
函数	：PCF8591Read(uchar cmd)
参数	：cmd XPT2046控制字节
返回值	：AD转出的数字量
描述	：读指定通道的输入的模拟量专为数字量
====================================*/
uint ReadAD(uchar cmd)
{
	uint DAT;
	CS = 0;
	SPI_Write(cmd);
	DCLK = 0;   //拉低时钟总线
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	DAT = SPI_Read();
	CS = 1;
	return(DAT);//返回读出数据
			
}

/************
 DA输出部分
*************/
//定时器0中断
void timer0() interrupt 1
{
	pwm_t++;//周期自加到255溢出后自动归零
	if(pwm_t <= DAC_VAL)// 设置占空比
		DAC_DATA = 1;
	else
		DAC_DATA = 0;			 
}
void main()//main函数自身会循环
{	
	uchar i;
	TMOD |= 0x02;//8位自动重装模块
	TH0 = 220;
	TL0 = 220;//11.0592M晶振下占空比最大比值是256,输出100HZ
	TR0 = 1;//启动定时器0
	ET0 = 1;//允许定时器0中断
	EA	= 1;//总中断允许
	while(1)
	{
		if(i >= 100)//延时500毫秒
		{
			i = 0;
			voltage = ReadAD(AD_CH0);	//通道0光敏cmd：0x94  通道1热敏cmd：0xd4  通道2电位器cmd：0xa4 通道3外部输入AIN3 cmd：0xe4
			voltage = voltage * 1.2207 ; //  （5/4096）
			DAC_VAL = voltage / 16;//12位AD值转8位DA输出
		}
		display(voltage);
		Delay_Ms(1);
		i++;
	}
} 