/*
****************�ṹ��ָ���������������************************
�򿪴��ں����������Ϊ9600
����ģʽΪ�ı�ģʽ
*/
#include <reg52.h>
#include <stdio.h>

struct date{ //����ṹ������
	unsigned int year;
	unsigned char month, day;	
};
/*���뼶��ʱ����*/
void Delay_Ms(unsigned int z)
{
	unsigned int x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}	
/*���ڳ�ʼ����������9600*/
void UART_Init()
{
	TMOD |= 0x20;  	//T1����ģʽ2  8λ�Զ���װ
	TH1 = 0xfd;
	TL1 = 0xfd; 	//������9600
	TR1 = 1;		//����T1��ʱ��
	SM0 = 0;
	SM1 = 1; 		//���ڹ�����ʽ1 10λ�첽
}

void main()
{
	struct date d1, d2;//����ṹ�����
	struct date *pd1;//����ṹ��ָ�����
	UART_Init(); //���ڳ�ʼ��
	TI = 1;//ʹ��printf()puts()ǰ��Ӧ�����λTI = 1��
	pd1 = &d1;//�ṹ��ָ�������ֵ,ָ��ṹ�����d1
	pd1->year = 2017;//ͨ���ṹ��ָ�������ýṹ��Ԫ��
	pd1->month = 5;
	pd1->day = 13;

	d2.year = 2018;//ͨ���ṹ��ָ�������ýṹ��Ԫ��
	d2.month = 8;
	d2.day = 8;
	while(1)
	{
		pd1 = &d1;//ָ��ṹ�����d1
		/*��Keil C51����printf���һ�����ֽ�����ʱҪʹ��%bd,���ʮ��λ����Ҫ��%hd�������ʮ��λ����%ld*/
		printf(" year: %hd \n month: %bd \n day: %bd \n", pd1->year, pd1->month, pd1->day);
		pd1++;//ָ����һ���ṹ�����
		printf(" year: %hd  month: %bd  day: %bd \n", pd1->year, pd1->month, pd1->day);
		Delay_Ms(5000);//5�뷢��һ��
	}	
}
/*************************************
��ע��
ʹ��printf()puts()ǰ��Ӧ�����λTI = 1��
printf()puts()  ʹ��putchar���������ֽ�

void putchar(uchar sbyte )
{
    while(��TI);   //�ȴ�������

    SBUF=sbyte;

}

���ڵ���putcharǰûTI=1 ��Զ�ȴ�
*************************************/