#include <reg51.h>
 
typedef unsigned char u8;
typedef unsigned int u16;
 
sbit RW=P2^5;
sbit RS=P2^6;
sbit EN=P2^7;
u8 code table[15]="Hello World";
 
/*��ʱ����*/
void delay(u16 i)
{
   while(i--);
 
}
/*дָ���*/
void LCD_WriteCommand(u8 con){
    P0=con;           //�ͳ�ָ��
	  RS=0;RW=0;EN=1;  //дָ��ʱ��
	  delay(200);
	  EN=0;
}
/*д���ݺ���*/
void write_dat(u8 dat){
   P0=dat;		     //�ͳ�����
	 RS=1;RW=0;EN=1;//д����ʱ��
	 delay(200);
	 EN=0;
}
/*LCD��ʼ������*/
void LcdInit(){
   write_con(0x01);//����
	 write_con(0x38);//����16*2��ʾ
	 write_con(0x0c);//����ʾ
	 write_con(0x06);//д������ʱ�������1λ
}
 
/*LCD��ʾ����*/
void LcdDisplay(u8  table[]){
    u8 i=0;
	write_con(0x80);   //���Ϊ��һ��
	while(table[i]!='\0'){
	   write_dat(table[i]);
	   delay(100);
	   i++;
	 }
	/*
	  i=0;
	write_con(0xc0);  //���Ϊ�ڶ���
	while(table[i]!='\0'){
	   write_dat(table[i]);
	   delay(100);
	   i++;
	 }
	 i=0;
	 */
}
 
/*������*/
void main()
{
   LcdInit();
	 while(1)
	 {
   LcdDisplay(table);
	 }
}