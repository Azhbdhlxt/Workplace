#include <reg52.h> //����51ͷ�ļ�

sbit LED2 = P1^1;
sbit LED3 = P1^2;
sbit LED5 = P1^4;

void main()
{
	LED2 = 0;  //����LED2
	LED3 = 0;  //����LED3
	LED5 = 0;  //����LED5		
}