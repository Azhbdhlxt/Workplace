#include <REGX51.H>
#define unit unsigned int
sbit led1 =P1^0;//������Ƭ��P1�ڵĵ�һλ
unit i, j;
void main()
{
	while(1)
	{
		led1=0;               //������һ��LED
		for(i=1000;i>0;i--)   //��ʱ
			for(j=110;j>0;j--);
		led1=1;
		for(i=1000;i>0;i--)		//Ϩ��LED
		for(j=110;j>0;j--);		//��ʱ
	}
	
}