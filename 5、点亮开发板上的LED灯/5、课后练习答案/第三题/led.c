#include <reg52.h> //引用51头文件

sbit LED2 = P1^1;
sbit LED3 = P1^2;
sbit LED5 = P1^4;

void main()
{
	LED2 = 0;  //点亮LED2
	LED3 = 0;  //点亮LED3
	LED5 = 0;  //点亮LED5		
}