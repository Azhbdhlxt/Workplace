#include <reg52.h> //引用51头文件

sbit LED5 = P1^4;

void main()
{
	LED5 = 0;//点亮LED5		
}