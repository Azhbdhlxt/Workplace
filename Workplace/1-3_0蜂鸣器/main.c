#include <REGX51.H>
#include <INTRINS.H>
#include <Delay.h>
typedef unsigned int u16;
typedef unsigned char u8;	


sbit BEEP =P2^5;

void main()
{
   u16 i=2000;//音长
	while (1)
    {
		while (i--)
        {
//			BEEP=!BEEP;
//			BEEP=0;
//			delay_10us(500);			//改变声
//			BEEP=1;               
//			delay_10us(20);//音色
//			delay_10us(15000); 
//			BEEP=0;
//			delay_10us(400);			//改变声
//			BEEP=1;               
//			delay_10us(1000);
//			BEEP=!BEEP;
//			delay_10us(5000);//音色
			BEEP=!BEEP;
			delay_10us(200);//改变音调
			BEEP=0;
			delay_10us(10);//声音大小
			BEEP=1;
			delay_10us(190);//声音大小
			//delay_10us(2500);
			BEEP=!BEEP;
			delay_10us(200);//改变音调
			BEEP=0;
			delay_10us(10);//声音大小
			BEEP=1;
			delay_10us(90);//声音大小
        }
		i=0;
		BEEP=0; 
    }    

}