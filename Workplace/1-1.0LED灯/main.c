#include <REGX51.H>
#include <INTRINS.h>
#define LED_PORT	P2
typedef unsigned int u16;
sbit LED1=P2^0;

//void Delay500ms()		//@12.000MHz
//{
//	unsigned char i, j, k;

//	_nop_();
//	i = 4;
//	j = 205;
//	k = 187;
//	do
//	{
//		do
//		{
//			while (--k);
//		} while (--j);
//	} while (--i);
//}
Delay500ms(u16 ten_us )
{
	while( ten_us--);
	
}

void main()
{
	u16 i=0;
	LED_PORT=~0x01;
	while(1)
	{
	 for(i=0;i<7;i++)
		{
////			LED_PORT=~(0xFE);
////			LED_PORT+=i;        //自增流水灯
////		Delay500ms(50000);
			LED_PORT=_crol_(LED_PORT,1);
			Delay500ms(50000);
			for(i=0;i<7;i++)
		{
////			LED_PORT=~(0xFE);
////			LED_PORT+=i;
////		Delay500ms(50000);
			LED_PORT=_cror_(LED_PORT,1);
			Delay500ms(50000);
		}
		}
	}
}
