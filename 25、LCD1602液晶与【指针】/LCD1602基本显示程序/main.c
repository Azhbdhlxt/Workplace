/**
LCD1602显示 12345
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
void main()
{
	Write_Cmd(0x38);//设置16*2显示
	Write_Cmd(0x0f);//开显示 显示光标，光标闪烁
	Write_Cmd(0x01);//清屏

	Write_Cmd(0x06);//地址指针移位命令
	Write_Cmd(0x80 | 0x06);//显示地址

	Write_Dat(1 + '0');
	Write_Dat(2 + '0');
	Write_Dat(3 + '0');
	Write_Dat(4 + '0');
	Write_Dat(5 + '0');
	while(1);
}