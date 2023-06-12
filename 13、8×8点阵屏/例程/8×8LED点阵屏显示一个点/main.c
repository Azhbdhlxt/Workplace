/*********************************************************************************
* 【作    者】：	清翔电子:向量
* 【版    本】：	V1.0
* 【网    站】：	http://www.qxmcu.com/ 
* 【淘宝店铺】：	http://qxmcu.taobao.com/
* 【实验平台】：	清翔 QX-MCS51 单片机开发板
* 【外部晶振】： 	11.0592mhz	
* 【主控芯片】： 	STC89C52
* 【编译环境】： 	Keil μVisio4	
* 【程序功能】： 	8*8点阵显示1个点		   			            			    
* 【使用说明】： 	正确安装好点阵模块
**********************************************************************************/
#include <reg52.h>
sbit DIO  = P3^4; //串行数据口
sbit S_CLK = P3^5;//移位寄存器时钟
sbit R_CLK = P3^6;//输出锁存器时钟
void main()
{
	//ROW 0x80 , COL 0xfe
	unsigned char i, dat;//i循环次数  dat需要发送的数据
	dat = 0xfe;//发送列值
	for(i=0; i<8; i++)//循环8次
	{
		S_CLK = 0;//拉低移位寄存器时钟
		R_CLK = 0;//拉低输出锁存器时钟
		if(dat & 0x01)//发送1
			DIO = 1;
		else		  //发送0
			DIO = 0;
		S_CLK = 1;//拉高移位寄存器时钟，数据移位
		dat >>= 1;//数据右移
	}
	dat = 0x80;//发送行值
	for(i=0; i<8; i++)//循环8次
	{
		S_CLK = 0;//拉低移位寄存器时钟
		R_CLK = 0;//拉低输出锁存器时钟
		if(dat & 0x01)//发送1
			DIO = 1;
		else		  //发送0
			DIO = 0;
		S_CLK = 1;//拉高移位寄存器时钟，数据移位
		dat >>= 1;//数据右移
	}
	R_CLK = 1;//拉高输出锁存器时钟，并行输出数据
	while(1);
}