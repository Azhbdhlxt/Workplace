#include <REGX51.H>
#include <INTRINS.h>
/*void Delay1000us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}


*/
void main(int argc[],char*argv)
{
	 
	// Delay1000us();
	P2=0xCC;
	//Delay1000us();
	P2=0xFE;
}
