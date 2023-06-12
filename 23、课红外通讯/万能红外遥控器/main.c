/**********************************************************************
---------------------��ѧϰ�����ܺ���ң����----------------------------
WEB			��WWW.QXMCU.COM
���Ե�Ƭ��	��STC89C52RC
�ⲿ����	��11.0592M
���Ի���	��KEIL3
����Ӳ��ƽ̨��ʹ�� ������ӿƼ� QX-MCS51��Ƭ�����������ͨ��
��������Ա	������÷
����		��2014-8-8
�ļ���		��main.c

	//-------------------------������-------------------------//	
	ʹ�� ����QX-MCS51��Ƭ��������ʵ��һ����ѧϰ�����ܺ���ң������
	LCD1602ʵʱ��ʾ��ֵ��״̬����ֵ��16������ʾ����
������Ҫ��Ϊѧϰ����ң�ذ��ֵ���β�����������QX-MCS51��Ƭ����������
EEPROM��AT24C02���У��ʹ�EEPROM��ȡѧϰ�ļ�ֵ�벨��ԭ�����������֡�
	
	//---------------------------��ע---------------------------//
	������ϰ���S14��S15��S16��S17��S4�ͺ������ͷ�ⲿ�ж�P32
����������������ʹ�ù��������ϰ�������ѧϰ�ͷ���ʹ�ã�

	//-------------------------ʹ��˵��-------------------------//
	����QX-MCS51��Ƭ���������ϵ��λ�����Ĭ��Ϊѧϰ��ֵ�벨��ģʽ��
	ͨ������S2����ѡ��Ϊѧϰģʽ��S3����ѡ��Ϊ����ģʽ��
������ѧϰģʽʱ���Ȱ���Ҫѧϰ�ĺ���ң�ذ��ϵİ�������ʱ�����û���Ͱ�
���뽫����ʾ��LCD1602�ĵڶ��У�Ȼ���ڰ����������ϵİ�����ѧϰ�˼�ֵ����
�浽EEPROM�У�ѧϰ����ɹ�LCD1602��0x8E��ַ������ʾ01��01��ʾ�ɹ���00��
ʾʧ�ܣ���

�����ڷ���ģʽʱ�����¿������ϰ���LCD1602��һ�л���ʾ���͵ļ�ֵ��
�ڶ�����ʾ���������Ƿ��ͳɹ� ��01��ʾ�ɹ���00��ʾʧ�ܡ���ַ0XC5����ʾ��
**********************************************************************/
#include "qxmcs51_config.h"//����QX-MCS51��Ƭ�������������ļ�
#include "delay.h"		   
#include "LCD1602.h"
#include "IR_TX_RX.H"
#include "KeyScan.h"
#include "IIC_AT24.h"			
//-------------------------------------------
//���̰����ļ�
//main.c
//delay.c
//LCD1602.c
//IR_TX_RX.c
//IIC_AT24.c
//KeyScan.c
//-------------------------------------------

/**/
extern BOOL IR_RX_Finish;	//�����Ƿ������ɣ����=1����=0��
extern BOOL	KeyPro_OK;		//���������Ƿ���ɣ����=1����=0��
extern BOOL	TorR_Mod;		//���ͺ�ѧϰģʽѡ��1���ͣ�0���գ�
extern INT8U IR_RX_CODE[4];	//IR�����������
extern INT8U IR_TX_CODE[4];	//IR��̷�������

