#include <REGX51.H>
#include <INTRINS.H>
#include <Delay.h>
typedef unsigned int u16;
typedef unsigned char u8;	


sbit BEEP =P2^5;

void main()
{
   u16 i=2000;//����
	while (1)
    {
		while (i--)
        {
//			BEEP=!BEEP;
//			BEEP=0;
//			delay_10us(500);			//�ı���
//			BEEP=1;               
//			delay_10us(20);//��ɫ
//			delay_10us(15000); 
//			BEEP=0;
//			delay_10us(400);			//�ı���
//			BEEP=1;               
//			delay_10us(1000);
//			BEEP=!BEEP;
//			delay_10us(5000);//��ɫ
			BEEP=!BEEP;
			delay_10us(200);//�ı�����
			BEEP=0;
			delay_10us(10);//������С
			BEEP=1;
			delay_10us(190);//������С
			//delay_10us(2500);
			BEEP=!BEEP;
			delay_10us(200);//�ı�����
			BEEP=0;
			delay_10us(10);//������С
			BEEP=1;
			delay_10us(90);//������С
        }
		i=0;
		BEEP=0; 
    }    

}