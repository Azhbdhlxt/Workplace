/*
****************ö�ٱ����Ķ���������************************
�򿪴��ں����������Ϊ9600
����ģʽΪ�ı�ģʽ
*/
#include <reg52.h>
#include <stdio.h>

enum eStaSystem {  //ö�����Ͷ���
    DIS_DATE, DIS_TIME
};
//ϵͳ����״̬
enum eStaSystem staSystem = DIS_TIME;  //ö�ٱ��������븳ֵ
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
	UART_Init(); //���ڳ�ʼ��
	TI = 1;//ʹ��printf()puts()ǰ��Ӧ�����λTI = 1��
	while(1)
	{
		if (staSystem == DIS_DATE)
		{
			printf("���ڣ�2017-8-8 \n");
		}
		if (staSystem == DIS_TIME)
		{
			printf("ʱ�䣺08:30:50 \n");
		}
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