/*������*/
void main()
{
	INT8U Temp_Key;	//��Ű�������ֵ
	BOOL I2C_OK;	//I2C��д�Ƿ�ɹ���1�ɹ���0ʧ�ܣ�
	LCD1602_Init();	//LCD1602��ʼ��
	IR_TX_RX_Init();//IR���͡����ճ�ʼ��
	I2C_Init();		//I2C��ʼ��
	Delay_Ms(50);
	LCD1602_Write_String(0, 0, "Welcome to use!"); 
	LCD1602_Write_String(0, 1, "IR Remote Module");	//��ӭ����
	Delay_Ms(5000); //5����ʱ
	LCD1602_Clear();//LCD1602����
	LCD1602_Write_String(0, 0, "Switch Mode!"); 
	LCD1602_Write_String(0, 1, "Default CopyMode"); //����ѡ����ʾ����
	while(1)
	{
		if (IR_RX_Finish) //IR�Ƿ�������
		{
			IR_RX_Finish = 0;
			LCD1602_Clear();
			/*LCD1602��16������ʾIR���յ����û������ֵ��*/
			LCD1602_Write_String(0, 1, INT8UtostrHEX(IR_RX_CODE[0]));
			LCD1602_Write_String(3, 1, INT8UtostrHEX(IR_RX_CODE[1]));
			LCD1602_Write_String(7, 1, INT8UtostrHEX(IR_RX_CODE[2]));
			LCD1602_Write_String(10, 1, INT8UtostrHEX(IR_RX_CODE[3]));
		}
		Temp_Key = KeyPro();//��ȡ��������ֵ
		if (KeyPro_OK)		//���������Ƿ�ɹ�
		{
			KeyPro_OK = 0;
			if (TorR_Mod)	//�Ƿ�ѡ��Ϊ����ģʽ
			{
				if(Temp_Key != 0xfd)	//��ѡ��ģʽ��������(S3,����ģʽ)
				{
					do
					{
						I2C_OK = I2C_ReadData(Temp_Key);
					}while(!I2C_OK);//�ȴ�I2C��ȡ������ɹ�
					Transmit_TR_CODE();//���ͺ������
					LCD1602_Clear();
					LCD1602_Write_String(0, 1, INT8UtostrHEX(Temp_Key));//��ʾѧϰ����ֵ
					LCD1602_Write_String(5, 1, INT8UtostrHEX(I2C_OK));	//��ʾ�ɹ���ȡIR_TX_CODE
					/*LEC1602��16������ʾ��I2Cȡ����IR_TX_CODE*/
					LCD1602_Write_String(0, 0, INT8UtostrHEX(IR_TX_CODE[0]));
					LCD1602_Write_String(3, 0, INT8UtostrHEX(IR_TX_CODE[1]));
					LCD1602_Write_String(7, 0, INT8UtostrHEX(IR_TX_CODE[2]));
					LCD1602_Write_String(10, 0, INT8UtostrHEX(IR_TX_CODE[3]));
				}
				else   //ѡ��ģʽ��������(S3)
				{
					LCD1602_Clear();
					LCD1602_Write_String(0, 0, "IR Transmit Mode");
					LCD1602_Write_String(3, 1, "Please Key!");	
				}	
			}
			else
			{
				if(Temp_Key != 0xfe)	//��ѡ��ģʽ��������(S2��ѧϰģʽ)
				{
					LCD1602_Clear();
					LCD1602_Write_String(14, 1, INT8UtostrHEX(Temp_Key));//��ʾ����ֵ
					do
					{
						I2C_OK = I2C_WriteData(Temp_Key);
					}while(!I2C_OK);//�ȴ������պ��������I2C�ɹ�
					LCD1602_Write_String(14, 0, INT8UtostrHEX(I2C_OK));	//��ʾ����ɹ�
					Delay_Ms(100);
					/*��I2C��ȡ�մ����IRѧϰ��*/
					I2C_ReadData(Temp_Key);
					LCD1602_Write_String(0, 0, INT8UtostrHEX(IR_TX_CODE[0]));
					LCD1602_Write_String(3, 0, INT8UtostrHEX(IR_TX_CODE[1]));
					LCD1602_Write_String(7, 0, INT8UtostrHEX(IR_TX_CODE[2]));
					LCD1602_Write_String(10, 0, INT8UtostrHEX(IR_TX_CODE[3]));
				}
				else	 //ѡ��ģʽ��������(S2��ѧϰģʽ)
				{
					LCD1602_Clear();
					LCD1602_Write_String(0, 0, "IRcode Copy Mode");
					LCD1602_Write_String(0, 1, "Please In IRcode");	
				}
			} 
		}
	}
}