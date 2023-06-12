/*********************************************************************************
* 【编写时间】： 2016年02月15日
* 【作    者】： 清翔电子:03
* 【版    本】： 1.0
* 【网    站】： http://www.qxmcu.com/ 
* 【淘宝店铺】： http://qxmcu.taobao.com/ 
* 【实验平台】： QX-MCS51 单片机开发板
* 【外部晶振】： 11.0592mhz	
* 【主控芯片】： STC89C52
* 【编译环境】： Keil μVisio3	
* 【程序功能】： LCD1602液晶循环显示DS1302 31个字节的静态RAM数据		   			            			    
* 【使用说明】： 试验前请用杜邦线分别把j5排针根据背面丝印相应接到P10、P11、P12
 				 并接上LCD1602液晶调节好对比度再下载此程序
				 【此程序是DS1302工作在突发模式读写31个RAM数据】

*  说明：免费开源，不提供源代码分析.
**********************************************************************************/

//调用1602显示驱动，在此文件内已包含51头文件和定义数据类型
#include "LCD1602.h"
/*====================================
 IO接口定义
====================================*/
sbit TSCLK = P1^0; //时钟
sbit TIO = P1^1;   //数据
sbit TRST = P1^2;  //使能

#define MAIN_Fosc		11059200UL	//宏定义主时钟HZ
#define DS1302_W_ADDR 0xfe  //RAM突发模式寄存器写
#define DS1302_R_ADDR 0xff	//RAM突发模式寄存器读

//31个字节RAM数据暂存数组
uchar TimeData[31]={0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
					0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
					0x19, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
					0x28, 0x29, 0x30, 0x31};

/*=================================================
*函数名称：Delay_Ms
*函数功能：12T 51单片机自适应主时钟毫秒级延时函数
*说明：需定义MAIN_Fosc（晶振主频单位HZ）
*输入：ms：要延时的毫秒
=================================================*/
void Delay_Ms(INT16U ms)
{
     INT16U i;
	 do{
	      i = MAIN_Fosc / 96000; 
		  while(--i);   //96T per loop
     }while(--ms);
}
/*=================================================
*函数名称：DS1302_W_Byte
*函数功能：DS1302写一字节数据
*输入：dat：要写入的数据
=================================================*/
void DS1302_W_Byte(uchar dat)
{
	uchar i;
	for(i = 0; i < 8; i++) //每次写1bit，写8次
	{
		TSCLK = 0;		   //拉低时钟总线
		TIO = dat & 0x01;  //从一字节最低位开始写
		TSCLK = 1;		   //拉高时钟总线，DS1302把数据读走
		dat >>= 1;		   //数据右移一位 
	}	
}
/*=================================================
*函数名称：DS1302_R_Byte
*函数功能：DS1302读一字节
*输出：dat：读取的数据
=================================================*/
uchar DS1302_R_Byte()
{
	uchar i, dat;
	for(i = 0; i < 8; i++)  //每次写1bit，写8次
	{
		TSCLK = 0;			//拉低时钟总线，DS1302把数据放到数据总线上
		dat >>= 1; 			//数据右移一位，数据从最低位开始读 
		if(TIO)	dat |= 0x80;//读取数据
		TSCLK = 1;			//拉高时钟总线
	}
	return dat;				//返回读取的数据
}
/*=================================================
*函数名称：DS1302_W_DAT
*函数功能：写DS1302数据一次写2个字节
*说明：先写命令后写数据
*调用：DS1302_W_Byte()
*输入：cmd：需要写的命令 ，dat：需要些的数据
=================================================*/
void DS1302_W_DAT(uchar cmd, uchar dat)
{
	TRST = 0;			 //拉低使能端
	TSCLK = 0;			 //拉低数据总线
	TRST = 1;			 //拉高使能端，开始写数据
	DS1302_W_Byte(cmd);	 //写命令
	DS1302_W_Byte(dat);	 //写数据
}
/*=================================================
*函数名称：DS1302_R_DAT
*函数功能：读DS1302数据
*说明：先写入命令字节后读出对应数据
*调用：	DS1302_W_Byte();DS1302_R_Byte();
*输入：	cmd：需要写的命令
*输出：	dat：读出的数据
=================================================*/
/*uchar DS1302_R_DAT(uchar cmd)	//此程序中未使用此函数
{
	uchar dat;
	TRST = 0;			 	//拉低使能端
	TSCLK = 0;				//拉低数据总线
	TRST = 1;				//拉高使能端，开始写数据
	DS1302_W_Byte(cmd);		//写命令
	dat = DS1302_R_Byte();	//读出数据
	return dat;				//返回读出数据
}*/

