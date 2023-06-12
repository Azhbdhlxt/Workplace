#include <reg52.h>
#include <intrins.h>
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
sbit DU  = P2^6;   //����ܶ�ѡ
sbit WE  = P2^7;   //�����λѡ
/*====================================
����������ܶ�ѡ��
====================================*/
uchar code table[]={ 
//0		1	 2     3     4     5     6     7     8
0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F,
//9     A     B	   C	 D	   E	 F		-	 .	  ����ʾ
0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x40, 0x80, 0x00
                   };

/*====================================
�����λѡ��
====================================*/
				  //��1λ	2λ	  3λ	 4λ   5λ	6λ	  7λ	8λ
uchar code T_COM[] = {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};//�����λ��

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
/*====================================
������void Display(INT16U Value)
������Value,��ʾֵ ȡֵ0-65535
�������������������ʾ��������ʾһ���ֽڵ���
====================================*/
void Display(INT16U Value)			//ע��������Ҫ��ʾ��������һ���ֽ������β���Ϊint��
{	
//------------------------------
	DU = 0;							//�رն�ѡ
	P0 = table[Value/100];		//�������ʾ��λ
	DU = 1;							//�򿪶�ѡ
	DU = 0;							//�رն�ѡ

	WE = 0;						//�ر�λѡ
	P0 = T_COM[0];				   //��һλ�����
	WE = 1;						//��λѡ
	WE = 0;						//�ر�λѡ
	Delay_Ms(3);
//-------------------------------
	DU = 0;
	P0 = table[Value%100/10]|0x80; //��ʾʮλ
	DU = 1;
	DU = 0;

	WE = 0;
	P0 = T_COM[1];			  //�ڶ�λ�����
	WE = 1;
	WE = 0;
	Delay_Ms(3);
//-------------------------------
	DU = 0;
	P0 = table[Value%10];		//��ʾ��λ
	DU = 1;
	DU = 0;

	WE = 0;
	P0 = T_COM[2];				//����λ�����
	WE = 1;
	WE = 0;
	Delay_Ms(3);
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
void main()
{
	uint i;
	uchar L, M;
/*	ds_init();//��ʼ��DS18B20
	write_byte(0xcc);//������ԾROMָ��
	write_byte(0x4e);//д�ݴ���ָ��
	write_byte(0x7f);
	write_byte(0xf7);
	write_byte(0x1f);//���ù�����9λģʽ��
	ds_init();//��ʼ��DS18B20
	write_byte(0xcc);//������ԾROMָ�� 
	write_byte(0x48);*/
	while(1)
	{
		ds_init();//��ʼ��DS18B20
		write_byte(0xcc);//������ԾROMָ��
		write_byte(0x44);//�����¶�ת��ָ��
		ds_init();//��ʼ��DS18B20
		write_byte(0xcc);//������ԾROMָ��
		write_byte(0xbe);//��ȡDS18B20�ݴ���ֵ
		L = read_byte();
		M = read_byte();
		i = M;
		i <<= 8;
		i |= L;						
		i = i * 0.0625 * 10 + 0.5;
		Display(i);
	}
}