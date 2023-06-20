#include <REGX51.H>
#include <INTRINS.H>
#include <stdio.h>
#include <stdlib.h>
void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 8;
	j = 1;
	k = 243;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void main()
{
	P2=0xFE;
	Delay1000ms();
	P2=0x22;
	Delay1000ms();
}