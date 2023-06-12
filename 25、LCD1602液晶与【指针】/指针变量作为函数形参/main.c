/**
ָ�������Ϊ�����β�
**/
#include <reg52.h>

typedef unsigned char uchar;
typedef unsigned int uint;

sbit RS = P3^5;
sbit RW = P3^6;
sbit EN = P3^4;
//�ж�Һ��æ�����æ��ȴ�
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
//дLCD1602����һ���ֽ�
void Write_Cmd(uchar cmd)
{
	Read_Busy();//�ж�æ
	RS = 0;
	RW = 0;
	P0 = cmd;
	EN = 1;
	EN = 0;
}
//дһ���ֽ�����
void Write_Dat(uchar dat)
{
	Read_Busy();
	RS = 1;
	RW = 0;
	P0 = dat;
	EN = 1;
	EN = 0;
}
//�Ƚϴ�С
void max(uchar a, uchar b, uchar *max)
{
	*max = (a>b)? a:b;
}
void main()
{
	uchar i;
	Write_Cmd(0x38);//����16*2��ʾ
	Write_Cmd(0x0f);//����ʾ ��ʾ��꣬�����˸
	Write_Cmd(0x01);//����

	Write_Cmd(0x06);//��ַָ����λ����
	Write_Cmd(0x80 | 0x00);//��ʾ��ַ
	
	max(3, 7, &i);
	Write_Dat(i + '0');
	while(1);
}