#include <REGX52.H>
#include <INTRINS.H>
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
	while(1)
	{
		P2=0xFE;
		 Delay500ms();
		P2=0x22;
		 Delay500ms();
		//从左到右记，灯第一个亮和3 ，4 ，5 ，7 ，8两组灯闪烁
	}
	
}