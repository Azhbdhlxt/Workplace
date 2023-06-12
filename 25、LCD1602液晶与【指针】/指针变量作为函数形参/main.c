/**
指针变量作为函数形参
**/
#include <reg52.h>

typedef unsigned char uchar;
typedef unsigned int uint;

sbit RS = P3^5;
sbit RW = P3^6;
sbit EN = P3^4;
//判断液晶忙，如果忙则等待
void Read_Busy()
{
	uchar busy;
	P0 = 0xff;
	RS = 0;
	RW = 1;
	do
	{
		EN = 1;
		busy = P0;
		EN = 0;
	}while(busy & 0x80);
}
//写LCD1602命令一个字节
void Write_Cmd(uchar cmd)
{
	Read_Busy();//判断忙
	RS = 0;
	RW = 0;
	P0 = cmd;
	EN = 1;
	EN = 0;
}
//写一个字节数据
void Write_Dat(uchar dat)
{
	Read_Busy();
	RS = 1;
	RW = 0;
	P0 = dat;
	EN = 1;
	EN = 0;
}
//比较大小
void max(uchar a, uchar b, uchar *max)
{
	*max = (a>b)? a:b;
}
void main()
{
	uchar i;
	Write_Cmd(0x38);//设置16*2显示
	Write_Cmd(0x0f);//开显示 显示光标，光标闪烁
	Write_Cmd(0x01);//清屏

	Write_Cmd(0x06);//地址指针移位命令
	Write_Cmd(0x80 | 0x00);//显示地址
	
	max(3, 7, &i);
	Write_Dat(i + '0');
	while(1);
}