/*
****************������ʾDS18B20�¶�ֵ************************
DS18B20���ݽӿ�ΪP2.2
�򿪴��ں����������Ϊ9600
����ģʽΪ�ı�ģʽ
*/
#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#define MAIN_Fosc		11059200UL	//�궨����ʱ��HZ
/*====================================
 �Զ���������
====================================*/
typedef unsigned char INT8U;
typedef unsigned char uchar;

typedef unsigned int INT16U;
typedef unsigned int uint;

/*====================================
 Ӳ���ӿ�λ����
====================================*/
sbit DS  = P2^2;   //DS18B20������


/*====================================
������void Delay_Ms(INT16U ms)
������ms��������ʱ�β�
������12T 51��Ƭ������Ӧ��ʱ�Ӻ��뼶��ʱ����
====================================*/
void Delay_Ms(INT16U ms)
{
     INT16U i;
	 do{
	      i = MAIN_Fosc / 96000; 
		  while(--i);   //96T per loop
     }while(--ms);
}
/*us��ʱ������ִ��һ��US--����6.5us����һ�κ�����Ҫ11.95us*/
void Delay_us(uchar us)
{
	while(us--);	
}

/*�����߳�ʼ��ʱ��*/
bit ds_init()
{
	bit i;
	DS = 1;
	_nop_();
	DS = 0;
	Delay_us(75); //��������499.45us �ҽ��������ϵ�18B20����ȫ������λ
	DS = 1; //�ͷ�����
	Delay_us(4); //��ʱ37.95us �ȴ�18B20���ش����ź�
	i = DS;
	Delay_us(20); //141.95us
	DS = 1;
	_nop_();
	return (i);
}
/*дһ���ֽ�*/
void write_byte(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		DS = 0;
		_nop_();//����Щʱ��
		DS = dat & 0x01;
		Delay_us(10);//76.95us
		DS = 1; //�ͷ�����׼����һ������д��
		_nop_();
		dat >>= 1;
	}
}

uchar read_byte()
{
	uchar i, j, dat;
	for(i=0;i<8;i++)
	{
		DS = 0;
		_nop_();//������ʱ��
		DS = 1;
		_nop_();//�ͷ�����
		j = DS;
		Delay_us(10);//76.95us
		DS = 1;
		_nop_();
		dat = (j<<7)|(dat>>1);	
	}
	return (dat);
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

union _Temp{  //���������Ͷ���
	uchar c[2];	//����c[0]�¶ȵĸ��ֽڣ�c[1]����¶ȵĵ��ֽ�
	uint x;	//x��������DS18B20���¶�ֵ
};

void main()
{
	union _Temp temp;//�������������
	float i;
	UART_Init(); //���ڳ�ʼ��
	TI = 1;//ʹ��printf()puts()ǰ��Ӧ�����λTI = 1��

	ds_init();//��ʼ��DS18B20
	write_byte(0xcc);//������ԾROMָ��
	write_byte(0x44);//�����¶�ת��ָ��
	Delay_Ms(1000);
	while(1)
	{
		ds_init();//��ʼ��DS18B20
		write_byte(0xcc);//������ԾROMָ��
		write_byte(0x44);//�����¶�ת��ָ��
		ds_init();//��ʼ��DS18B20
		write_byte(0xcc);//������ԾROMָ��
		write_byte(0xbe);//��ȡDS18B20�ݴ���ֵ
		temp.c[1] = read_byte();//�¶ȵ��ֽ�
		temp.c[0] = read_byte();//�¶ȸ��ֽ�						
		i = temp.x * 0.0625;//��ԭС��
		printf("�¶�: %4.2f", i); //����С�������λ���
		printf("�� \n");
		Delay_Ms(5000);//5�뷢��һ��
	}
}