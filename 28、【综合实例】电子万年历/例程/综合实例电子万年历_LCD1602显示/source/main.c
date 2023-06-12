/*********************************************************************************
* ��ʵ��ƽ̨���� QX-MCS51 ��Ƭ��������
* ���ⲿ���񡿣� 11.0592mhz	
* ������оƬ���� STC89C52
* �����뻷������ Keil ��Visio4	
* �������ܡ��� ����������,LCD1602Һ����ʾ����ʾʱ���롢���ڡ����������������ӡ������¶ȡ�����Уʱ���������ӡ�			   			            			    
* ��ʹ��˵������ ʹ��3���Ű��߰�P10�ӵ�DS1302��TSCLK�ϣ�P11�ӵ�TIO��P12�ӵ�TRST�ϡ�
				 ����LCD1602Һ����
* ����������˵��������������ʾ״̬��S2����Ϊ�л�������ʱ��/������������ʾ��������״̬��S2Ϊ�������ơ�
					S5Ϊ���ü�������ʾ����ʱ��״̬�£�����S5Ϊ���ù�����ʱ�ӣ��ٴΰ���S5Ϊд�����ò������˳�
					����ʾ����������״̬�£�����S5Ϊ�������������ӣ��ٴΰ���S5Ϊд�����ò������˳�
					������״̬�£�S3Ϊ���ݼ�
					������״̬�£�S4Ϊ���ݼ�
*  ˵������ѿ�Դ�����ṩԴ�������.
**********************************************************************************/
#define  _MAIN_C
#include "config.h"
#include "LCD1602.h"
#include "DS1302.h"
#include "DS18B20.h"
#include "app.h"
#include "Key_Buzzer.h"
#include "main.h"

bit flag1s = 0;    //1s��ʱ��־λ
bit flag200ms = 0; //200ms��ʱ��־
enum eStaSystem staSystem = E_NORMAL;  //ϵͳ����״̬

void main()
{
	EA = 1;			//�����ж�
	Start18B20();   //�����¶�ת��
	Init_Timer0();  //��ʼ����ʱ��0
	Init_LCD1602(); //LCD1602��ʼ��
	Init_DS1302();	//��ʼ��DS1302
	while (!flag1s);//�ϵ����ʱ1��
    flag1s = 0;
	RefreshTime();	//ˢ��ʱ��
	RefreshDate(1);	//����ˢ������
	RefreshTemp(1); //����ˢ���¶���ʾ

	while(1)
	{
	    KeyDriver();	//�������
		if(flag200ms)	//200msʱ�䵽
		{
			flag200ms = 0;	//�����־λ
			AlarmMonitor(); //�������
			if (staSystem == E_NORMAL)//���ϵͳ��������
			{
				RefreshTime(); //ˢ��ʱ��
				RefreshDate(0);//ˢ������
			}
			if (staSystem == Dis_Alarm)
			{
				RefreshLunar(); //ˢ������
			}
		}	
		if(flag1s && (staSystem == E_NORMAL))//1sʱ�䵽����ϵͳΪ��������
		{
			flag1s = 0;//�����־λ
			RefreshTemp(0);//ˢ���¶�	
		}
   }
}

/* �¶�ˢ�º�������ȡ��ǰ�¶Ȳ�������Ҫˢ��Һ����ʾ��
** ops-ˢ��ѡ�Ϊ0ʱֻ���¶ȱ仯��ˢ�£���0������ˢ�� */
void RefreshTemp(uint8 ops)
{
    int16 temp;
    uint8 pdata str[8];
    static int16 backup = 0;
    
    Get18B20Temp(&temp); //��ȡ��ǰ�¶�ֵ
    Start18B20();        //������һ��ת��
    temp >>= 4;          //����4bitС��λ
    if ((backup!=temp) || (ops!=0)) //����Ҫˢ��Һ����ʾ
    {
        str[0] = (temp / 10) + '0';  //ʮλתΪASCII��
        str[1] = (temp % 10) + '0';  //��λתΪASCII��
        str[2] = '\'';             //��'C�����
        str[3] = 'C';
        str[4] = '\0';             //�ַ���������
        LCD1602_Dis_Str(12, 1, str);    //��ʾ��Һ����
        backup = temp;             //ˢ���ϴ��¶�ֵ
    }
}


/*��ʼ����ʱ��0 ��ʱ5ms*/	
void Init_Timer0()
{
	
	TMOD = 0x01;  //��ʱ��16Ϊ��������ģʽ
	TH0 =0xee;
	TL0 =0x00; //5ms
	ET0 = 1; //����ʱ��0�ж�
	TR0 = 1;//������ʱ��0
}
/*��ʱ��0�жϷ������*/
void Timer0() interrupt 1
{
	static uint8 tmr1s = 0;
    static uint8 tmr200ms = 0;
	
	TH0 =0xee;
	TL0 =0x00; //5ms
	tmr200ms++;  //��ʱ200ms
    if (tmr200ms >= 40)
    {
        tmr200ms = 0;
        flag200ms = 1;
        tmr1s++;  //��ʱ1s
        if (tmr1s >= 5)
        {
            tmr1s = 0;
            flag1s = 1;
        }
    }
	KeyScan();//����ɨ��
}