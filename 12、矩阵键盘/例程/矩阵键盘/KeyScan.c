/*********************************************************************************
* ����    �ߡ���	�������:����
* ����    ������	V1.0
* ����    վ����	http://www.qxmcu.com/ 
* ���Ա����̡���	http://qxmcu.taobao.com/
* ��ʵ��ƽ̨����	���� QX-MCS51 ��Ƭ��������
* ���ⲿ���񡿣� 	11.0592mhz	
* ������оƬ���� 	STC89C52
* �����뻷������ 	Keil ��Visio4	
* �������ܡ��� 	4*4���������4λ��������ʶ��		   			            			    
* ��ʹ��˵������ 	���¾�����̺Ͷ���������������������ʾ��Ӧ��ֵ
					��ʼ��ʾ��-����
**********************************************************************************/
#include <reg52.h>//����51ͷ�ļ�
#include <intrins.h>//������λ��׼�⺯��ͷ�ļ�

#define uint unsigned int
#define uchar unsigned char

sbit DU = P2^6;//����ܶ�ѡ
sbit WE = P2^7;//����ܶ�ѡ
uchar num;//�������ʾ��ֵ
uchar KeyValue = 20;//����ֵ ��ʾ-

//��������ܶ�ѡ��
uchar  code tabel[]= {
//0		1	 2     3     4     5     6     7     8
0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F,
//9     A     B	   C	 D	   E	 F		H	 L	 
0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x76, 0x38,
//n	   u	  -	  Ϩ��
0x37, 0x3E, 0x40, 0x00 };

/*====================================
����	�� delay(uint z)
����	��z ��ʱ�����趨��ȡֵ��Χ0-65535
����ֵ	����
����	��12T/Fosc11.0592M���뼶��ʱ
====================================*/
void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--); 		
} 
/*====================================
����	��KeyScan()
����	����
����ֵ	����
����	��4*4����������������ɨ��
��������KeyValueȫ�ֱ���ֵ������Ӧ�仯
====================================*/
void KeyScan()
{
	//4*4�������ɨ��
	P3 = 0XF0;//��ɨ��
	if(P3 != 0XF0)//�жϰ����Ƿ񱻰���
	{
		delay(10);//�������10ms
		if(P3 != 0XF0)//�жϰ����Ƿ񱻰���
		{
			switch(P3) //�ж���һ�б�����
			{
				case 0xe0:	KeyValue = 0;	break;//��һ�б�����
				case 0xd0:	KeyValue = 1;	break;//�ڶ��б�����
				case 0xb0:	KeyValue = 2;	break;//�����б�����
				case 0x70:	KeyValue = 3;	break;//�����б����� 
			}
			P3 = 0X0F;//��ɨ��
			switch(P3) //�ж���һ�б�����
			{
				case 0x0e:	KeyValue = KeyValue;	break;//��һ�б�����
				case 0x0d:	KeyValue = KeyValue + 4;	break;//�ڶ��б�����
				case 0x0b:	KeyValue = KeyValue + 8;	break;//�����б�����
				case 0x07:	KeyValue = KeyValue + 12;	break;//�����б����� 
			}
			while(P3 != 0X0F);//���ּ��	
		}
	}
	P3 = 0XFF;//��������ɨ��
	if(P3 != 0XFF)
	{
		delay(10);//�������10ms
		if(P3 != 0XFF)
		{
			switch(P3) //�ж���һ�б�����
			{
				case 0xfe:	KeyValue = 16;	break;//S2������
				case 0xfd:	KeyValue = 17;	break;//S3������
				case 0xfb:	KeyValue = 18;	break;//S4������
				case 0xf7:	KeyValue = 19;	break;//S5������ 
			}
			while(P3 != 0XFF);//���ּ��			
		}	
	}

}

void main()//main���������ѭ��
{
	WE = 1;//��λѡ������
	P0 = 0XFE; //1111 1110
	WE = 0;//����λѡ����

	DU = 1;//�򿪶�ѡ������
	while(1)
	{
		KeyScan();//20����������ɨ��
		P0 = tabel[KeyValue];//��ʾ����ֵ
	}	
}  