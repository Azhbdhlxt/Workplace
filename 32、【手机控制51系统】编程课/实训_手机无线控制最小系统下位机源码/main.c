#include <reg52.h> 

sbit LED1 = P1^0;
void UART_INIT()
{
	SM0 = 0;
	SM1 = 1;//���ڹ�����ʽ1
	REN = 1;//�����ڽ���
	EA = 1;//�����ж�
	ES = 1;//�������ж�
	TMOD = 0x20;//8λ�Զ���װģʽ
	TH1 = 0xfd;
	TL1 = 0xfd;//9600������
	TR1 = 1;//������ʱ��1
}
//�����ж�
void UART_SER() interrupt 4
{
	if(RI)
	{
		RI = 0;//������ձ�־
		switch(SBUF)
		{
			case 0x01: LED1 = 0; break;//����
			case 0x02: LED1 = 1; break;//�ص� 
		}
	}
}
void main()
{
	UART_INIT();//���ڳ�ʼ��
	while(1);	
}