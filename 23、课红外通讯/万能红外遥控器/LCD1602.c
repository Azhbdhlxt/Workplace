#include "qxmcs51_config.h"
#include "delay.h"
#include "LCD1602.H"

#define LCD1602_DataPort P0	  //LCD1602���ݶ˿�

INT8U str[3];//INT8Utostr����ת�����ַ���
INT8U *INT8UtostrHEX(INT8U num)	//��һ���ֽڵ�����ת��Ϊ�ַ���
{
	INT8U i = 0;
	switch(num/16)
	{
		case 0:		str[i] = '0';	i++;	break;
		case 1:		str[i] = '1';	i++;	break;
		case 2:		str[i] = '2';	i++;	break;
		case 3:		str[i] = '3';	i++;	break;
		case 4:		str[i] = '4';	i++;	break;
		case 5:		str[i] = '5';	i++;	break;
		case 6:		str[i] = '6';	i++;	break;
		case 7:		str[i] = '7';	i++;	break;
		case 8:		str[i] = '8';	i++;	break;
		case 9:		str[i] = '9';	i++;	break;
		case 10:	str[i] = 'A';	i++;	break;
		case 11:	str[i] = 'B';	i++;	break;
		case 12:	str[i] = 'C';	i++;	break;
		case 13:	str[i] = 'D';	i++;	break;
		case 14:	str[i] = 'E';	i++;	break;
		case 15:	str[i] = 'F';	i++;	break;
	}

	switch(num%16)
	{
		case 0:		str[i] = '0';	i++;	break;
		case 1:		str[i] = '1';	i++;	break;
		case 2:		str[i] = '2';	i++;	break;
		case 3:		str[i] = '3';	i++;	break;
		case 4:		str[i] = '4';	i++;	break;
		case 5:		str[i] = '5';	i++;	break;
		case 6:		str[i] = '6';	i++;	break;
		case 7:		str[i] = '7';	i++;	break;
		case 8:		str[i] = '8';	i++;	break;
		case 9:		str[i] = '9';	i++;	break;
		case 10:	str[i] = 'A';	i++;	break;
		case 11:	str[i] = 'B';	i++;	break;
		case 12:	str[i] = 'C';	i++;	break;
		case 13:	str[i] = 'D';	i++;	break;
		case 14:	str[i] = 'E';	i++;	break;
		case 15:	str[i] = 'F';	i++;	break;
	}
	str[i]='\0';
	return str;
}

BOOL LCD1602_Check_Busy()	//LCD1602æ�ж� 
{ 
	LCD1602_DataPort = 0xFF; 
	LCD_RS = 0; //���ݹ涨��RSΪ�͵�ƽ��RWΪ�ߵ�ƽʱ�����Զ�״̬
	LCD_RW = 1; 
	LCD_EN = 0; 
	_nop_(); 
	LCD_EN = 1;
	return (BOOL)(LCD1602_DataPort & 0x80);
}

void LCD1602_Clear()
{
   LCD1602_Write_Com(0x01);    /*��ʾ����*/ 
   Delay_Ms(5);	
}
/*
INT8U LCDE1602_DATAP_ADDR()
{
	LCD1602_DataPort = 0xFF; 
	LCD_RS = 0; //���ݹ涨��RSΪ�͵�ƽ��RWΪ�ߵ�ƽʱ�����Զ�״̬
	LCD_RW = 1; 
	LCD_EN = 0; 
	_nop_(); 
	LCD_EN = 1;
	return (LCD1602_DataPort & 0x7f);	
} */

void LCD1602_Write_Com(INT8U com)  //LCD1602д����
{
	while(LCD1602_Check_Busy()); //æ��ȴ�
	LCD_RS = 0; 
	LCD_RW = 0; 
	LCD_EN = 1; 
	LCD1602_DataPort= com; 
	_nop_(); 
	LCD_EN = 0;	
}

void LCD1602_Write_Dat(INT8U dat)	  //LCD1602д����
{
	while(LCD1602_Check_Busy()); //æ��ȴ�
	LCD_RS = 1;
	LCD_RW = 0; 
	LCD_EN = 1;
	LCD1602_DataPort= dat; 
 	_nop_();
	LCD_EN = 0;		
}


void LCD1602_Write_String(INT8U x,INT8U y,INT8U *s) //LCD1602д�ַ���
{     
	if (y == 0) 
	{     
		LCD1602_Write_Com(0x80 + x);     //��ʾ��һ��
	}
	else 
	{      
		LCD1602_Write_Com(0xC0 + x);      //��ʾ�ڶ���
	}        
	while (*s != '\0') 
	{     
		LCD1602_Write_Dat(*s++);         
	}
}

void LCD1602_Init() //LCD1602��ʼ��
{
   LCD1602_Write_Com(0x38);    /*��ʾģʽ����*/ 
   LCD1602_Write_Com(0x06);    /*��ʾ����ƶ�����*/
   LCD1602_Write_Com(0x0C);    /*��ʾ�����������*/
   LCD1602_Write_Com(0x01);    /*��ʾ����*/ 
   Delay_Ms(5);
}