/*=================================================
*函数名称：DS1302_Clear_WP
*函数功能：清除DS1302写保护
*说明：先写入命令0x8e（写控制寄存器）接着向该寄存器写0
*调用：DS1302_W_DAT()
=================================================*/
void DS1302_Clear_WP()
{
	DS1302_W_DAT(0x8e,0x00);  //把控制寄存器WP位置0
}
/*=================================================
*函数名称：DS1302_Clear_WP
*函数功能：设置DS1302写保护
*说明：先写入命令0x8e（写控制寄存器）接着向该寄存器写0x80
*调用：DS1302_W_DAT()
=================================================*/
void DS1302_Set_WP()
{	
	DS1302_W_DAT(0x8e,0x80); //把控制寄存器WP位置1
	TRST = 0;				 //拉低使能端
	TSCLK = 0;				 //拉低数据总线
} 
/*=================================================
*函数名称：Set_DS1302_Time
*函数功能：写31个字节RAM数据
*说明：把TimeData数据31个字节用突发模式连续写入DS1302
		31个字节静态RAM中
*调用：DS1302_Clear_WP();DS1302_W_Byte();DS1302_Set_WP();
*输入：写RAM突发模式寄存器命令DS1302_W_ADDR
=================================================*/
void Set_DS1302_Time()
{
	uchar i;
	DS1302_Clear_WP();		//清除写保护
	TRST = 0;			 	//拉低使能端
	TSCLK = 0;				//拉低数据总线
	TRST = 1;				//拉高使能端，开始写数据
	DS1302_W_Byte(DS1302_W_ADDR); //写时钟突发模式寄存器
	for(i = 0; i < 31; i++)	//写31个字节的静态RAM数据
	{
	/*	j = TimeData[i]/10;	 //BCD码转换
		TimeData[i] %= 10;	 //BCD码转换
		TimeData[i] += j*16; //BCD码转换   */
		DS1302_W_Byte(TimeData[i]); //在突发模式下可以连续写数据
	}
	DS1302_Set_WP(); //开起写保护		
}
/*=================================================
*函数名称：Read_DS1302_Time
*函数功能：读31个字节RAM数据
*说明：用突发模式连续读取DS1302 31个字节的静态RAM数据
*调用：DS1302_Clear_WP();DS1302_R_Byte();DS1302_Set_WP();
*输入：写 读RAM突发模式寄存器命令DS1302_R_ADDR
=================================================*/
void Read_DS1302_Time()
{
	uchar i;
	DS1302_Clear_WP();    	//清楚些保护
	TRST = 0;			 	//拉低使能端
	TSCLK = 0;				//拉低数据总线
	TRST = 1;				//拉高使能端，开始写数据
	DS1302_W_Byte(DS1302_R_ADDR); //写 读突发模式寄存器命令
	for(i = 0; i < 31; i++)	//读31个字节的静态RAM数据
	{
		TimeData[i] = DS1302_R_Byte();//在突发模式下可以连续读数据
	}
	DS1302_Set_WP();   //开起写保护
} 
/*=================================================
*函数名称：main
*函数功能：主函数 1602液晶循环显示DS1302RAM数据
=================================================*/
void main()
{
	InitLcd1602();              	//1602初始化
	Set_DS1302_Time(); //RAM突发模式写
	Read_DS1302_Time();//RAM突发模式读
	while(1)
	{ 
		LcdShowStr(2,0,"RAMData(HEX)");  //1602显示字符串 RAM数据（十六进制）
		DisplayOneStr (0,1,TimeData[0]); //从第一个数据开始显示
		DisplayOneStr (3,1,TimeData[1]);
		DisplayOneStr (6,1,TimeData[2]);
		DisplayOneStr (9,1,TimeData[3]);
		DisplayOneStr (12,1,TimeData[4]);
		Delay_Ms(2000);					 //延时2秒
		Lcd1602_Write_Cmd(0x01);    //1602液晶清屏
		DisplayOneStr (0,0,TimeData[5]);
		DisplayOneStr (3,0,TimeData[6]);
		DisplayOneStr (6,0,TimeData[7]);
		DisplayOneStr (9,0,TimeData[8]);
		DisplayOneStr (12,0,TimeData[9]);
		DisplayOneStr (0,1,TimeData[10]);
		DisplayOneStr (3,1,TimeData[11]);
		DisplayOneStr (6,1,TimeData[12]);
		DisplayOneStr (9,1,TimeData[13]);
		DisplayOneStr (12,1,TimeData[14]);
		Delay_Ms(4000);					 //延时4秒
		Lcd1602_Write_Cmd(0x01);    //1602液晶清屏
		DisplayOneStr (0,0,TimeData[15]);
		DisplayOneStr (3,0,TimeData[16]);
		DisplayOneStr (6,0,TimeData[17]);
		DisplayOneStr (9,0,TimeData[18]);
		DisplayOneStr (12,0,TimeData[19]);
		DisplayOneStr (0,1,TimeData[20]);
		DisplayOneStr (3,1,TimeData[21]);
		DisplayOneStr (6,1,TimeData[22]);
		DisplayOneStr (9,1,TimeData[23]);
		DisplayOneStr (12,1,TimeData[24]);
		Delay_Ms(4000);					 //延时4秒
		Lcd1602_Write_Cmd(0x01);    //1602液晶清屏
		DisplayOneStr (0,0,TimeData[25]);
		DisplayOneStr (3,0,TimeData[26]);
		DisplayOneStr (6,0,TimeData[27]);
		DisplayOneStr (9,0,TimeData[28]);
		DisplayOneStr (12,0,TimeData[29]);
		DisplayOneStr (0,1,TimeData[30]);
		Delay_Ms(5000);					 //延时5秒
		Lcd1602_Write_Cmd(0x01);    //1602液晶清屏
	}		
}
