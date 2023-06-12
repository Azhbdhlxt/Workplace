/*********************************************************************************
* 【作    者】：	清翔电子:向量
* 【版    本】：	V1.0
* 【网    站】：	http://www.qxmcu.com/ 
* 【淘宝店铺】：	http://qxmcu.taobao.com/
* 【实验平台】：	清翔 QX-MCS51 单片机开发板
* 【外部晶振】： 	11.0592mhz	
* 【主控芯片】： 	STC89C52
* 【编译环境】： 	Keil μVisio4	
* 【程序功能】： 	IIC通信，AT24C02读写数据，数码管显示数据。		   			            			    
* 【使用说明】： 
**********************************************************************************/
#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char
#define At24c02ADDR 0XA0   //AT24C02硬件地址
#define	I2cRead 1		   //I2C读方向位
#define	I2cWrite 0		   //I2C写方向位

sbit DU = P2^6;//数码管段选
sbit WE = P2^7;//数码管段选
sbit SCL = P2^1;//I2C时钟总线
sbit SDA = P2^0;//I2C数据总线
uchar num;//数码管显示的值
bit AckFlag;//应答标志位

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
	TMOD |= 0X01; //定时器工作模式1，16位定时模式
	TH0 = 0xED;
	TL0 = 0xFF; //定时5ms
}
/****************************************************
IIC通信代码
****************************************************/

/*====================================
函数	：delay5us()
参数	：无
返回值	：无
描述	：5us延时函数
====================================*/
void delay5us()
{
	_nop_();
}

/*====================================
函数	：I2cStart()
参数	：无
返回值	：无
描述	：I2C总线起始信号
====================================*/
void I2cStart()
{
//时钟总线为高电平期间数据总线又高变低产生起始型号
	SCL = 1; 
	SDA = 1;
	delay5us();//状态保持5us
	SDA = 0;
	delay5us();//状态保持5us 
}

/*====================================
函数	：I2cStop()
参数	：无
返回值	：无
描述	：I2C总线停止信号
====================================*/
void I2cStop()
{
//时钟总线为高电平期间，数据总线从高变低产生终止信号
	SCL = 0;
	SDA = 0;
	SCL = 1;
	delay5us();//状态保持5us
	SDA = 1;
	delay5us();//状态保持5us	
}

/*====================================
函数	：ReadACK()
参数	：无
返回值	：1非应答，0应答
描述	：I2C总线读从机应答信号
====================================*/
bit ReadACK()
{
	SCL = 0;//拉低时钟总线，允许从机控制SDA
	SCL = 1;//拉高，读SDA
	delay5us();
	if(SDA)//NOACK
	{
		SCL = 0;
		return(1);//返回1
	}
	else//ACK
	{
		SCL = 0;
		return(0);//返回0
	}
}

/*====================================
函数	：SendACK(bit i)
参数	：1主机发送非应答，0发送应答
返回值	：无
描述	：主机发送应答信号
====================================*/
void SendACK(bit i)
{
	SCL = 0;//拉低时钟总线，允许主机控制SDA
	if(i)	//发非应答
		SDA = 1;
	else	//发应答
		SDA = 0;
	SCL = 1;  //拉高总线，让从机读SDA
	delay5us();//保持5us
	SCL = 0; //拉低时钟总线，允许SDA释放
	SDA = 1;//释放数据总线
}

/*====================================
函数	：I2cSendByte(uchar DAT)
参数	：DAT需要发送的数据
返回值	：无
描述	：I2C发送一个字节数据
====================================*/
void I2cSendByte(uchar DAT)
{
	uchar i; 
	for(i=0; i<8; i++) //分别写8次，每次写1位
	{
		SCL = 0;//拉低时钟总线，允许SDA变化
		if(DAT & 0x80)//先写数据最高位
			SDA = 1;  //写1
		else
			SDA = 0;  //写0
		SCL = 1;	  //拉高时钟，让从机读SDA
		DAT <<= 1;	  //为发送下一位左移1位
	}
	SCL = 0; //拉低时钟总线，允许SDA释放
	SDA = 1;//释放数据总线
}

/*====================================
函数	：At24c02Write(uchar ADDR, DAT)
参数	：ADDR 单元地址0-255，DAT 需要输入的数据0-255
返回值	：无
描述	：At24c02指定单元写入一个字节数据
====================================*/
void At24c02Write(uchar ADDR, DAT)
{
	I2cStart();//I2C起始信号
	I2cSendByte(At24c02ADDR + I2cWrite);//发送器件地址加读写方向位
	if(ReadACK()) //读从机应答
		AckFlag = 1;	//NOACK
	else
		AckFlag = 0;	//ACK
	I2cSendByte(ADDR);//发送储存单元地址字节
	if(ReadACK())//读从机应答
		AckFlag = 1;	//NOACK
	else
		AckFlag = 0;	//ACK
	I2cSendByte(DAT);//发送一字节数据
	if(ReadACK())//读从机应答
		AckFlag = 1;	//NOACK
	else
		AckFlag = 0;	//ACK
	I2cStop();	//I2C停止信号
}

/*====================================
函数	：I2cReadByte()
参数	：无
返回值	：返回读出的一字节数据
描述	：I2C总线读一字节数据
====================================*/
uchar I2cReadByte()
{
	uchar i, DAT;
	for(i=0; i<8; i++)//分别读8次，每次读一位
	{
		DAT <<= 1; //数据左移1位，准备接收一位
		SCL = 0;   //拉低时钟总线，允许从机控制SDA变化
		SCL = 1;   //拉高时钟总线，读取SDA上的数据
		if(SDA)
			DAT |= 0X01;//为1则写1，否则不行执行写1，通过左移补0
	}
	return(DAT); //返回读出的数据
}

/*====================================
函数	：At24c02Read(uchar ADDR)
参数	：ADDR 单元地址	0-255
返回值	：返回指定单元的数据
描述	：读AT24C02指定单元内数据
====================================*/
uchar At24c02Read(uchar ADDR)
{
	uchar DAT;
	I2cStart();//I2C起始信号
	I2cSendByte(At24c02ADDR + I2cWrite);//发送器件地址加读写方向位
	if(ReadACK())//读从机应答
		AckFlag = 1;	//NOACK
	else
		AckFlag = 0;	//ACK
	I2cSendByte(ADDR);//I2C发送一个字节
	ReadACK();//读从机应答
	I2cStart();//再次产生I2C起始信号
	I2cSendByte(At24c02ADDR + I2cRead);//发送器件地址加读写方向位 读
	if(ReadACK())//读从机应答
		AckFlag = 1;	//NOACK
	else
		AckFlag = 0;	//ACK
	DAT = I2cReadByte();//读一字节
	SendACK(1);//主机发送非应答
	I2cStop(); //I2C停止信号
	return(DAT);//返回读出数据
			
}

void main()//main函数自身会循环
{	
	timer0Init();//定时器0初始化
	EA = 0;//屏蔽中断
	At24c02Write(3, 188);//给第3单元写入数据“188”
	delay(5);//延时等待AT24C02处理
	num = At24c02Read(3);//读出第3单元内数据送给显示变量
	if(AckFlag)//当从机非应答
		P1 = 0;//亮P1所有灯
	else
		P1 = 0XFF;//灭P1所有灯
	EA = 1;//开中断
	while(1);
} 

//定时器0中断函数
void timer0() interrupt 1
{
	TH0 = 0xED;
	TL0 = 0xFF; //定时5ms
	display(num); //数码管显示函数	
} 