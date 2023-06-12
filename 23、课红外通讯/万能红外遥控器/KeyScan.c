#include "qxmcs51_config.h"
#include "delay.h"
#include "KeyScan.h"

BOOL KeyScan_OK, KeyPro_OK, TorR_Mod; 

INT8U KeyScan()	//������ֵ���Ӻ���
{
	INT8U cord_l,cord_h;//�������ߺ����ߵ�ֵ�Ĵ������
	P3 = 0x0f;//0000 1111
	if( (P3 & 0x0f) != 0x0f)//�ж��Ƿ��а�������
	{
		Delay_Ms(5);//�������
		if( (P3 & 0x0f) != 0x0f)//�ж��Ƿ��а�������
		{
			  cord_l = P3 & 0x0f;// ��������ֵ
			  P3 = cord_l | 0xf0;
			  cord_h = P3 & 0xf0;// ��������ֵ
			  while((P3 & 0xf0) != 0xf0 );//���ּ��
			  KeyScan_OK = 1;
			  return (cord_l + cord_h);//���ؼ�ֵ��
		}	
	}		
}

INT8U KeyPro()
{
	INT8U Key_Value;		
	Key_Value = KeyScan();
	if (KeyScan_OK)
	{
		KeyScan_OK = 0;		
		switch(Key_Value)
		{
		 	
			//��һ�м�ֵ��
			case 0xee: Key_Value = 0x00;		break;
			case 0xde: Key_Value = 0x04;		break;
			case 0xbe: Key_Value = 0x08;		break;
			case 0x7e: Key_Value = 0x0b;		break;
			
			//�ڶ��м�ֵ��
			case 0xed: Key_Value = 0x10;		break;
			case 0xdd: Key_Value = 0x14;		break;
			case 0xbd: Key_Value = 0x18;		break;
			case 0x7d: Key_Value = 0x1b;		break;
	
			//�����м�ֵ��
			case 0xeb: Key_Value = 0x09;		break;
			case 0xdb: Key_Value = 0x0A;		break;
			case 0xbb: Key_Value = 0x0b;		break;
			case 0x7b: Key_Value = 0x0C;		break;
	
			//�����м�ֵ��
			case 0xe7: Key_Value = 0x20;		break;
			case 0xd7: Key_Value = 0x24;		break;
			case 0xb7: Key_Value = 0x28;		break;
			case 0x77: Key_Value = 0x2b;		break;
							
			//��������
			case 0xfe: TorR_Mod  = 0;			break;
			case 0xfd: TorR_Mod  = 1;			break;
			case 0xfb: Key_Value = 0x13;		break;
			case 0xf7: Key_Value = 0x30;		break;

		}
		KeyPro_OK = 1;	
		BEEP = 0;
		Delay_Ms(100);
		BEEP = 1;
		return (Key_Value);
	}	
}
