#include <REGX51.H>
#define unit unsigned int
sbit led1 =P1^0;//声明单片机P1口的第一位
unit i, j;
void main()
{
	while(1)
	{
		led1=0;               //点亮第一个LED
		for(i=1000;i>0;i--)   //延时
			for(j=110;j>0;j--);
		led1=1;
		for(i=1000;i>0;i--)		//熄灭LED
		for(j=110;j>0;j--);		//延时
	}
	
}