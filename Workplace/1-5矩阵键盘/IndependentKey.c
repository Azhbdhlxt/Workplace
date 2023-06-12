#include <REGX51.H>

#include "Delay.h"
unsigned char KeyNum;
unsigned char IndependentKey()
{
	P2=0xFF;
	
	/*
	P3=~0x01;//默认高电平
	do{if (P3_1==0){Delay(20);while (P3_1==0);
  Delay(20);KeyNum=17;}
	if (P3_0==0){Delay(20);while (P3_0==0);
  Delay(20);KeyNum=18;}
	if (P3_2==0){Delay(20);while (P3_2==0);
  Delay(20);KeyNum=19;}
	if (P3_3==0){Delay(20);while (P3_3==0);
  Delay(20);KeyNum=20;}
}while(1);*/
	
	while(1)
	{
		if(P3_1==0)			//??K1????
		{
			Delay(20);
			while(P3_1==0);
			Delay(20);
			KeyNum=17;
			
		/*
      	LEDNum++;		//LEDNum??
			if(LEDNum>=8)	//??LEDNum????
				LEDNum=0;
			P2=~(0x01<<LEDNum);	//LED??LEDNum???
      */	
		}
		if(P3_0==0)			//??K2????
		{
			Delay(20);
			while(P3_0==0);
			Delay(20);
			KeyNum=18;
			
		/*
      	if(LEDNum==0)	//LEDNum??0???7
				LEDNum=7;
			else			//LEDNum???0,??
				LEDNum--;
			P2=~(0x01<<LEDNum);	//LED??LEDNum???
      */	
		}
		if(P3_2==0)			//??K1????
		{
			Delay(20);
			while(P3_2==0);
			Delay(20);
			KeyNum=19;
		}
		if(P3_3==0)			//??K1????
		{
			Delay(20);
			while(P3_3==0);
			Delay(20);
			KeyNum=20;
		}
	}
	return KeyNum;
}
