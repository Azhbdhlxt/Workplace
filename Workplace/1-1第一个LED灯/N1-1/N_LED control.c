#include <REGX51.H>
#include <INTRINS.h>
sbit LED1=P2^0;
void Delay500ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 4;
	j = 205;
	k = 187;
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
	
//	while(1)
//	{
	  LED1=0;
		Delay500ms();	
		LED1=0;
		Delay500ms();	 
		LED1=0;
		Delay500ms();	
		
//	}
}
