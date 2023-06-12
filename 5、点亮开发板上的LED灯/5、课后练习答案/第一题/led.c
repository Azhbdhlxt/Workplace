#include <reg52.h> //引用51头文件

sbit LED2 = P1^1;

void main()
{
	LED2 = 0;	//点亮LED2	
}